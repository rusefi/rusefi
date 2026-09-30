package com.rusefi.test;

import com.opensr5.ini.ExpressionEvaluator;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.junit.jupiter.api.Assertions.*;

class TablePointerUnitsTest {
    private static final double KPA_TO_PSI = 0.145038;
    private static final double EPSILON = 0.000001;
    private static final Map<String, String> LOAD_BINS = Map.of(
            "veLoadBins", "veTableYAxis",
            "secondVeLoadBins", "veTableYAxis",
            "ignitionLoadBins", "loadForIgnitionTableDot",
            "secondIgnitionLoadBins", "loadForIgnitionTableDot",
            "fuelTrimLoadBins", "fuelingLoad",
            "ignTrimLoadBins", "ignitionLoad",
            "lambdaLoadBins", "afrTableYAxis",
            "lambdaMaxDeviationLoadBins", "afrTableYAxis",
            "injectorStagingLoadBins", "afrTableYAxis");

    @Test
    void imperialMapPointers() throws IOException {
        // #10338: both atmosphere and 6 psi idle pressure exceed a displayed 23 psi axis.
        for (double pressure : new double[]{100, 6 / KPA_TO_PSI}) {
            checkPointers(0, 0, 0, pressure);
            // Explicit MAP override also needs conversion with other fuel algorithms.
            checkPointers(0, 1, 1, pressure);
        }
    }

    @Test
    void metricAndNonPressurePointers() throws IOException {
        for (int metric = 0; metric <= 1; metric++) {
            for (int algorithm = 0; algorithm <= 3; algorithm++) {
                for (int override = 0; override <= 6; override++) {
                    checkPointers(metric, algorithm, override, 42);
                }
            }
        }
    }

    @Test
    void tablesWithUnconvertedBinsKeepRawChannels() throws IOException {
        String template = read("tunerstudio/tunerstudio.template.ini");
        for (String bins : new String[]{"torqueLoadBins", "hpfpTargetLoadBins"}) {
            Matcher binding = Pattern.compile("(?m)^\\h*yBins\\h*=\\h*" + bins
                    + ",\\h*(\\w+)").matcher(template);
            assertTrue(binding.find(), bins);
            assertEquals("veTableYAxis", binding.group(1), bins);
        }
    }

    private static void checkPointers(int metric, int algorithm, int override, double raw) throws IOException {
        String template = read("tunerstudio/tunerstudio.template.ini");
        String definitions = read("integration/rusefi_config.txt") + read("integration/config_page_4.txt");
        Map<String, Double> values = new HashMap<>();
        values.put("useMetricOnInterface", (double) metric);
        values.put("fuelAlgorithm", (double) algorithm);
        values.put("veOverrideMode", (double) override);
        values.put("ignOverrideMode", (double) override);
        values.put("afrOverrideMode", (double) override);
        for (String channel : LOAD_BINS.values()) {
            values.put(channel, raw);
        }

        Matcher bindings = Pattern.compile("(?m)^\\h*yBins\\h*=\\h*(\\w+),\\h*(\\w+)").matcher(template);
        int checked = 0;
        while (bindings.find()) {
            String bins = bindings.group(1);
            if (!LOAD_BINS.containsKey(bins)) {
                continue;
            }
            String context = bins + " metric=" + metric + " algorithm=" + algorithm + " override=" + override;
            Matcher scale = Pattern.compile("\\b" + bins + ";;\\{[^\\r\\n]*?},\\h*(\\{[^}]+})")
                    .matcher(definitions);
            assertTrue(scale.find(), "Missing bin scale: " + bins);
            double displayedBin = raw * evaluate(scale.group(1), values);
            String channel = bindings.group(2);
            Matcher expression = Pattern.compile("(?m)^\\h*" + channel + "\\h*=\\h*(\\{[^\\r\\n]+})")
                    .matcher(template);
            double pointer = expression.find() ? evaluate(expression.group(1), values) : values.get(channel);

            // The same physical load must occupy the same row in either unit system.
            assertEquals(displayedBin, pointer, EPSILON, context);
            if (metric == 0 && (override == 1 || (override == 0 && algorithm == 0))) {
                assertEquals(raw * KPA_TO_PSI, displayedBin, EPSILON, context);
            }
            checked++;
        }
        // Main/second VE and ignition, trim views, AFR/lambda, analyzer targets,
        // maximum lambda deviation and injector staging all use these load bins.
        assertEquals(36, checked);
    }

    private static double evaluate(String expression, Map<String, Double> values) {
        // The general numeric evaluator only accepts simple ternary conditions.
        // Use the boolean evaluator for TS conditions containing comparisons and ||.
        ExpressionEvaluator.TernaryExpression ternary = ExpressionEvaluator.parseTernary(expression);
        if (ternary != null) {
            Boolean condition = ExpressionEvaluator.evaluateBooleanExpression(ternary.condition, values);
            assertNotNull(condition, ternary.condition);
            return evaluate(condition ? ternary.trueExpr : ternary.falseExpr, values);
        }
        Double result = ExpressionEvaluator.tryEvaluateWithContext(
                expression.replace("@@UNITS_KPA_TO_PSI@@", Double.toString(KPA_TO_PSI)), values);
        assertNotNull(result, expression);
        return result;
    }

    private static String read(String file) throws IOException {
        return Files.readString(Path.of(ConfigDefinitionTest.FIRMWARE, file));
    }
}
