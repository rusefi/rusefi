package com.rusefi.test;

import com.rusefi.EnumsReader;
import com.rusefi.VariableRegistry;
import org.jetbrains.annotations.NotNull;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import static com.rusefi.AssertCompatibility.assertNotNull;
import static com.rusefi.AssertCompatibility.assertTrue;

public class ConfigDefinitionTest {
    public static final String FIRMWARE = "../../firmware";

    @Test
    public void testEnumIntoType() throws IOException {
        EnumsReader enumsReader = new EnumsReader();
        enumsReader.read(new FileReader(FIRMWARE + File.separator + "controllers/algo/engine_types.h"));

        VariableRegistry variableRegistry = readRealConfig();


        String sb = variableRegistry.getEnumOptionsForTunerStudio(enumsReader, "engine_type_e");

        System.out.println(sb);
        assertNotNull(sb);
        assertTrue("Seems too long" + sb, sb.length() < 100000);
    }

    @NotNull
    public static VariableRegistry readRealConfig() {
        VariableRegistry variableRegistry = new VariableRegistry();

        variableRegistry.readPrependValues(FIRMWARE + File.separator + "integration/rusefi_config.txt", true);
        return variableRegistry;
    }
}
