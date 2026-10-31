package com.rusefi.output;

import com.rusefi.ReaderState;
import com.rusefi.ReaderStateImpl;

import java.io.IOException;

public class PlainTsProjectConsumer implements ConfigurationConsumer {

    private final TSProjectConsumer.TSProjectConsumerState consumerState;

    public PlainTsProjectConsumer(ReaderStateImpl readerState) {
        consumerState = new TSProjectConsumer.TSProjectConsumerState(readerState, new TsOutput(true));
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        consumerState.handleEndStruct(readerState, structure);
    }

    public String getContent() {
        return consumerState.tsOutput.getContent();
    }

    public int getTotalSize() {
        return consumerState.getTotalSize();
    }
}
