package com.rusefi.newparse.outputs;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.layout.StructLayout;
import com.rusefi.newparse.layout.StructNamePrefixer;
import com.rusefi.newparse.parsing.Struct;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class SdLogWriter {
    private final PrintStream ps;

    public SdLogWriter(String outputFile) throws FileNotFoundException {
        this(new PrintStreamAlwaysUnix(new FileOutputStream(outputFile)));
    }

    public SdLogWriter(PrintStream ps) {
        this.ps = ps;

        ps.println("static constexpr LogField fields[] = {");
        ps.println("\t{packedTime, GAUGE_NAME_TIME, \"sec\", 0},");
    }

    public void endFile() {
        ps.println("};");
    }

    public void writeSdLogs(ParseState parser, String sourceName) {
        // Assume the last struct is the one we want...
        Struct s = parser.getStructs().get(parser.getStructs().size() - 1);

        StructLayout sl = new StructLayout(0, "root", s);

        SdLogVisitor v = new SdLogVisitor(sourceName);
        StructNamePrefixer prefixer = new StructNamePrefixer('.');

        v.visit(sl, ps, prefixer, 0, new int[0]);
    }
}
