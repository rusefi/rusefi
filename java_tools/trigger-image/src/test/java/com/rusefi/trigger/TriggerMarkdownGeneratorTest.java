package com.rusefi.trigger;

import org.junit.jupiter.api.Test;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class TriggerMarkdownGeneratorTest {
    @Test
    public void testGenerator() throws IOException {
        String testFolder = "test_triggers";
        new File(testFolder).mkdirs();
        String triggersFile = testFolder + File.separator + "triggers.txt";
        String content = "TRIGGERTYPE 1 2 TT_TEST_A 0.00\n" +
                "knownOperationMode=true\n" +
                "#\n" +
                "event 0 0 0 0.0 0.0\n" +
                "event 0 1 0 10.0 0.0\n" +
                "TRIGGERTYPE 2 2 TT_TEST_B 0.00\n" +
                "knownOperationMode=true\n" +
                "#\n" +
                "event 0 0 0 0.0 0.0\n" +
                "event 0 1 0 10.0 0.0\n" +
                "TRIGGERTYPE 3 1 TT_TEST_C 0.00\n" +
                "knownOperationMode=true\n" +
                "#\n" +
                "event 0 0 0 0.0 0.0\n";
        Files.write(Paths.get(triggersFile), content.getBytes());

        String outputFile = "test_report.md";
        TriggerMarkdownGenerator.generate(testFolder, outputFile);

        assertTrue(new File(outputFile).exists());
        List<String> lines = Files.readAllLines(Paths.get(outputFile));

        // Check grouping and sorting
        assertTrue(lines.contains("## tooth count 0"));
        assertTrue(lines.contains("- [TEST_C](#TEST_C)"));
        assertFalse(lines.contains("- [TT_TEST_C](#TT_TEST_C)"));
        assertTrue(lines.contains("## tooth count 1"));
        assertTrue(lines.contains("- [TEST_A](#TEST_A)"));
        assertTrue(lines.contains("- [TEST_B](#TEST_B)"));

        // Clean up
        new File(triggersFile).delete();
        new File(testFolder).delete();
        new File(outputFile).delete();
    }
}
