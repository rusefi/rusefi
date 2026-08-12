package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.SizeMismatchException;
import com.rusefi.output.JavaFieldsConsumer;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.function.Executable;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class CustomFieldTest {
    @Test
    public void testCustomEnumAlignment() {
        assertThrows(SizeMismatchException.class, new Executable() {
            @Override
            public void execute() throws Throwable {
                String test = "struct pid_s\n" +
                    "#define can_wbo_type_e_enum \"RusEFI\", \"AEM X-series\", \"Disabled/Analog\"\n" +
                    "\tcustom can_wbo_type_e 2 bits, U08, @OFFSET@, [0:1], @@can_wbo_type_e_enum@@\n" +
                    "\tcan_wbo_type_e wboType1;\n" +
                    "end_struct\n";
                ReaderStateImpl state = new ReaderStateImpl();

                TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
                state.readBufferedReader(test, tsProjectConsumer);
            }
        });
    }

    @Test
    public void testCustomEnumWithTsVariable() {
        String test = "struct pid_s\n" +
            "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
            "custom ego_sensor_e 1 bits, S08, @OFFSET@, [0:1], $ego_sensor_e_list\n" +
            "ego_sensor_e afr_type1;\n" +
            "int16_t int\n" +
            "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type1 = bits, S08, 0, [0:1], $ego_sensor_e_list\n" +
            "int = scalar, S16, 2, \"\", 1, 0, 0, 32000, 0\n" +
            "; total TS size = 4\n", tsProjectConsumer.getContent());
    }

    @Test
    public void testShortForm() {
        String test = "struct pid_s\n" +
            "#define ego_sensor_e_enum \"BPSX\"\n" +
            "custom ego_sensor_e 1 bits, S08, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
            "ego_sensor_e afr_type1;\n" +
            "ego_sensor_e afr_type2;\n" +
            "int8_t int\n" +
            "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("afr_type1 = bits, S08, 0, [0:1], \"BPSX\"\n" +
            "afr_type2 = bits, S08, 1, [0:1], \"BPSX\"\n" +
            "int = scalar, S08, 2, \"\", 1, 0, 0, 100, 0\n" +
            "; total TS size = 4\n", tsProjectConsumer.getContent());
    }

    @Test
    public void useCustomType() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test = "struct pid_s\n" +
            "#define ERROR_BUFFER_SIZE 120\n" +
            "\tcustom critical_error_message_t @@ERROR_BUFFER_SIZE@@ string, ASCII, @OFFSET@, @@ERROR_BUFFER_SIZE@@\n" +
            "\tcritical_error_message_t var;\n" +
            "\tint16_t periodMs;PID dTime;\"ms\",      1,      0,       0, 3000,      0\n" +
            "end_struct\n" +
            "";

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, javaFieldsConsumer);

        assertEquals("\tpublic static final Field VAR = Field.create(\"VAR\", 0, 120, FieldType.STRING).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field PERIODMS = Field.create(\"PERIODMS\", 120, FieldType.INT16).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field ALIGNMENTFILL_AT_122 = Field.create(\"ALIGNMENTFILL_AT_122\", 122, FieldType.INT8).setScale(1.0).setBaseOffset(0);\n",
            javaFieldsConsumer.getContent());
    }

    @Test
    public void testCustomTypeArray() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test = "struct pid_s\n" +
            "#define ERROR_BUFFER_SIZE 120\n" +
            "\tcustom critical_error_message_t @@ERROR_BUFFER_SIZE@@ string, ASCII, @OFFSET@, @@ERROR_BUFFER_SIZE@@\n" +
            "\tcritical_error_message_t[2 iterate] var;\n" +
            "end_struct\n" +
            "";

        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);
        state.readBufferedReader(test, javaFieldsConsumer);

        assertEquals("\tpublic static final Field VAR1 = Field.create(\"VAR1\", 0, 120, FieldType.STRING).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field VAR2 = Field.create(\"VAR2\", 120, 120, FieldType.STRING).setScale(1.0).setBaseOffset(0);\n",
            javaFieldsConsumer.getContent());
    }

    @Test
    public void testCustomTypeArrayTsWithSpace() {
        ReaderStateImpl state = new ReaderStateImpl();
        String test = "struct pid_s\n" +
            "#define ERROR_BUFFER_SIZE 120\n" +
            "\tcustom critical_error_message_t @@ERROR_BUFFER_SIZE@@ string, ASCII, @OFFSET@, @@ERROR_BUFFER_SIZE@@\n" +
            "\tcritical_error_message_t [2 iterate] var;\n" +
            "end_struct\n" +
            "";

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);

        assertEquals("var1 = string, ASCII, 0, 120\n" +
            "var2 = string, ASCII, 120, 120\n" +
            "; total TS size = 240\n", tsProjectConsumer.getContent());
    }

    @Test
    public void test2byteCustomEnum() {
        String test = "struct pid_s\n" +
            "#define ego_sensor_e_enum \"BPSX\", \"Innovate\", \"14Point7\"\n" +
            "custom ego_sensor_e2 2 bits, S16, @OFFSET@, [0:1], @@ego_sensor_e_enum@@\n" +
            "int8_t int\n" +
            "ego_sensor_e2 afr_type1;\n" +
            "ego_sensor_e2 afr_type2;\n" +
            "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();

        TestTSProjectConsumer tsProjectConsumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(test, tsProjectConsumer);
        assertEquals("int = scalar, S08, 0, \"\", 1, 0, 0, 100, 0\n" +
            "afr_type1 = bits, S16, 2, [0:1], \"BPSX\", \"Innovate\", \"14Point7\", \"INVALID\"\n" +
            "afr_type2 = bits, S16, 4, [0:1], \"BPSX\", \"Innovate\", \"14Point7\", \"INVALID\"\n" +
            "; total TS size = 8\n", tsProjectConsumer.getContent());
    }
}
