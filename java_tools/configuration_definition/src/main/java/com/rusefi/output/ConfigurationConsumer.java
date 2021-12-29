package com.rusefi.output;

import com.rusefi.ReaderState;

import java.io.IOException;

public interface ConfigurationConsumer {
    void startFile() throws IOException;

    void endFile() throws IOException;

    void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException;
}
