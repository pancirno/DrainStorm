// Export direct function-to-function calls discovered by Ghidra.
// @category DrainStorm

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.Set;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

public class ExportCallGraph extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) {
            throw new IllegalArgumentException("Expected one output-directory argument");
        }

        File outputDirectory = new File(args[0]);
        if (!outputDirectory.isDirectory() && !outputDirectory.mkdirs()) {
            throw new IllegalStateException("Cannot create " + outputDirectory);
        }

        File outputFile = new File(outputDirectory, "callgraph.csv");
        int edgeCount = 0;
        try (BufferedWriter output = new BufferedWriter(new FileWriter(outputFile))) {
            output.write("caller_address,caller_name,callee_address,callee_name\n");
            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            while (functions.hasNext() && !monitor.isCancelled()) {
                Function caller = functions.next();
                Set<Function> callees = caller.getCalledFunctions(monitor);
                for (Function callee : callees) {
                    output.write(csv(caller.getEntryPoint().toString()));
                    output.write("," + csv(caller.getName()));
                    output.write("," + csv(callee.getEntryPoint().toString()));
                    output.write("," + csv(callee.getName()));
                    output.write("\n");
                    edgeCount++;
                }
            }
        }
        println("Exported " + edgeCount + " call-graph edges");
    }

    private static String csv(String value) {
        if (value == null) {
            return "";
        }
        return "\"" + value.replace("\"", "\"\"") + "\"";
    }
}
