package com.rusefi.newparse.outputs;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.layout.StructLayout;
import com.rusefi.newparse.parsing.Definition;

import java.io.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TsWriter {
    // matches strings in the form of @@MY_var_123@@
    private static final Pattern VAR = Pattern.compile("@@([a-zA-Z0-9_]+?)@@");

    private static final Pattern OPTIONAL_LINE = Pattern.compile("@@if_([a-zA-Z0-9_]+)");

    public void writeTunerstudio(ParseState parser, String inputFile, String outputFile) throws FileNotFoundException, IOException {
        BufferedReader is = new BufferedReader(new FileReader(inputFile));
        PrintStream ps = new PrintStream(new FileOutputStream(outputFile));

        // TODO: handle signature
        parser.addDefinition("TS_SIGNATURE", "sig", Definition.OverwritePolicy.NotAllowed);

        while (is.ready()) {
            String line = is.readLine();

            if (line.contains("CONFIG_DEFINITION_START")) {
                writeLayoutAndComments(parser, ps);
                continue;
            }

            // Check if this line has a "skip me" suffix
            Matcher match = OPTIONAL_LINE.matcher(line);
            if (match.find()) {
                String varName = match.group(1);

                Definition def = parser.findDefinition(varName);

                if (def == null || def.toString().equals("false")) {
                    // Definition was missing or false, skip this line
                    continue;
                }

                // Delete that part of the line
                line = line.replace(match.group(0), "");
            }

            // While there is a line to replace, do it
            while (line.contains("@@")) {
                match = VAR.matcher(line);

                if (!match.find()) {
                    throw new RuntimeException("Failed to resolve definition in line: " + line);
                }

                String varName = match.group(1);
                Definition def = parser.findDefinition(varName);
                line = line.replaceAll(match.group(0), def != null ? def.toString() : "MISSING DEFINITION");
            }

            // TODO: remove extra whitespace from the line

            // Copy the line to the output stream
            ps.println(line);
        }

        is.close();
        ps.close();
    }

    private void writeLayoutAndComments(ParseState parser, PrintStream ps) {
        StructLayout root = new StructLayout(0, "root", parser.getLastStruct());
        TsMetadata meta = new TsMetadata();

        // Print configuration layout
        int size = root.getSize();
        ps.println("pageSize            = " + size);
        ps.println("page = 1");
        root.writeTunerstudioLayout(ps, meta);
        ps.println("; total TS size = " + size);

        // Print context help
        ps.println("[SettingContextHelp]");
        meta.writeComments(ps);
    }
}
