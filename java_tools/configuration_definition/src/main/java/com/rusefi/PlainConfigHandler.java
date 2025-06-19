package com.rusefi;

import com.rusefi.output.PlainTsProjectConsumer;

import java.io.IOException;

class PlainConfigHandler {
    ReaderStateImpl readerState = new ReaderStateImpl();
    PlainTsProjectConsumer tsProjectConsumer = new PlainTsProjectConsumer(readerState);

    public PlainConfigHandler(String definitionInputFile) {
        readerState.setDefinitionInputFile(definitionInputFile);
        readerState.addPrepend("integration/rusefi_config_shared.txt");
        readerState.addPrepend("integration/ts_protocol.txt");

        readerState.addDestination(tsProjectConsumer);
    }

    public void doJob() throws IOException {
        readerState.doJob();
    }
}
