package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.BaseCHeaderConsumer;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.output.TSProjectConsumer;
import org.junit.Test;

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
    public void conditionalField() {
        String test = "struct pid_s\n" +
                "custom afr_table_t 4x4 array,   U08,   @OFFSET@, [4x4],\"deg\",\t   {1/10},     0,        0,  25.0,     1 \n" +
                "#if LAMBDA\n" +
                "\tint periodMs2;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
                "#else\n" +
                "\tint periodMs2;PID dTime;\"ms2\",      1,      0,       0, 3000,      0\n" +
                "#endif\n" +
                "#if LAMBDA\n" +
                "afr_table_t afrTable;\t\t\n" +
                "#else\n" +
                "afr_table_t afrTable;\t\t\n" +
                "#endif\n" +
                "\tint16_t autoscale periodMs;PID dTime;\"ms\",      {1/10},      0,       0, 3000,      0\n" +
                "end_struct\n" +
                "";

        ReaderStateImpl state = new ReaderStateImpl();
        TSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer("", state);
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);


        BaseCHeaderConsumer consumer = new BaseCHeaderConsumer();

        state.readBufferedReader(test, javaFieldsConsumer, consumer, tsProjectConsumer);

        assertEquals("#if LAMBDA\n" +
                "periodMs2 = scalar, S32, 0, \"ms\", 1, 0, 0, 3000, 0\n" +
                "#else\n" +
                "periodMs2 = scalar, S32, 0, \"ms2\", 1, 0, 0, 3000, 0\n" +
                "#endif\n" +
                "#if LAMBDA\n" +
                "afrTable = array, U08, 4, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "#else\n" +
                "afrTable = array, U08, 4, [4x4],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "#endif\n" +
                "periodMs = scalar, S16, 20, \"ms\", 0.1, 0, 0, 3000, 0\n" +
                "; total TS size = 24\n", tsProjectConsumer.getContent());

        assertEquals("\tpublic static final Field PERIODMS2 = Field.create(\"PERIODMS2\", 0, FieldType.INT).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field AFRTABLE = Field.create(\"AFRTABLE\", 4, FieldType.INT).setScale(1.0).setBaseOffset(0);\n" +
                        "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 20, FieldType.INT16).setScale(0.1).setBaseOffset(0);\n" +
                        "\tpublic static final Field ALIGNMENTFILL_AT_22 = Field.create(\"ALIGNMENTFILL_AT_22\", 22, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n",
                javaFieldsConsumer.getContent());


        assertEquals("struct pid_s {\n" +
                "\t// PID dTime\n" +
                "\t// ms\n" +
                "\t// offset 0\n" +
                "\tint periodMs2 = (int)0;\n" +
                "\t// offset 4\n" +
                "\tafr_table_t afrTable;\n" +
                "\t// PID dTime\n" +
                "\t// ms\n" +
                "\t// offset 20\n" +
                "\tscaled_channel<int16_t, 10, 1> periodMs = (int16_t)0;\n" +
                "\t// offset 22\n" +
                "\tuint8_t alignmentFill_at_22[2];\n" +
                "};\n" +
                "static_assert(sizeof(pid_s) == 24);\n" +
                "\n", consumer.getContent());
    }
}
