package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.TsFileContent;
import com.rusefi.output.TSProjectConsumer;
import com.rusefi.util.Output;

import java.io.IOException;

public class TestTSProjectConsumer extends TSProjectConsumer {
    public TestTSProjectConsumer(ReaderStateImpl state) {
        super(null, state);
    }

    @Override
    public void endFile() {
    }

    // exposes the protected page-table emission for unit testing
    public void registerTsPagesBlockForTest() {
        registerTsPagesBlock();
    }

    public String writeContentForTest(String fieldsSection, TsFileContent tsContent) throws IOException {
        StringBuilder result = new StringBuilder();
        writeContent(fieldsSection, tsContent, new Output() {
            @Override
            public void write(String line) {
                result.append(line);
            }

            @Override
            public void close() {
            }
        });
        return result.toString();
    }
}
