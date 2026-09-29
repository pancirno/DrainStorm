"""Read-only reachable direct near-call discovery, not a full disassembler.

Starts at exported and manually reviewed game/UI entries, follows direct
branches and the explicitly reviewed bounded switch tables below. Any other
indirect jump is reported; no linear scanning of embedded data is used as proof.
"""
import csv
from collections import defaultdict
from pathlib import Path
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_CS, X86_REG_BX

# Each entry was checked against its dominating unsigned CMP/bounds branch,
# index doubling and CS:[BX + table] jump. Values are entry counts, not
# guessed function sizes. Tables begin immediately after the five-byte JMP.
switch_counts = {
    0x1008: {0x0e3c:18,0x2057:21,0x2ee2:31,0x63cd:176,0x6664:45,
             0x8278:11,0x8d49:12,0x9863:11,0x99fc:8,0x9a30:8,
             0x9a64:8,0x9a9e:8,0x9f09:45,0xa1a1:17,0xa3e2:19,
             0xad74:9,0xb1e2:95,0xc04b:15,0xc0d9:8,0xc253:8,0xc7f9:11},
    0x1010: {0x9b89:12,0x9f8b:9},
}

root = Path(__file__).resolve().parents[2]
exe = (root/'Drain Storm/DRAIN.EXE').read_bytes()
ne = struct.unpack_from('<I',exe,0x3c)[0]
table = ne+struct.unpack_from('<H',exe,ne+0x22)[0]
shift = struct.unpack_from('<H',exe,ne+0x32)[0]
with (root/'analysis/export/functions.csv').open(newline='') as stream:
    exports = {r['address'] for r in csv.DictReader(stream)}
with (root/'analysis/audit/review-ledger.csv').open(newline='') as stream:
    ledger = list(csv.DictReader(stream))
reviewed = {r['original_address'] for r in ledger}
excluded = {r['original_address'] for r in ledger if r['status']=='not-a-function'}
md = Cs(CS_ARCH_X86,CS_MODE_16)
md.detail = True
for selector in (0x1008,0x1010,0x1018):
    sector,length = struct.unpack_from('<HH',exe,table+((selector-0x1000)//8)*8)
    data = exe[sector<<shift:(sector<<shift)+(length or 65536)]
    roots = sorted(int(a.split(':')[1],16) for a in (exports|reviewed)-excluded
                   if a.startswith(f'{selector:04x}:'))
    pending = roots[:]
    seen = set()
    calls = defaultdict(set)
    indirect = set()
    resolved_switches = set()
    invalid = set()
    while pending:
        pc = pending.pop()
        while 0 <= pc < len(data) and pc not in seen:
            ins = next(md.disasm(data[pc:pc+15],pc,count=1),None)
            if ins is None:
                invalid.add(pc)
                break
            seen.add(pc)
            immediate = bool(ins.operands and ins.operands[0].type==X86_OP_IMM)
            if ins.mnemonic in ('ret','retf','iret','int3','hlt'):
                break
            if ins.mnemonic=='call' and immediate:
                target = ins.operands[0].imm & 0xffff
                calls[target].add(pc)
                pending.append(target)
            if ins.mnemonic.startswith('j') or ins.mnemonic.startswith('loop'):
                if immediate:
                    pending.append(ins.operands[0].imm & 0xffff)
                else:
                    count = switch_counts.get(selector,{}).get(pc)
                    if count is None:
                        indirect.add(pc)
                    else:
                        operand = ins.operands[0]
                        assert ins.mnemonic == 'jmp' and ins.size == 5
                        assert operand.type == X86_OP_MEM
                        assert operand.mem.segment == X86_REG_CS and operand.mem.base == X86_REG_BX
                        assert operand.mem.disp & 0xffff == pc + 5
                        targets = struct.unpack_from(f'<{count}H',data,pc+5)
                        assert all(0 <= target < len(data) for target in targets)
                        pending.extend(targets)
                        resolved_switches.add(pc)
                if ins.mnemonic in ('jmp','ljmp'):
                    break
            pc += ins.size
    unknown = {target:sites for target,sites in calls.items()
               if f'{selector:04x}:{target:04x}' not in exports|reviewed}
    print(f'{selector:04x}: roots={len(roots)} instructions={len(seen)} '
          f'near_targets={len(calls)} unindexed_targets={len(unknown)} '
          f'resolved_switches={len(resolved_switches)} '
          f'unresolved_indirect_jumps={len(indirect)} undecodable={len(invalid)}')
    for target,sites in sorted(unknown.items()):
        print(f'  candidate {selector:04x}:{target:04x} from '
              + ' '.join(f'{pc:04x}' for pc in sorted(sites)))
    print('  indirect jumps:', ' '.join(f'{pc:04x}' for pc in sorted(indirect)))
