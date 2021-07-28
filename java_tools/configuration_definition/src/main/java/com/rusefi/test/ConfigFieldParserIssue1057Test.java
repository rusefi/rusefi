package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.JavaFieldsConsumer;
import org.junit.Test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.Arrays;

import static org.junit.Assert.assertEquals;

public class ConfigFieldParserIssue1057Test {
    @Test
    public void testBitsPadding() throws IOException {
        ReaderState state = new ReaderState();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);

        String inputString = "struct pid_s\nbit activateAuxPid1;\n" +
                "int fieldName;\n" +
                "end_struct\n";
        BufferedReader reader = new BufferedReader(new StringReader(inputString));


        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));

        assertEquals("\tpublic static final Field ACTIVATEAUXPID1 = Field.create(\"ACTIVATEAUXPID1\", 0, FieldType.BIT, 0);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_1 = Field.create(\"UNUSEDBIT_1_1\", 0, FieldType.BIT, 1);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_2 = Field.create(\"UNUSEDBIT_1_2\", 0, FieldType.BIT, 2);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_3 = Field.create(\"UNUSEDBIT_1_3\", 0, FieldType.BIT, 3);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_4 = Field.create(\"UNUSEDBIT_1_4\", 0, FieldType.BIT, 4);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_5 = Field.create(\"UNUSEDBIT_1_5\", 0, FieldType.BIT, 5);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_6 = Field.create(\"UNUSEDBIT_1_6\", 0, FieldType.BIT, 6);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_7 = Field.create(\"UNUSEDBIT_1_7\", 0, FieldType.BIT, 7);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_8 = Field.create(\"UNUSEDBIT_1_8\", 0, FieldType.BIT, 8);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_9 = Field.create(\"UNUSEDBIT_1_9\", 0, FieldType.BIT, 9);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_10 = Field.create(\"UNUSEDBIT_1_10\", 0, FieldType.BIT, 10);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_11 = Field.create(\"UNUSEDBIT_1_11\", 0, FieldType.BIT, 11);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_12 = Field.create(\"UNUSEDBIT_1_12\", 0, FieldType.BIT, 12);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_13 = Field.create(\"UNUSEDBIT_1_13\", 0, FieldType.BIT, 13);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_14 = Field.create(\"UNUSEDBIT_1_14\", 0, FieldType.BIT, 14);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_15 = Field.create(\"UNUSEDBIT_1_15\", 0, FieldType.BIT, 15);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_16 = Field.create(\"UNUSEDBIT_1_16\", 0, FieldType.BIT, 16);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_17 = Field.create(\"UNUSEDBIT_1_17\", 0, FieldType.BIT, 17);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_18 = Field.create(\"UNUSEDBIT_1_18\", 0, FieldType.BIT, 18);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_19 = Field.create(\"UNUSEDBIT_1_19\", 0, FieldType.BIT, 19);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_20 = Field.create(\"UNUSEDBIT_1_20\", 0, FieldType.BIT, 20);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_21 = Field.create(\"UNUSEDBIT_1_21\", 0, FieldType.BIT, 21);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_22 = Field.create(\"UNUSEDBIT_1_22\", 0, FieldType.BIT, 22);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_23 = Field.create(\"UNUSEDBIT_1_23\", 0, FieldType.BIT, 23);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_24 = Field.create(\"UNUSEDBIT_1_24\", 0, FieldType.BIT, 24);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_25 = Field.create(\"UNUSEDBIT_1_25\", 0, FieldType.BIT, 25);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_26 = Field.create(\"UNUSEDBIT_1_26\", 0, FieldType.BIT, 26);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_27 = Field.create(\"UNUSEDBIT_1_27\", 0, FieldType.BIT, 27);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_28 = Field.create(\"UNUSEDBIT_1_28\", 0, FieldType.BIT, 28);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_29 = Field.create(\"UNUSEDBIT_1_29\", 0, FieldType.BIT, 29);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_30 = Field.create(\"UNUSEDBIT_1_30\", 0, FieldType.BIT, 30);\n" +
                        "\tpublic static final Field UNUSEDBIT_1_31 = Field.create(\"UNUSEDBIT_1_31\", 0, FieldType.BIT, 31);\n" +
                        "\tpublic static final Field FIELDNAME = Field.create(\"FIELDNAME\", 4, FieldType.INT);\n",
                javaFieldsConsumer.getJavaFieldsWriter());
    }
}
