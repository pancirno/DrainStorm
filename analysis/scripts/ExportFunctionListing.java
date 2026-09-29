// Export instruction listings for one or more named functions.
// @category DrainStorm

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class ExportFunctionListing extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) {
            throw new IllegalArgumentException("Expected function-name and output-file arguments");
        }
        Function function = getGlobalFunctions(args[0]).stream().findFirst()
            .orElseThrow(() -> new IllegalArgumentException("Function not found: " + args[0]));
        File output = new File(args[1]);
        File parent = output.getParentFile();
        if (parent != null) {
            parent.mkdirs();
        }
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(output))) {
            InstructionIterator instructions = currentProgram.getListing()
                .getInstructions(function.getBody(), true);
            while (instructions.hasNext()) {
                Instruction instruction = instructions.next();
                writer.write(instruction.getAddress() + "  " + instruction.toString());
                writer.write("\n");
            }
        }
        println(function.getName() + " -> " + output.getAbsolutePath());
    }
}
