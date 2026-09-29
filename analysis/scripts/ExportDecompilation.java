// Export discovered functions and Ghidra decompiler output for review and reconstruction.
// @category DrainStorm

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

public class ExportDecompilation extends GhidraScript {
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

        DecompInterface decompiler = new DecompInterface();
        decompiler.toggleCCode(true);
        decompiler.toggleSyntaxTree(true);
        if (!decompiler.openProgram(currentProgram)) {
            throw new IllegalStateException("Decompiler could not open program");
        }

        File inventoryFile = new File(outputDirectory, "functions.csv");
        File decompilationFile = new File(outputDirectory, "decompiled.c");
        int count = 0;
        int failures = 0;

        try (BufferedWriter inventory = new BufferedWriter(new FileWriter(inventoryFile));
             BufferedWriter decompilation = new BufferedWriter(new FileWriter(decompilationFile))) {
            inventory.write("address,name,size,parameter_count,calling_convention\n");

            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            while (functions.hasNext() && !monitor.isCancelled()) {
                Function function = functions.next();
                count++;
                inventory.write(csv(function.getEntryPoint().toString()));
                inventory.write("," + csv(function.getName()));
                inventory.write("," + function.getBody().getNumAddresses());
                inventory.write("," + function.getParameterCount());
                inventory.write("," + csv(function.getCallingConventionName()));
                inventory.write("\n");

                decompilation.write("\n/* ============================================================\n");
                decompilation.write("   " + function.getEntryPoint() + "  " + function.getName() + "\n");
                decompilation.write("   ============================================================ */\n");

                DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
                if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                    decompilation.write(result.getDecompiledFunction().getC());
                    decompilation.write("\n");
                }
                else {
                    failures++;
                    decompilation.write("/* DECOMPILATION FAILED: " + result.getErrorMessage() + " */\n");
                }
            }
        }
        finally {
            decompiler.dispose();
        }

        println("Exported " + count + " functions; " + failures + " decompilation failures");
    }

    private static String csv(String value) {
        if (value == null) {
            return "";
        }
        return "\"" + value.replace("\"", "\"\"") + "\"";
    }
}
