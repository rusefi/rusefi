package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.JavaFieldsConsumer;

public class TestJavaFieldsConsumer extends JavaFieldsConsumer {
    public TestJavaFieldsConsumer(ReaderState state) {
        super(state);
    }

    @Override
    public void startFile() {
    }

    @Override
    public void endFile() {
    }
}
