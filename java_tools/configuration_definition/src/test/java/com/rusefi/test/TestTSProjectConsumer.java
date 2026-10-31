package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.TSProjectConsumer;

public class TestTSProjectConsumer extends TSProjectConsumer {
    public TestTSProjectConsumer(ReaderStateImpl state) {
        super(null, state);
    }

    @Override
    public void endFile() {
    }
}
