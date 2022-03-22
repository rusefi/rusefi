package com.rusefi.newparse.outputs;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.layout.StructLayout;
import com.rusefi.newparse.parsing.Struct;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

/**
 * proposed alternative which does not seem to be covered by any unit tests
 */
public class CStructWriter {
    public void writeCStructs(ParseState parser, String outputFile) throws FileNotFoundException {
        PrintStream ps = new PrintStream(new FileOutputStream(outputFile));

        ps.println(
                "// begin\n" +
                "#pragma once\n" +
                "#include \"rusefi_types.h\""
        );

        for (Struct s : parser.getStructs()) {
            StructLayout sl = new StructLayout(0, "root", s);
            sl.writeCLayoutRoot(ps);
        }

        ps.println("// end");

        ps.close();
    }
}
