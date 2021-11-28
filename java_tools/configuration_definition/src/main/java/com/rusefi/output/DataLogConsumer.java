package com.rusefi.output;

import com.rusefi.ReaderState;

import java.io.IOException;

public class DataLogConsumer implements ConfigurationConsumer {
    private final ReaderState state;

    public DataLogConsumer(ReaderState state) {
        this.state = state;
    }

    @Override
    public void startFile() throws IOException {
        System.out.println("startFile");

    }

    @Override
    public void endFile() throws IOException {
        System.out.println("endFile");

    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {

        }
    }
}
