package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.VariableRegistry;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.tools.tune.BoardConfigStrategy;
import org.junit.jupiter.api.Test;

import static com.rusefi.AssertCompatibility.assertEquals;

public class BoardConfigTest {
    @Test
    public void testBoardConfig() {
        ReaderStateImpl state = new ReaderStateImpl();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);

        state.getVariableRegistry().register(BoardConfigStrategy.BOARD_CONFIG_FROM_FILE, "struct pid_s\n" +
            "  float pFactor;;\"\", 1, 0, -10000, 10000, 4\n" +
            "\t \tint16_t maxValue;Output Max Duty Cycle;\"\", 1, 0, -30000, 30000, 0\n" +
            "end_struct\n" +
            "int fieldName2;\n");

        String inputString = "struct parent\n" +
            "int fieldName;\n" +
            ReaderStateImpl.SPLIT_LINES + "@@" + BoardConfigStrategy.BOARD_CONFIG_FROM_FILE + "@@\n" +
            "end_struct\n";

        state.readBufferedReader(inputString, javaFieldsConsumer);

        assertEquals("\tpublic static final Field FIELDNAME = Field.create(\"FIELDNAME\", 0, FieldType.INT).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field FIELDNAME2 = Field.create(\"FIELDNAME2\", 4, FieldType.INT).setScale(1.0).setBaseOffset(0);\n",
            javaFieldsConsumer.getContent());
    }

    @Test
    public void testBoardConfigWithCustomType() {
        ReaderStateImpl state = new ReaderStateImpl();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);

        state.getVariableRegistry().register(BoardConfigStrategy.BOARD_CONFIG_FROM_FILE, "custom string_here_t 64 string, ASCII, @OFFSET@, 64\n" +
            "string_here_t fieldName2;\n");

        String inputString = "struct parent\n" +
            ReaderStateImpl.SPLIT_LINES + VariableRegistry.TEMPLATE_TAG + BoardConfigStrategy.BOARD_CONFIG_FROM_FILE + VariableRegistry.TEMPLATE_TAG +
            "\n" +
            "int fieldName;\n" +
            "end_struct\n";

        state.readBufferedReader(inputString, javaFieldsConsumer);

        assertEquals("\tpublic static final Field FIELDNAME2 = Field.create(\"FIELDNAME2\", 0, 64, FieldType.STRING).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field FIELDNAME = Field.create(\"FIELDNAME\", 64, FieldType.INT).setScale(1.0).setBaseOffset(0);\n",
            javaFieldsConsumer.getContent());
    }}
