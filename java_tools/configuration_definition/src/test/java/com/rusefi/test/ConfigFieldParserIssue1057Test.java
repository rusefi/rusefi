package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.output.JavaFieldsConsumer;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class ConfigFieldParserIssue1057Test {
    @Test
    public void testBitsPadding() {
        ReaderStateImpl state = new ReaderStateImpl();
        JavaFieldsConsumer javaFieldsConsumer = new TestJavaFieldsConsumer(state);

        String inputString = "struct pid_s\nbit activateAuxPid1;\n" +
                "int fieldName;\n" +
                "end_struct\n";

        state.readBufferedReader(inputString, javaFieldsConsumer);

        assertEquals("\tpublic static final Field ACTIVATEAUXPID1 = Field.create(\"ACTIVATEAUXPID1\", 0, FieldType.BIT, 0).setBaseOffset(0);\n" +
                        "\tpublic static final Field FIELDNAME = Field.create(\"FIELDNAME\", 4, FieldType.INT).setScale(1.0).setBaseOffset(0);\n",
                javaFieldsConsumer.getContent());
    }
}
