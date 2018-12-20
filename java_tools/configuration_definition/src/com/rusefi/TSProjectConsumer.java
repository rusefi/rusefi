package com.rusefi;

import java.io.CharArrayWriter;
import java.io.IOException;

public class TSProjectConsumer implements ConfigurationConsumer {
    private final CharArrayWriter tsWriter;
    private static int totalTsSize;

    public TSProjectConsumer(ReaderState state, CharArrayWriter tsWriter) {
        this.tsWriter = tsWriter;
    }

    @Override
    public void startFile() throws IOException {

    }

    @Override
    public void endFile() throws IOException {

    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
    }

    @Override
    public void onTotalEnd() {

    }
}
