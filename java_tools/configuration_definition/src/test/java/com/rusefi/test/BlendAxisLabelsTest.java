package com.rusefi.test;

import com.opensr5.ini.ExpressionEvaluator;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
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
            String config = "boostOpenLoopBlends" + i;
            assertLabelsWithYAxis("boostOpenLoopBlend" + i + "Bias", "boostOpenBlend" + i + "Table", config, "Weight",
                    "\"RPM\", {bitStringValue(pwmAxisLabels, " + config + "_yAxisOverride ? "
                            + config + "_yAxisOverride : boostOpenLoopYAxis)}");
        }
    }

    @Test
    void closedLoopBoostBlendLabels() throws IOException {
        for (int i = 1; i <= 2; i++) {
            String config = "boostClosedLoopBlends" + i;
            assertLabelsWithYAxis("boostClosedLoopBlend" + i + "Bias", "boostClosedBlend" + i + "Table", config, "Weight",
                    "\"RPM\", {bitStringValue(pwmAxisLabels, " + config + "_yAxisOverride ? " + config
                            + "_yAxisOverride : throttlePedalPositionAdcChannel > @@adc_channel_e_EFI_ADC_NONE@@"
                            + " && throttlePedalPositionAdcChannel < @@adc_channel_e_EFI_ADC_TOTAL_CHANNELS@@"
                            + " ? @@gppwm_channel_e_GPPWM_AccelPedal@@ : @@gppwm_channel_e_GPPWM_Tps@@)}");
        }
    }

    @Test
    void closedLoopBoostLoadSourceFollowsFirmwareRouting() throws IOException {
        String template = Files.readString(Path.of(
                ConfigDefinitionTest.FIRMWARE, "tunerstudio", "tunerstudio.template.ini"));
        for (int i = 1; i <= 2; i++) {
            String label = property(template, "table", "boostClosedBlend" + i + "Table", "xyLabels")
                    .substring("\"RPM\", ".length());
            // A single string lookup avoids TS displaying an unevaluated string-valued ternary.
            String prefix = "{bitStringValue(pwmAxisLabels, ";
            assertTrue(label.startsWith(prefix) && label.endsWith(")}"), label);
            assertTrue(template.contains("\"Table load\", " + label + ","), label);
            String index = label.substring(prefix.length(), label.length() - 2)
                    .replace("@@adc_channel_e_EFI_ADC_NONE@@", "0")
                    .replace("@@adc_channel_e_EFI_ADC_TOTAL_CHANNELS@@", "49")
                    .replace("@@gppwm_channel_e_GPPWM_AccelPedal@@", "9")
                    .replace("@@gppwm_channel_e_GPPWM_Tps@@", "1");
            for (int pedal : new int[]{0, 1, 48, 49, 50}) {
                for (int override : new int[]{0, 1, 2, 9, 27}) {
                    Map<String, Double> values = Map.of(
                            "boostClosedLoopBlends" + i + "_yAxisOverride", (double) override,
                            "throttlePedalPositionAdcChannel", (double) pedal);
                    int expected = override != 0 ? override : pedal > 0 && pedal < 49 ? 9 : 1;
                    assertEquals(expected, evaluateIndex(index, values),
                            "blend=" + i + ", pedal=" + pedal + ", override=" + override);
                }
            }
        }
    }

    private static double evaluateIndex(String expression, Map<String, Double> values) {
        ExpressionEvaluator.TernaryExpression ternary = ExpressionEvaluator.parseTernary(expression);
        if (ternary != null) {
            Boolean condition = ExpressionEvaluator.evaluateBooleanExpression(ternary.condition, values);
            assertNotNull(condition, ternary.condition);
            return evaluateIndex(condition ? ternary.trueExpr : ternary.falseExpr, values);
        }
        Double result = ExpressionEvaluator.tryEvaluateWithContext(expression, values);
        assertNotNull(result, expression);
        return result;
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
