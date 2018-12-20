package com.rusefi;

import java.io.CharArrayWriter;
import java.io.IOException;

public class JavaFieldsConsumer implements ConfigurationConsumer {
    private final CharArrayWriter javaFieldsWriter;
    private final ReaderState state;

    public JavaFieldsConsumer(CharArrayWriter javaFieldsWriter, ReaderState state) {
        this.javaFieldsWriter = javaFieldsWriter;
        this.state = state;
    }

    @Override
    public void startFile() throws IOException {

    }

    @Override
    public void endFile() throws IOException {

    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            structure.writeJavaFields(state,"", javaFieldsWriter, 0);
        }
    }

    @Override
    public void onTotalEnd() {

    }
}
