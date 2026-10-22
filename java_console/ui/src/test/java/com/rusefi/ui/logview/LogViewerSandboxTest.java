package com.rusefi.ui.logview;

import com.rusefi.logview.BinaryLogReader;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JList;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LogViewerSandboxTest {
    @Test
    void loadsBinaryFixtureWithChartFields() throws IOException {
        CsvLogReader log = LogViewerSandbox.load("/logview/test-log-binary-1.mlg");
        assertEquals(220, log.sampleCount());
        assertEquals(917, log.fieldCount());
        assertEquals(GAUGE_NAME_TIME, log.name(0));
        assertEquals("RPM", log.name(32));
        assertEquals("TPS", log.name(40));
        assertEquals("MAP", log.name(45));
        assertTrue(Double.isFinite(log.value(0, 0)));
        assertTrue(log.maximum(0) > log.minimum(0));
    }

    @Test
    void stillLoadsTextFixture() throws IOException {
        CsvLogReader log = LogViewerSandbox.load("/logview/test-log-1.msl");
        assertEquals(GAUGE_NAME_TIME, log.name(0));
        assertTrue(log.sampleCount() > 3000);
    }

    /** Time lookups use binary search, which requires non-decreasing time. */
    @Test
    void fixtureTimeNeverDecreases() throws IOException {
        for (String path : new String[]{"/logview/test-log-binary-1.mlg", "/logview/test-log-1.msl"}) {
            CsvLogReader log = LogViewerSandbox.load(path);
            for (int sample = 1; sample < log.sampleCount(); sample++) {
                assertTrue(log.value(sample, 0) >= log.value(sample - 1, 0), path + " sample " + sample);
            }
        }
    }

    @Test
    void timeIsNotASelectableFieldInEitherLogFormat() throws IOException {
        for (String path : new String[]{"/logview/test-log-binary-1.mlg", "/logview/test-log-1.msl"}) {
            CsvLogReader log = LogViewerSandbox.load(path);
            LogViewerPanel panel = new LogViewerPanel(log);
            JList<SignalSections.Row> list = panel.fieldList();

            // with zero and constant signals shown, every signal but Time is in exactly one group
            panel.hideZeroBox.doClick();
            if (panel.hideConstantBox.isSelected()) {
                panel.hideConstantBox.doClick();
            }
            int inGroups = 0;
            for (int index = 0; index < list.getModel().getSize(); index++) {
                SignalSections.Row row = list.getModel().getElementAt(index);
                assertNotEquals(0, row.field, path);
                if (row.isHeading() && row.kind == SignalSections.Kind.GROUP) {
                    inGroups += row.count;
                }
            }
            assertEquals(log.fieldCount() - 1, inGroups, path);

            panel.searchField().setText(GAUGE_NAME_TIME);
            for (int index = 0; index < list.getModel().getSize(); index++) {
                assertNotEquals(0, list.getModel().getElementAt(index).field, path);
            }
        }
    }

    @Test
    void chartAlwaysDrawsTimeScaleWithoutSelectedSignals() throws IOException {
        CsvLogReader log = LogViewerSandbox.load("/logview/test-log-1.msl");
        LogChart chart = new LogChart(log, new boolean[log.fieldCount()]);
        chart.setSize(500, 300);
        BufferedImage image = new BufferedImage(500, 300, BufferedImage.TYPE_INT_RGB);
        chart.paint(image.createGraphics());

        int labelColor = chart.palette().label.getRGB();
        int labelPixels = 0;
        for (int y = 255; y < 300; y++) {
            for (int x = 0; x < 500; x++) {
                if (image.getRGB(x, y) == labelColor) {
                    labelPixels++;
                }
            }
        }
        assertTrue(labelPixels > 0, "time scale labels should appear below the chart");
    }

    @Test
    void rejectsCorruptBinaryRecord() throws IOException {
        byte[] bytes;
        try (InputStream stream = getClass().getResourceAsStream("/logview/test-log-binary-1.mlg")) {
            bytes = stream.readAllBytes();
        }
        bytes[bytes.length - 1] ^= 1;
        assertThrows(IOException.class, () -> BinaryLogReader.read(new ByteArrayInputStream(bytes)));
    }
}
