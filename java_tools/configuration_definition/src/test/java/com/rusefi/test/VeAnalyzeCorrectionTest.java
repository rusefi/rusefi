package com.rusefi.test;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class VeAnalyzeCorrectionTest {
    private static final double EPSILON = 0.0001;
    private static final Pattern STFT_FIELD = Pattern.compile(
            "(?m)^\\s*float\\[FT_BANK_COUNT iterate]\\s+stftCorrection;[^\\r\\n]*?\"%\",\\s*" +
                    "([-+]?\\d+(?:\\.\\d+)?),\\s*([-+]?\\d+(?:\\.\\d+)?)");
    private static final Pattern VE_ANALYZE_CORRECTION = Pattern.compile(
            "(?m)^\\s*egoCorrectionForVeAnalyze\\s*=\\s*\\{\\s*Gego\\s*}\\s*$");

    @Test
    void stftDisplayUsesZeroAsNeutral() throws IOException {
        String engineState = Files.readString(Path.of(
                ConfigDefinitionTest.FIRMWARE, "controllers", "algo", "engine_state.txt"));
        Matcher matcher = STFT_FIELD.matcher(engineState);

        assertTrue(matcher.find(), "Missing stftCorrection live-data field");

        double scale = Double.parseDouble(matcher.group(1));
        double translate = Double.parseDouble(matcher.group(2));

        assertEquals(-10.0, toTunerStudioValue(0.9, scale, translate), EPSILON);
        assertEquals(0.0, toTunerStudioValue(1.0, scale, translate), EPSILON);
        assertEquals(10.0, toTunerStudioValue(1.1, scale, translate), EPSILON);
    }

    @Test
    void veAnalyzeUsesHundredBasedChannelDirectly() throws IOException {
        String template = Files.readString(Path.of(
                ConfigDefinitionTest.FIRMWARE, "tunerstudio", "tunerstudio.template.ini"));

        assertTrue(VE_ANALYZE_CORRECTION.matcher(template).find(),
                "VE Analyze must use the 100-based Gego channel, independently of the STFT display scale");
    }

    private static double toTunerStudioValue(double rawValue, double scale, double translate) {
        return (rawValue + translate) * scale;
    }
}
