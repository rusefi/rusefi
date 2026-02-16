package com.rusefi.test;

import com.rusefi.BitState;
import com.rusefi.ReaderStateImpl;
import com.rusefi.TsFileContent;
import com.rusefi.output.TSProjectConsumer;
import com.rusefi.util.Output;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.function.Executable;

import java.io.*;

import static com.rusefi.AssertCompatibility.assertThrows;
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

    @Test
    public void test35Bits() {
        assertThrows(BitState.TooManyBitsInARow.class, new Executable() {
            @Override
            public void execute() throws Throwable {
                ReaderStateImpl state = new ReaderStateImpl();

                StringBuilder input = new StringBuilder("struct bit_s\n");
                for (int i = 0; i < 35; i++) {
                    input.append(ReaderStateImpl.BIT).append(" fieldName").append(i).append(",\"true\",\"false\"\n");
                }
                input.append("end_struct\n");

                StringWriter sw = new StringWriter();

                TSProjectConsumer javaFieldsConsumer = new TSProjectConsumer("", state) {
                    @Override
                    protected void writeTunerStudioFile(String tsPath, String fieldsSection) throws IOException {
                        writeContent(fieldsSection, new TsFileContent("", ""), createOutput(sw));
                    }
                };
                state.setHeaderMessage("test");
                state.readBufferedReader(input.toString(), javaFieldsConsumer);

                String result = sw.toString();
                System.out.println(result);

                assertTrue(result.contains("fieldName0 = bits, U32, 0, [0:0], \"false\", \"true\""));
                assertTrue(result.contains("fieldName31 = bits, U32, 0, [31:31], \"false\", \"true\""));
                assertTrue(result.contains("fieldName32 = bits, U32, 4, [0:0], \"false\", \"true\""));
                assertTrue(result.contains("fieldName34 = bits, U32, 4, [2:2], \"false\", \"true\""));
            }
        });
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
