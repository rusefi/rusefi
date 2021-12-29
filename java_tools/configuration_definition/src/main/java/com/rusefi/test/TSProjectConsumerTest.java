package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.BaseCHeaderConsumer;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.output.TSProjectConsumer;
import org.junit.Test;

import java.io.CharArrayWriter;
import java.io.IOException;
import java.util.Arrays;

import static org.junit.Assert.assertEquals;

public class TSProjectConsumerTest {
    @Test
    public void getTsCondition() {
        assertEquals("ts", TSProjectConsumer.getToken("\"HIP9011 Settings (knock sensor) (alpha version)\"  @@if_ts\r\n"));
        assertEquals("ts_show_hip9011", TSProjectConsumer.getToken("\"HIP9011 Settings (knock sensor) (alpha version)\"  @@if_ts_show_hip9011"));

        assertEquals("1", TSProjectConsumer.removeToken("1@@if_ts"));
        assertEquals("12", TSProjectConsumer.removeToken("1@@if_ts 2"));
        assertEquals("H2\r\n", TSProjectConsumer.removeToken("H@@if_ts 2\r\n"));
    }

    @Test
    public void conditionalField() throws IOException {
        String test = "struct pid_s\n" +
                "custom afr_table_t 4x4 array,   U08,   @OFFSET@, [4x4],\"deg\",\t   {1/10},     0,        0,  25.0,     1 \n" +
                "#if LAMBDA\n" +
                "\tint16_t periodMs2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "#else\n" +
                "\tint16_t periodMs2;PID dTime;\"ms2\",      1,      0,       0, 3000,      0\n" +
                "#endif\n" +
                "#if LAMBDA\n" +
                "afr_table_t afrTable;\t\t\n" +
                "#else\n" +
                "afr_table_t afrTable;\t\t\n" +
                "#endif\n" +
                "\tint16_t periodMs;PID dTime;\"ms\",      {1/10},      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "";

        CharArrayWriter writer = new CharArrayWriter();
        ReaderState state = new ReaderState();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(writer, "", state);
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);


        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer() {
            @Override
            public void endFile() {
            }
        };

        state.readBufferedReader(test, Arrays.asList(javaFieldsConsumer, consumer, tsProjectConsumer));

        assertEquals("#if LAMBDA\n" +
                "periodMs2 = scalar, S16, 0, \"ms\", 1, 0, 0, 3000, 0\n" +
                "#else\n" +
                "periodMs2 = scalar, S16, 0, \"ms2\", 1, 0, 0, 3000, 0\n" +
                "#endif\n" +
                "#if LAMBDA\n" +
                "afrTable = array, U08, 2, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "#else\n" +
                "afrTable = array, U08, 2, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "#endif\n" +
                "periodMs = scalar, S16, 18, \"ms\", 0.1, 0, 0, 3000, 0\n" +
                "; total TS size = 20\n", new String(writer.toCharArray()));

        assertEquals("\tpublic static final Field PERIODMS2 = Field.create(\"PERIODMS2\", 0, FieldType.INT16);\n" +
                        "\tpublic static final Field AFRTABLE = Field.create(\"AFRTABLE\", 2, FieldType.INT);\n" +
                        "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 18, FieldType.INT16);\n",
                javaFieldsConsumer.getJavaFieldsWriter());


        assertEquals("// start of pid_s\n" +
                "struct pid_s {\n" +
                "\t/**\n" +
                "\t * PID dTime\n" +
                "\tms\n" +
                "\t * offset 0\n" +
                "\t */\n" +
                "\tint16_t periodMs2 = (int16_t)0;\n" +
                "\t/**\n" +
                "\t * offset 2\n" +
                "\t */\n" +
                "\tafr_table_t afrTable;\n" +
                "\t/**\n" +
                "\t * PID dTime\n" +
                "\tms\n" +
                "\t * offset 18\n" +
                "\t */\n" +
                "\tint16_t periodMs = (int16_t)0;\n" +
                "\t/** total size 20*/\n" +
                "};\n" +
                "\n", consumer.getContent().toString());
    }
}
