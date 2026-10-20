package com.rusefi.ui.logview;

import com.rusefi.logview.BinaryLogReader;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

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
        assertEquals("Time", log.name(0));
        assertEquals("RPM", log.name(32));
        assertEquals("TPS", log.name(40));
        assertEquals("MAP", log.name(45));
        assertTrue(Double.isFinite(log.value(0, 0)));
        assertTrue(log.maximum(0) > log.minimum(0));
    }

    @Test
    void stillLoadsTextFixture() throws IOException {
        CsvLogReader log = LogViewerSandbox.load("/logview/test-log-1.msl");
        assertEquals("Time", log.name(0));
        assertTrue(log.sampleCount() > 3000);
    }

    @Test
    void timeIsNotASelectableFieldInEitherLogFormat() throws IOException {
        for (String path : new String[]{"/logview/test-log-binary-1.mlg", "/logview/test-log-1.msl"}) {
            CsvLogReader log = LogViewerSandbox.load(path);
            LogViewerPanel panel = new LogViewerPanel(log);
            JSplitPane split = (JSplitPane) panel.getComponent(0);
            JPanel fields = (JPanel) split.getLeftComponent();
            JScrollPane scroll = (JScrollPane) fields.getComponent(1);
            JList<?> list = (JList<?>) scroll.getViewport().getView();

            assertEquals(log.fieldCount() - 1, list.getModel().getSize(), path);
            for (int index = 0; index < list.getModel().getSize(); index++) {
                assertNotEquals(0, list.getModel().getElementAt(index), path);
            }

            JPanel heading = (JPanel) fields.getComponent(0);
            JTextField search = (JTextField) heading.getComponent(1);
            search.setText("Time");
            for (int index = 0; index < list.getModel().getSize(); index++) {
                assertNotEquals(0, list.getModel().getElementAt(index), path);
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

        int labelColor = 0xffc6d1de;
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
