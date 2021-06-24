package com.rusefi.newparse.outputs;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.layout.StructLayout;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class TsWriter {
    public void writeTunerstudio(ParseState parser, String inputFile, String outputFile) throws FileNotFoundException {
        // Build the TunerStudio layout
        StructLayout root = new StructLayout(0, "root", parser.getLastStruct());

        TsMetadata meta = new TsMetadata();

        // Write to file!
        PrintStream ps = new PrintStream(new FileOutputStream(outputFile));

        // Print configuration layout
        root.writeTunerstudioLayout(ps, meta);
        ps.println("; total TS size = " + root.getSize());

        // Print context help
        ps.println("[SettingContextHelp]");
        meta.writeComments(ps);

        ps.close();
    }
}
