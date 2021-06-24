package com.rusefi.newparse.outputs;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.layout.StructLayout;

import java.io.*;

public class TsWriter {
    public void writeTunerstudio(ParseState parser, String inputFile, String outputFile) throws FileNotFoundException, IOException {
        BufferedReader is = new BufferedReader(new FileReader(inputFile));
        PrintStream ps = new PrintStream(new FileOutputStream(outputFile));

        while (is.ready()) {
            String line = is.readLine();

            if (line.contains("CONFIG_DEFINITION_START")) {
                writeLayoutAndComments(parser, ps);
                continue;
            }

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
