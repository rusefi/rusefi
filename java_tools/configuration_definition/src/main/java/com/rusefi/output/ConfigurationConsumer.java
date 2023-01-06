package com.rusefi.output;

import com.rusefi.IReaderState;

import java.io.IOException;

public interface ConfigurationConsumer {
    default void startFile() {

    }

    default void endFile() throws IOException {

    }

    void handleEndStruct(IReaderState readerState, ConfigStructure structure) throws IOException;
}
