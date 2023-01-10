package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.TSProjectConsumer;

public class TestTSProjectConsumer extends TSProjectConsumer {
    public TestTSProjectConsumer(String tsPath, ReaderStateImpl state) {
        super(tsPath, state);
    }

    @Override
    public void endFile() {
    }
}
