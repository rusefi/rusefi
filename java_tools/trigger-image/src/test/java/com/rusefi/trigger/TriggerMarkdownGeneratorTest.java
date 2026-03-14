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
        String triggersFile = testFolder + File.separator + "triggers.yaml";
        String content = "- name: TT_TEST_A\n" +
            "  operationMode: FOUR_STROKE_CAM_SENSOR\n" +
            "  syncEdge: RiseOnly\n" +
            "  syncGaps:\n" +
            "  - from: 1.5\n" +
            "    to: 2.5\n" +
            "  teeth:\n" +
            "  - channel: 0\n" +
            "    angle: 0\n" +
            "  - channel: 1\n" +
            "    angle: 20\n" +
            "- name: TT_TEST_B\n" +
            "  operationMode: FOUR_STROKE_CAM_SENSOR\n" +
            "  syncEdge: RiseOnly\n" +
            "  syncGaps:\n" +
            "  - from: 1.5\n" +
            "    to: 2.5\n" +
            "  teeth:\n" +
            "  - channel: 0\n" +
            "    angle: 0\n" +
            "  - channel: 1\n" +
            "    angle: 20\n" +
            "- name: TT_TEST_C\n" +
            "  operationMode: FOUR_STROKE_CAM_SENSOR\n" +
            "  syncEdge: RiseOnly\n" +
            "  syncGaps:\n" +
            "  - from: 1.5\n" +
            "    to: 2.5\n" +
            "  teeth:\n" +
            "  - channel: 0\n" +
            "    angle: 0\n";
 
        Files.write(Paths.get(triggersFile), content.getBytes());

        String outputFile = "test_report.md";
        TriggerMarkdownGenerator.generate(triggersFile, outputFile);

        assertTrue(new File(outputFile).exists());
        List<String> lines = Files.readAllLines(Paths.get(outputFile));

        // Check grouping and sorting
        assertTrue(lines.contains("## tooth count 1"));
        assertTrue(lines.contains("- [TEST_C](#TEST_C)"));
        assertFalse(lines.contains("- [TT_TEST_C](#TT_TEST_C)"));
        assertTrue(lines.contains("## tooth count 2"));
        assertTrue(lines.contains("- [TEST_A](#TEST_A)"));
        assertTrue(lines.contains("- [TEST_B](#TEST_B)"));

        // Clean up
        new File(triggersFile).delete();
        new File(testFolder).delete();
        new File(outputFile).delete();
    }
}
