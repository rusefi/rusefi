package com.rusefi.test;

import com.rusefi.EnumGenerator;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.FileReader;
import java.nio.file.Files;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/**
 * The TS custom-type field allocates 1<<bits selectable slots, which exceeds the enum entry
 * count whenever the count is a power of two. Without padding the generated _list, TunerStudio
 * offers out-of-range values that crash the ECU. These tests
 * pin the INVALID padding produced by {@link EnumGenerator.Parser}.
 */
class EnumListPaddingTest {

    private static String listRepresentation(String name, String entriesJson) throws Exception {
        File tmp = File.createTempFile("enum_" + name, ".json");
        tmp.deleteOnExit();
        Files.writeString(tmp.toPath(),
            "{\"enum\":{\"name\":\"" + name + "\",\"type\":\"uint8_t\",\"entries\":[" + entriesJson + "]}}");

        EnumGenerator.Parser parser = new EnumGenerator.Parser(new FileReader(tmp));
        // parsedData is a static accumulator, so filter to the enum under test
        List<String> reps = parser.getDefinitions(EnumGenerator.Parser.Definitions.TsTextRepresentations);
        return reps.stream()
            .filter(s -> s.startsWith(name + "_list:"))
            .findFirst()
            .orElseThrow(() -> new AssertionError("no _list generated for " + name));
    }

    private static int countInvalid(String rep) {
        int count = 0;
        for (int i = rep.indexOf(':'); (i = rep.indexOf("\"INVALID\"", i + 1)) != -1; ) {
            count++;
        }
        return count;
    }

    @Test
    void powerOfTwoEntryCountIsPaddedToDoubleWidth() throws Exception {
        // 4 real entries -> field allocates [0:2] = 8 slots -> 4 INVALID
        String rep = listRepresentation("padPow2_e",
            "{\"i\":0,\"v\":\"A\",\"n\":\"AA\"},{\"i\":1,\"v\":\"B\",\"n\":\"BB\"}," +
            "{\"i\":2,\"v\":\"C\",\"n\":\"CC\"},{\"i\":3,\"v\":\"D\",\"n\":\"DD\"}");
        assertEquals(4, countInvalid(rep), rep);
        assertTrue(rep.endsWith("\"AA\",\"BB\",\"CC\",\"DD\",\"INVALID\",\"INVALID\",\"INVALID\",\"INVALID\""), rep);
    }

    @Test
    void nonPowerOfTwoEntryCountIsPaddedToFillWidth() throws Exception {
        // 3 real entries -> field allocates [0:1] = 4 slots -> 1 INVALID
        String rep = listRepresentation("padThree_e",
            "{\"i\":0,\"v\":\"A\",\"n\":\"AA\"},{\"i\":1,\"v\":\"B\",\"n\":\"BB\"},{\"i\":2,\"v\":\"C\",\"n\":\"CC\"}");
        assertEquals(1, countInvalid(rep), rep);
        assertTrue(rep.endsWith("\"AA\",\"BB\",\"CC\",\"INVALID\""), rep);
    }
}
