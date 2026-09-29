// Export defined strings and the functions that reference them.
// @category DrainStorm

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;

public class ExportStringReferences extends GhidraScript {
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

        File outputFile = new File(outputDirectory, "string_references.csv");
        int count = 0;
        try (BufferedWriter output = new BufferedWriter(new FileWriter(outputFile))) {
            output.write("string_address,string,reference_address,function_address,function_name\n");
            DataIterator dataItems = currentProgram.getListing().getDefinedData(true);
            while (dataItems.hasNext() && !monitor.isCancelled()) {
                Data data = dataItems.next();
                Object value = data.getValue();
                if (!(value instanceof String)) {
                    continue;
                }
                ReferenceIterator references = currentProgram.getReferenceManager()
                    .getReferencesTo(data.getAddress());
                while (references.hasNext()) {
                    Reference reference = references.next();
                    Function function = currentProgram.getFunctionManager()
                        .getFunctionContaining(reference.getFromAddress());
                    output.write(csv(data.getAddress().toString()));
                    output.write("," + csv((String)value));
                    output.write("," + csv(reference.getFromAddress().toString()));
                    output.write("," + csv(function == null ? "" : function.getEntryPoint().toString()));
                    output.write("," + csv(function == null ? "" : function.getName()));
                    output.write("\n");
                    count++;
                }
            }
        }
        println("Exported " + count + " string references");
    }

    private static String csv(String value) {
        if (value == null) {
            return "";
        }
        return "\"" + value.replace("\"", "\"\"") + "\"";
    }
}
