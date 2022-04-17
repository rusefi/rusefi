package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.TSProjectConsumer;

import java.io.CharArrayWriter;

public class TestTSProjectConsumer extends TSProjectConsumer {
    public TestTSProjectConsumer(String tsPath, ReaderState state) {
        super(tsPath, state);
    }

    @Override
    public void endFile() {
    }
}
