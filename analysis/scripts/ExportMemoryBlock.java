// Export the initialized bytes of the memory block containing a requested address.
// @category DrainStorm

import java.io.File;
import java.io.FileOutputStream;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;

public class ExportMemoryBlock extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) {
            throw new IllegalArgumentException("Expected address and output-file arguments");
        }
        Address address = currentProgram.getAddressFactory().getAddress(args[0]);
        MemoryBlock block = currentProgram.getMemory().getBlock(address);
        if (block == null) {
            throw new IllegalArgumentException("No memory block contains " + args[0]);
        }
        if (!block.isInitialized()) {
            throw new IllegalArgumentException("Memory block is uninitialized: " + block.getName());
        }
        if (block.getSize() > Integer.MAX_VALUE) {
            throw new IllegalArgumentException("Memory block is too large to export");
        }

        byte[] bytes = new byte[(int)block.getSize()];
        int read = currentProgram.getMemory().getBytes(block.getStart(), bytes);
        if (read != bytes.length) {
            throw new IllegalStateException("Read " + read + " of " + bytes.length + " bytes");
        }
        File output = new File(args[1]);
        File parent = output.getParentFile();
        if (parent != null) {
            parent.mkdirs();
        }
        try (FileOutputStream stream = new FileOutputStream(output)) {
            stream.write(bytes);
        }
        println(block.getName() + " " + block.getStart() + "-" + block.getEnd() +
                " -> " + output.getAbsolutePath());
    }
}
