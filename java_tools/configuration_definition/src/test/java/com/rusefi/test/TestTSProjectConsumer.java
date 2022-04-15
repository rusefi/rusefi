package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.TSProjectConsumer;

import java.io.CharArrayWriter;

public class TestTSProjectConsumer extends TSProjectConsumer {
    public TestTSProjectConsumer(CharArrayWriter writer, String tsPath, ReaderState state) {
        super(writer, tsPath, state);
    }

    @Override
    public void endFile() {
    }
}
