package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.ConfigStructure;
import com.rusefi.output.ConfigurationConsumer;
import com.rusefi.output.DataLogConsumer;
import com.rusefi.output.SdCardFieldsContent;
import com.rusefi.util.LazyFile;
import com.rusefi.util.LazyFileImpl;

import java.io.IOException;

/**
 * @see DataLogConsumer
 */
public class SdCardFieldsTestConsumer implements ConfigurationConsumer {

    private final SdCardFieldsContent content = new SdCardFieldsContent();
    private final LazyFile output;

    public SdCardFieldsTestConsumer(String outputFileName, boolean isPtr) {
        output = new LazyFileImpl(outputFileName);
        content.isPtr = isPtr;
    }

    @Override
    public void endFile() throws IOException {
        SdCardFieldsContent.wrapContent(output, getBody());
        output.close();
    }

    @Override
    public void handleEndStruct(ReaderState state, ConfigStructure structure) throws IOException {
        content.handleEndStruct(state, structure);
    }

    public String getBody() {
        return content.getBody();
    }
}
