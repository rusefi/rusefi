package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.TsFileContent;
import com.rusefi.output.TSProjectConsumer;
import com.rusefi.util.Output;
import org.junit.jupiter.api.Test;

import java.io.*;

import static com.rusefi.AssertCompatibility.assertTrue;

public class BitParsingTest {
    @Test
    public void testBitParser() {
        ReaderStateImpl state = new ReaderStateImpl();

        String inputString = "struct pid_s\n" +
                ReaderStateImpl.BIT + " fieldName,\"true\",\"false\"\n" +
                ReaderStateImpl.BIT + " fieldName2,\"si\",\"nada\";comment\n" +
                "end_struct\n";

        StringWriter sw = new StringWriter();

        TSProjectConsumer javaFieldsConsumer = new TSProjectConsumer("", state) {
            @Override
            protected void writeTunerStudioFile(String tsPath, String fieldsSection) throws IOException {
                writeContent(fieldsSection, new TsFileContent("", ""), createOutput(sw));
            }
        };
        state.setHeaderMessage("test");
        state.readBufferedReader(inputString, javaFieldsConsumer);

        System.out.printf("start[" + sw + "]end");

        assertTrue(sw.toString().contains("\"false\", \"true\""));
        assertTrue(sw.toString().contains("\"nada\", \"si\""));
    }

    private Output createOutput(StringWriter sw) {
        return new Output() {
            @Override
            public void write(String line) {
                sw.write(line);
            }

            @Override
            public void close() {

            }
        };
    }
}
