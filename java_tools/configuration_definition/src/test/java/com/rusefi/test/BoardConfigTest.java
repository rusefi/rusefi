package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.JavaFieldsConsumer;
import com.rusefi.tools.tune.BoardConfigStrategy;
import org.junit.jupiter.api.Test;

import static com.rusefi.AssertCompatibility.assertEquals;

public class BoardConfigTest {
    @Test
    public void testBoardConfig() {
        ReaderStateImpl state = new ReaderStateImpl();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);

        state.getVariableRegistry().register(BoardConfigStrategy.BOARD_CONFIG_FROM_FILE, "int fieldName2;\n");

        String inputString = "struct pid_s\n" +
            "int fieldName;\n" +
            "@@" + BoardConfigStrategy.BOARD_CONFIG_FROM_FILE + "@@\n" +
            "end_struct\n";

        state.readBufferedReader(inputString, javaFieldsConsumer);

        assertEquals("\tpublic static final Field FIELDNAME = Field.create(\"FIELDNAME\", 0, FieldType.INT).setScale(1.0).setBaseOffset(0);\n" +
                "\tpublic static final Field FIELDNAME2 = Field.create(\"FIELDNAME2\", 4, FieldType.INT).setScale(1.0).setBaseOffset(0);\n",
            javaFieldsConsumer.getContent());
    }
}
