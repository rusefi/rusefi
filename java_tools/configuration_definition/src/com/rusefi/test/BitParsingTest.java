package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.TsFileContent;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.output.TSProjectConsumer;
import com.rusefi.util.Output;
import org.junit.Test;

import java.io.*;
import java.util.Arrays;

public class BitParsingTest {
    @Test
    public void testBitParser() throws IOException {
        ReaderState state = new ReaderState();

        String inputString = "struct pid_s\nbit activateAuxPid1;\n" +
                "bit fieldName;\n" +
                "end_struct\n";
        BufferedReader reader = new BufferedReader(new StringReader(inputString));

        StringWriter sw = new StringWriter();

        TSProjectConsumer javaFieldsConsumer = new TSProjectConsumer(new CharArrayWriter(), "", state) {
            @Override
            protected void writeTunerStudioFile(String tsPath, String fieldsSection) throws IOException {
                writeContent(fieldsSection, new TsFileContent("", ""), createOutput(sw));
            }
        };
        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));

        System.out.printf("start[" + sw.toString() + "]end");

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
