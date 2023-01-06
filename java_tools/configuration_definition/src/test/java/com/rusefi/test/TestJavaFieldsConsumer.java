package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.JavaFieldsConsumer;

public class TestJavaFieldsConsumer extends JavaFieldsConsumer {
    public TestJavaFieldsConsumer(ReaderStateImpl state) {
        super(state, 0);
    }

    @Override
    public void endFile() {
    }
}
