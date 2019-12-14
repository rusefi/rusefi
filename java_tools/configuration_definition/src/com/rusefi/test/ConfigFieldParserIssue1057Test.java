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
        JavaFieldsConsumer javaFieldsConsumer = new JavaFieldsConsumer(state) {
            @Override
            public void startFile() {
            }

            @Override
            public void endFile() {
            }
        };

        String inputString = "struct pid_s\nbit activateAuxPid1;\n" +
                "int fieldName;\n" +
                "end_struct\n";
        BufferedReader reader = new BufferedReader(new StringReader(inputString));


        state.readBufferedReader(reader, Arrays.asList(javaFieldsConsumer));

        assertEquals("\tpublic static final Field ACTIVATEAUXPID1 = Field.create(\"ACTIVATEAUXPID1\", 0, FieldType.BIT, 0);\n" +
                        "\tpublic static final Field FIELDNAME = Field.create(\"FIELDNAME\", 4, FieldType.INT);\n",
                javaFieldsConsumer.getJavaFieldsWriter());
    }
}
