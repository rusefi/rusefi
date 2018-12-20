package com.rusefi;

import java.io.IOException;

public interface ConfigurationConsumer {
    void startFile() throws IOException;

    void endFile() throws IOException;

    void handleEndStruct(ConfigStructure structure) throws IOException;
}
