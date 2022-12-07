package com.rusefi.newparse.outputs;

import java.io.OutputStream;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

public class PrintStreamAlwaysUnix extends PrintStream {
    public PrintStreamAlwaysUnix(OutputStream out) {
        super(out);
    }

    public PrintStreamAlwaysUnix(OutputStream out, boolean autoFlush, String encoding) throws UnsupportedEncodingException {
        super(out, autoFlush, encoding);
    }

    @Override
    public void println() {
        print('\n');
    }

    @Override
    public void println(String s) {
        print(s);
        println();
    }
}
