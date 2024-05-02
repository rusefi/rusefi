package com.rusefi.newparse.outputs;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.layout.StructLayout;
import com.rusefi.newparse.parsing.Struct;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class JavaFieldsWriter {
    private final PrintStream ps;

    public JavaFieldsWriter(String outputFile, String javaConstants) throws FileNotFoundException {
        this.ps = new PrintStreamAlwaysUnix(new FileOutputStream(outputFile));

        ps.println(javaConstants);
    }

    public void writeJavaFields(ParseState parser) {
        Struct s = parser.getStructs().get(parser.getStructs().size() - 1);

        StructLayout sl = new StructLayout(0, "root", s);
    }
}
