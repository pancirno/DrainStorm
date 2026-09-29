// Create functions whose far pointers are embedded indirectly in Win16 structures.
// @category DrainStorm

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

public class ApplyKnownEntryPoints extends GhidraScript {
    private record KnownEntryPoint(String address, String name) {}

    private static final KnownEntryPoint[] ENTRY_POINTS = {
        new KnownEntryPoint("1008:4584", "ArsenalWindowProcedure"),
        new KnownEntryPoint("1008:5be4", "BoardWindowProcedure"),
        new KnownEntryPoint("1008:6b88", "MainWindowProcedure")
    };

    @Override
    public void run() throws Exception {
        for (KnownEntryPoint known : ENTRY_POINTS) {
            Address address = currentProgram.getAddressFactory().getAddress(known.address());
            if (address == null) {
                throw new IllegalArgumentException("Invalid address " + known.address());
            }

            Function function = currentProgram.getFunctionManager().getFunctionAt(address);
            if (function == null) {
                disassemble(address);
                function = createFunction(address, null);
            }
            if (function == null) {
                throw new IllegalStateException("Could not create function at " + address);
            }
            function.setName(known.name(), SourceType.USER_DEFINED);
            println(known.address() + " -> " + known.name());
        }
    }
}
