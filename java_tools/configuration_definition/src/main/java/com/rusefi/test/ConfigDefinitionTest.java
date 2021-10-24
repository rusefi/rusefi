package com.rusefi.test;

import com.rusefi.EnumsReader;
import com.rusefi.VariableRegistry;
import org.junit.Test;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import static org.junit.Assert.assertNotNull;

public class ConfigDefinitionTest {
    private static final String FIRMWARE = "../../firmware";

    @Test
    public void testEnumIntoType() throws IOException {
        EnumsReader enumsReader = new EnumsReader();
        enumsReader.read(new FileReader(FIRMWARE + File.separator + "controllers/algo/rusefi_enums.h"));

        VariableRegistry variableRegistry = new VariableRegistry();

        variableRegistry.readPrependValues(FIRMWARE + File.separator + "integration/rusefi_config.txt");


        String sb = variableRegistry.getEnumOptionsForTunerStudio(enumsReader, "engine_type_e");

        System.out.println(sb);
        assertNotNull(sb);
    }
}
