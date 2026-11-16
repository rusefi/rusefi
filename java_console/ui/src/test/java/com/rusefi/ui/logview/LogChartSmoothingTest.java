package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JMenu;
import javax.swing.JRadioButtonMenuItem;
import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LogChartSmoothingTest {
    private static CsvLogReader log() throws IOException {
        return CsvLogReader.read(new StringReader(
                "Time\tSignal\tOther\n"
                        + "s\tV\tV\n"
                        + "0\t0\t0\n"
                        + "0.01\t10\t10\n"
                        + "0.09\t90\t90\n"
                        + "0.10\t100\t100\n"));
    }

    private static LogChart chart(CsvLogReader log, double windowSeconds) {
        ChartSettings settings = new ChartSettings();
        settings.setSmoothingWindowSeconds(windowSeconds);
        PlotLayout layout = new PlotLayout(log.fieldCount());
        layout.add(1);
        LogChart chart = new LogChart(log, layout, settings);
        chart.setSize(600, 400);
        chart.paint(new BufferedImage(600, 400, BufferedImage.TYPE_INT_RGB).createGraphics());
        return chart;
    }

    @Test
    void smoothingChangesOnlyThePaintedTrace() throws IOException {
        CsvLogReader log = log();
        LogChart chart = chart(log, 0.10);

        assertEquals(5, chart.traceValue(0, 1), 1e-12);
        assertEquals(5, chart.traceValue(1, 1), 1e-12);
        assertEquals(95, chart.traceValue(2, 1), 1e-12);
        assertEquals(95, chart.traceValue(3, 1), 1e-12);

        assertEquals(0, log.value(0, 1), "the recorded sample stays raw");
        assertEquals(10, log.value(1, 1), "the recorded sample stays raw");
        assertEquals("10", chart.legendValue(1, 1), "cursor values stay raw");
        assertArrayEquals(new double[]{0, 100}, chart.range(1), 1e-12, "the y scale stays raw");
        assertEquals(0, chart.visibleStats().get(1).min, "statistics stay raw");
        assertEquals(100, chart.visibleStats().get(1).max, "statistics stay raw");
    }

    @Test
    void tracePathUsesTheSmoothedValues() throws IOException {
        LogChart chart = chart(log(), 0.10);
        Rectangle area = chart.bands().get(0).traces;
        Rectangle2D path = chart.tracePath(1).getBounds2D();

        assertTrue(path.getMinY() > area.y, "smoothed maximum no longer reaches the raw 100 V edge");
        assertTrue(path.getMaxY() < area.y + area.height, "smoothed minimum no longer reaches the raw 0 V edge");
    }

    @Test
    void rawSettingDrawsRecordedSamples() throws IOException {
        CsvLogReader log = log();
        LogChart chart = chart(log, 0);

        for (int sample = 0; sample < log.sampleCount(); sample++) {
            assertEquals(log.value(sample, 1), chart.traceValue(sample, 1));
        }
    }

    @Test
    void legendMenuOverridesOneSignalAndCanRestoreTheGlobalLevel() throws IOException {
        CsvLogReader log = log();
        ChartSettings settings = new ChartSettings();
        settings.setSmoothingWindowSeconds(0.10);
        PlotLayout layout = new PlotLayout(log.fieldCount());
        layout.add(1);
        layout.add(2);
        LogChart chart = new LogChart(log, layout, settings);
        chart.smoothingWindowPrompt = (signal, initial, preview) -> {
            preview.accept(10);
            return 10;
        };

        JMenu smoothing = chart.smoothingSubmenu(1);
        assertTrue(item(smoothing, "Use global (100 ms)").isSelected());
        item(smoothing, "Custom\u2026").doClick();
        assertEquals(0.01, settings.getSmoothingWindowSeconds("Signal"), 1e-12);
        assertEquals(0, chart.traceValue(0, 1), 1e-12, "Signal uses its 10 ms override");
        assertEquals(5, chart.traceValue(0, 2), 1e-12, "Other still uses the 100 ms global level");

        smoothing = chart.smoothingSubmenu(1);
        assertTrue(item(smoothing, "Custom (10 ms)\u2026").isSelected());
        chart.smoothingWindowPrompt = (signal, initial, preview) -> {
            preview.accept(50);
            return null;
        };
        item(smoothing, "Custom (10 ms)\u2026").doClick();
        assertEquals(0.01, settings.getSmoothingWindowSeconds("Signal"), 1e-12,
                "Cancel restores the setting before the preview");

        smoothing = chart.smoothingSubmenu(1);
        item(smoothing, "Raw").doClick();
        assertEquals(0, chart.traceValue(0, 1), 1e-12);
        assertEquals(5, chart.traceValue(0, 2), 1e-12);

        smoothing = chart.smoothingSubmenu(1);
        item(smoothing, "Use global (100 ms)").doClick();
        assertTrue(settings.usesGlobalSmoothing("Signal"));
        assertEquals(5, chart.traceValue(0, 1), 1e-12);
    }

    private static JRadioButtonMenuItem item(JMenu menu, String text) {
        for (int i = 0; i < menu.getItemCount(); i++) {
            if (menu.getItem(i) instanceof JRadioButtonMenuItem && text.equals(menu.getItem(i).getText())) {
                return (JRadioButtonMenuItem) menu.getItem(i);
            }
        }
        throw new AssertionError("No smoothing item " + text);
    }
}
