package com.rusefi.test;

import com.rusefi.VariableRegistry;
import com.rusefi.output.HashUtil;
import com.rusefi.output.SharedStringHashGenerator;
import org.junit.jupiter.api.Test;

import java.io.StringReader;

import static com.rusefi.AssertCompatibility.assertTrue;
import static com.rusefi.AssertCompatibility.assertEquals;

public class SharedStringHashGeneratorTest {
    @Test
    public void testGenerateContent() throws Exception {
        VariableRegistry vr = new VariableRegistry();
        StringReader sr = new StringReader(
                "#define FIRMWARE_NAME \"Hello1\"\n" +
                "#define BUILD_NUMBER 123\n" +
                "#define PROJECT_NAME \"HeLLo2\"\n"
        );
        vr.readPrependValues(sr, false);

        String content = SharedStringHashGenerator.generateContent(vr);

        int h1 = HashUtil.hash("Hello1");
        int h2 = HashUtil.hash("HeLLo2");

        assertTrue(content.contains("// Hello1"));
        assertTrue(content.contains("#define FIRMWARE_NAME_HASH " + h1));

        assertTrue(content.contains("// HeLLo2"));
        assertTrue(content.contains("#define PROJECT_NAME_HASH " + h2));

        // ensure numeric define was ignored
        assertEquals(-1, content.indexOf("BUILD_NUMBER_HASH"));
    }
}
