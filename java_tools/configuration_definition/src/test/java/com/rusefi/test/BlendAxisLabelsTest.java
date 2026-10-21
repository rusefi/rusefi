package com.rusefi.test;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class BlendAxisLabelsTest {
    @Test
    void ignitionBlendLabels() throws IOException {
        for (int i = 1; i <= 4; i++) {
            String config = "ignBlends" + i;
            assertLabelsWithYAxis("ignAdder" + i + "Bias", "ignAdder" + i + "Table", config, "Weight",
                    "\"RPM\", {" + config + "_yAxisOverride ? bitStringValue(pwmAxisLabels, " + config
                            + "_yAxisOverride) : bitStringValue(ignBlendLoadLabels, ignLoadSrc)}");
        }
    }

    @Test
    void openLoopBoostBlendLabels() throws IOException {
        for (int i = 1; i <= 2; i++) {
            assertLabels("boostOpenLoopBlend" + i + "Bias", "boostOpenBlend" + i + "Table",
                    "boostOpenLoopBlends" + i, "boostOpenLoopYAxis");
        }
    }

    @Test
    void closedLoopBoostBlendLabels() throws IOException {
        for (int i = 1; i <= 2; i++) {
            assertLabels("boostClosedLoopBlend" + i + "Bias", "boostClosedBlend" + i + "Table",
                    "boostClosedLoopBlends" + i, "1");
        }
    }

    private static void assertLabels(String curve, String table, String config, String defaultAxis)
            throws IOException {
        assertLabelsWithYAxis(curve, table, config, "bias",
                "\"RPM\", {bitStringValue(pwmAxisLabels, " + config + "_yAxisOverride ? "
                        + config + "_yAxisOverride : " + defaultAxis + ")}");
    }

    private static void assertLabelsWithYAxis(String curve, String table, String config, String weightLabel,
                                             String yAxisLabel) throws IOException {
        String template = Files.readString(Path.of(
                ConfigDefinitionTest.FIRMWARE, "tunerstudio", "tunerstudio.template.ini"));
        // Each label must follow its own selector, including the no-override fallback.
        assertEquals("{bitStringValue(pwmAxisLabels, " + config + "_blendParameter)}, \"" + weightLabel + "\"",
                property(template, "curve", curve, "columnLabel"), curve);
        assertEquals(yAxisLabel, property(template, "table", table, "xyLabels"), table);
        // Check the associated bins too, so each label is tied to the right blend instance.
        assertTrue(property(template, "curve", curve, "xBins").startsWith(config + "_blendBins,"), curve);
        assertTrue(property(template, "table", table, "yBins").startsWith(config + "_loadBins,"), table);
        assertEquals(config + "_table", property(template, "table", table, "zBins"), table);
    }

    private static String property(String template, String kind, String name, String property) {
        Matcher block = Pattern.compile("(?ms)^\\h*" + kind + "\\h*=\\h*" + Pattern.quote(name)
                + ",[^\\r\\n]*\\R(.*?)(?=^\\h*(?:curve|table)\\h*=|^\\[|\\z)").matcher(template);
        assertTrue(block.find(), "Missing " + kind + " " + name);
        Matcher value = Pattern.compile("(?m)^\\h*" + property + "\\h*=\\h*([^\\r\\n]+)").matcher(block.group(1));
        assertTrue(value.find(), "Missing " + property + " in " + name);
        return value.group(1).trim();
    }
}
