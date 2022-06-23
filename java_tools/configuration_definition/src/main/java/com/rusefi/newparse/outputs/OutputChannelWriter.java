package com.rusefi.newparse.outputs;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.layout.StructLayout;
import com.rusefi.newparse.parsing.Struct;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class OutputChannelWriter {
    private final PrintStream ps;

    public OutputChannelWriter(String outputFile) throws FileNotFoundException {
        this.ps = new PrintStreamAlwaysUnix(new FileOutputStream(outputFile));
    }

    public OutputChannelWriter(PrintStream ps) {
        this.ps = ps;
    }

    private int cumulativeSize = 0;

    public void writeOutputChannels(ParseState parser, String namePrefix) throws FileNotFoundException {
        // Assume the last struct is the one we want...
        Struct s = parser.getStructs().get(parser.getStructs().size() - 1);

        StructLayout sl = new StructLayout(0, "root", s);
        sl.writeOutputChannelLayout(ps, namePrefix, cumulativeSize);

        cumulativeSize += sl.getSize();
        ps.println("; total TS size = " + cumulativeSize);
    }
}
