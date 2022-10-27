package com.rusefi.output;

import com.rusefi.*;
import com.rusefi.util.SystemOut;

import java.io.IOException;

/**
 * Configuration consumer which writes Signature header file
 */
public class SignatureConsumer implements ConfigurationConsumer {
    private final String destHeader;
    private final VariableRegistry registry;

    public SignatureConsumer(String destHeader, VariableRegistry registry) {
        SystemOut.println("Writing Signature header to " + destHeader);
        this.destHeader = destHeader;
        this.registry = registry;
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        ExtraUtil.writeDefinesToFile(registry, destHeader, "by SignatureConsumer");
    }
}
