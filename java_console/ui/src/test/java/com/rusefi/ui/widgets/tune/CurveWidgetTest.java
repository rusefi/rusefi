package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

public class CurveWidgetTest {
    @Test
    public void primingValuesRemainVisibleAndEditable() throws Throwable {
        // Issue #9191: screenshot values are valid (0..1250 mg), but the INI plot ends at 150.
        // All loaded points must be visible, and edits must respect field limits, not plot bounds.
        IniFileModel ini = readPrimingIni();
        ConfigurationImage image = new ConfigurationImage(24);
        ArrayIniField xField = (ArrayIniField) ini.findIniField("primeBins").get();
        ArrayIniField yField = (ArrayIniField) ini.findIniField("primeValues").get();
        Double[] temperatures = {-40.0, -20.0, 0.0, 20.0, 40.0, 60.0, 80.0, 100.0};
        Double[] masses = {755.0, 605.0, 265.0, 140.0, 75.0, 50.0, 45.0, 40.0};
        ConfigurationImageGetterSetter.setArrayValues(xField, image, temperatures);
        ConfigurationImageGetterSetter.setArrayValues(yField, image, masses);
        byte[] original = image.getContent().clone();

        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = new CurveWidget(ini.getCurves().get("primingPulse"), ini, image);
            JTable table = findTable(widget.getContentPane());
            CurveWidget.CurveCanvas canvas = (CurveWidget.CurveCanvas) widget.getContentPane().getComponent(0);
            AtomicInteger edits = new AtomicInteger();
            widget.setOnEdit(edits::incrementAndGet);
            for (Dimension size : new Dimension[]{new Dimension(470, 360), new Dimension(300, 180),
                    new Dimension(1560, 185)}) {
                canvas.setSize(size);
                for (int row = 0; row < masses.length; row++) {
                    assertEquals(masses[row], Double.valueOf(table.getValueAt(row, 1).toString()));
                    table.changeSelection(row, 1, false, false);
                    BufferedImage rendered = render(canvas);
                    assertTrue(hasColor(rendered, Color.MAGENTA), "visible point " + row + " at " + size);
                    assertFalse(hasCurveOutsidePlot(rendered), "curve must stay inside the plot");
                }
            }
            assertArrayEquals(original, image.getContent(), "opening and painting must not modify the tune");

            edit(table, 0, 1, "760");
            assertEquals("760", table.getValueAt(0, 1));
            assertEquals(760.0, ConfigurationImageGetterSetter.getArrayValues(yField, image)[0][0]);
            assertEquals(1, edits.get());

            canvas.setSize(470, 360);
            Point start = canvas.worldToCanvas(20, 140);
            Point end = canvas.worldToCanvas(20, 760);
            canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_PRESSED, 0, 0, start.x, start.y, 1, false));
            canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_DRAGGED, 0, 0, end.x, end.y, 0, false));
            canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_RELEASED, 0, 0, end.x, end.y, 1, false));
            assertEquals(760.0, ConfigurationImageGetterSetter.getArrayValues(yField, image)[3][0]);
            assertEquals(2, edits.get());
            assertEquals(605.0, ConfigurationImageGetterSetter.getArrayValues(yField, image)[1][0]);

            edit(table, 0, 1, "1400");
            assertEquals("1250", table.getValueAt(0, 1));
            assertEquals(1250.0, ConfigurationImageGetterSetter.getArrayValues(yField, image)[0][0]);
            edit(table, 0, 1, "-5");
            assertEquals("0", table.getValueAt(0, 1));
            edit(table, 7, 0, "135");
            assertEquals("135", table.getValueAt(7, 0));
        });
    }

    private static IniFileModel readPrimingIni() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "primeBins = array, S16, 0, [8], \"C\", 1, 0, -40, 150, 0\n"
            + "primeValues = array, U08, 16, [8], \"mg\", 5, 0, 0, 1250, 0\n"
            + "[CurveEditor]\ncurve = primingPulse, \"Priming pulse fuel mass\"\n"
            + "columnLabel = \"Coolant\", \"Prime Pulse\"\n"
            + "xAxis = -40, 120, 9\nyAxis = 0, 150, 9\n"
            + "xBins = primeBins\nyBins = primeValues\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(24);
        when(meta.getSignature()).thenReturn("test");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "test", meta);
    }

    @Test
    public void fieldLimitsResolveExpressionsWithoutUsingPlotBounds() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "xBins = array, S16, 0, [2], \"C\", 1, 0, -100, { fieldHigh }, 0\n"
            + "yBins = array, F32, 4, [2], \"mg\", 1, 0\n"
            + "metric = scalar, U08, 12, \"\", 1, 0, 0, 1, 0\n"
            + "[OutputChannels]\nfieldHigh = { metric ? 200 : 400 }\n"
            + "[CurveEditor]\ncurve = limits, \"Field limits\"\n"
            + "xAxis = -40, 120, 9\nyAxis = -10, 10, 9\nxBins = xBins\nyBins = yBins\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(16);
        when(meta.getSignature()).thenReturn("test");
        IniFileModel ini = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "test", meta);
        for (int mode = 0; mode < 3; mode++) {
            // A missing image must not turn an unresolved bound into zero or a plot-axis limit.
            // readNumericValue currently requests four bytes even for the U08 flag at offset 12.
            ConfigurationImage image = mode == 2 ? null : new ConfigurationImage(16);
            if (image != null) {
                image.getContent()[12] = (byte) mode;
                ConfigurationImageGetterSetter.setArrayValues((ArrayIniField) ini.findIniField("xBins").get(),
                    image, new Double[]{-50.0, 150.0});
                ConfigurationImageGetterSetter.setArrayValues((ArrayIniField) ini.findIniField("yBins").get(),
                    image, new Double[]{-25.0, 55.0});
            }
            int expectedMax = mode == 0 ? 400 : mode == 1 ? 200 : 999;
            SwingUtilities.invokeAndWait(() -> {
                CurveWidget widget = new CurveWidget(ini.getCurves().get("limits"), ini, image);
                JTable table = findTable(widget.getContentPane());
                JPanel canvas = (JPanel) widget.getContentPane().getComponent(0);
                canvas.setSize(470, 360);
                for (int row = 0; row < 2; row++) {
                    table.changeSelection(row, 1, false, false);
                    assertTrue(hasColor(render(canvas), Color.MAGENTA));
                }
                edit(table, 0, 0, "-999");
                assertEquals("-100", table.getValueAt(0, 0));
                edit(table, 1, 0, "999");
                assertEquals(Integer.toString(expectedMax), table.getValueAt(1, 0));
                edit(table, 0, 1, "-500");
                assertEquals("-500", table.getValueAt(0, 1));
                edit(table, 1, 1, "1000");
                assertEquals("1000", table.getValueAt(1, 1));
                table.changeSelection(1, 1, false, false);
                assertTrue(hasColor(render(canvas), Color.MAGENTA));
                AtomicInteger edits = new AtomicInteger();
                widget.setOnEdit(edits::incrementAndGet);
                for (String invalid : new String[]{"NaN", "Infinity", "-Infinity"}) {
                    edit(table, 1, 1, invalid);
                    assertEquals("1000", table.getValueAt(1, 1));
                }
                assertEquals(0, edits.get());
                if (image != null) {
                    assertArrayEquals(new Double[][]{{-500.0}, {1000.0}},
                        ConfigurationImageGetterSetter.getArrayValues((ArrayIniField) ini.findIniField("yBins").get(), image));
                }
            });
        }
    }

    @Test
    public void draggingUsesOneScaleUntilRelease() throws Throwable {
        IniFileModel ini = readPrimingIni();
        ConfigurationImage image = new ConfigurationImage(24);
        ArrayIniField xField = (ArrayIniField) ini.findIniField("primeBins").get();
        ArrayIniField yField = (ArrayIniField) ini.findIniField("primeValues").get();
        ConfigurationImageGetterSetter.setArrayValues(xField, image,
            new Double[]{-40.0, -20.0, 0.0, 20.0, 40.0, 60.0, 80.0, 100.0});
        ConfigurationImageGetterSetter.setArrayValues(yField, image,
            new Double[]{755.0, 605.0, 265.0, 140.0, 75.0, 50.0, 45.0, 40.0});
        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = new CurveWidget(ini.getCurves().get("primingPulse"), ini, image);
            CurveWidget.CurveCanvas canvas = (CurveWidget.CurveCanvas) widget.getContentPane().getComponent(0);
            canvas.setSize(470, 360);
            JTable table = findTable(widget.getContentPane());
            table.changeSelection(3, 1, false, false);
            Point start = canvas.worldToCanvas(20, 140);
            Point end = canvas.worldToCanvas(20, 1200);
            canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_PRESSED, 0, 0, start.x, start.y, 1, false));
            for (int i = 0; i < 2; i++) {
                canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_DRAGGED, 0, 0, end.x, end.y, 0, false));
                assertEquals("1200", table.getValueAt(3, 1));
                assertEquals(end, canvas.worldToCanvas(20, 1200), "scale stays fixed during the gesture");
                assertFalse(hasCurveOutsidePlot(render(canvas)), "off-plot drag must not paint over the title");
            }
            canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_RELEASED, 0, 0, end.x, end.y, 1, false));
            assertEquals(1200.0, ConfigurationImageGetterSetter.getArrayValues(yField, image)[3][0]);
            assertTrue(hasColor(render(canvas), Color.MAGENTA), "released point is visible after fitting the scale");
        });
    }

    private static BufferedImage render(JPanel canvas) {
        BufferedImage image = new BufferedImage(canvas.getWidth(), canvas.getHeight(), BufferedImage.TYPE_INT_RGB);
        Graphics2D graphics = image.createGraphics();
        try {
            canvas.paint(graphics);
        } finally {
            graphics.dispose();
        }
        return image;
    }

    private static boolean hasColor(BufferedImage image, Color color) {
        for (int y = 0; y < image.getHeight(); y++) {
            for (int x = 0; x < image.getWidth(); x++) {
                if (image.getRGB(x, y) == color.getRGB()) {
                    return true;
                }
            }
        }
        return false;
    }

    private static boolean hasCurveOutsidePlot(BufferedImage image) {
        for (int y = 0; y < image.getHeight(); y++) {
            for (int x = 0; x < image.getWidth(); x++) {
                if (x >= 50 && x <= image.getWidth() - 20 && y >= 20 && y <= image.getHeight() - 40) {
                    continue;
                }
                Color pixel = new Color(image.getRGB(x, y));
                int red = pixel.getRed();
                int green = pixel.getGreen();
                int blue = pixel.getBlue();
                // Include antialiased orange/green/magenta over black; exclude red labels and white text.
                if ((red > green && green > 0 && blue == 0)
                        || (green > red && red == blue) || (red == blue && red > green)) {
                    return true;
                }
            }
        }
        return false;
    }

    @Test
    public void coolantLimitEdit() throws Throwable {
        IniFileModel ini = readIni();
        ConfigurationImage image = new ConfigurationImage(20);
        image.getContent()[16] = 1; // Metric interface
        ArrayIniField xField = (ArrayIniField) ini.findIniField("cltRevLimitRpmBins").get();
        ArrayIniField yField = (ArrayIniField) ini.findIniField("cltRevLimitRpm").get();
        ConfigurationImageGetterSetter.setArrayValues(xField, image, new Double[]{-20.0, 0.0, 11.0, 11.0});
        ConfigurationImageGetterSetter.setArrayValues(yField, image, new Double[]{3000.0, 6000.0, 6000.0, 2000.0});

        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = new CurveWidget(ini.getCurves().get("cltRevLimitCurve"), ini, image);
            AtomicInteger edits = new AtomicInteger();
            widget.setOnEdit(edits::incrementAndGet);
            JTable table = findTable(widget.getContentPane());
            table.changeSelection(3, 0, false, false);
            assertTrue(table.editCellAt(3, 0));
            ((JTextField) table.getEditorComponent()).setText("115");
            assertTrue(table.getCellEditor().stopCellEditing());

            // Issue #10218: the fix must preserve the entered 115 in the table and tune.
            assertEquals("115", table.getValueAt(3, 0));
            assertArrayEquals(new Double[][]{{-20.0}, {0.0}, {11.0}, {115.0}},
                ConfigurationImageGetterSetter.getArrayValues(xField, image));
            assertArrayEquals(new Double[][]{{3000.0}, {6000.0}, {6000.0}, {2000.0}},
                ConfigurationImageGetterSetter.getArrayValues(yField, image));
            assertEquals(1, edits.get());

            // The selected 115 C / 2000 RPM point must be visible on the -40..120 plot.
            table.changeSelection(3, 0, false, false);
            JPanel canvas = (JPanel) widget.getContentPane().getComponent(0);
            canvas.setSize(470, 360);
            BufferedImage rendered = new BufferedImage(470, 360, BufferedImage.TYPE_INT_RGB);
            Graphics2D graphics = rendered.createGraphics();
            try {
                canvas.paint(graphics);
            } finally {
                graphics.dispose();
            }
            assertEquals(Color.MAGENTA.getRGB(), rendered.getRGB(437, 245));

            canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_PRESSED, 0, 0, 437, 245, 1, false));
            canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_DRAGGED, 0, 0, 450, 95, 0, false));
            canvas.dispatchEvent(new MouseEvent(canvas, MouseEvent.MOUSE_RELEASED, 0, 0, 450, 95, 1, false));
            assertEquals(120.0, ConfigurationImageGetterSetter.getArrayValues(xField, image)[3][0]);
            assertEquals(6000.0, ConfigurationImageGetterSetter.getArrayValues(yField, image)[3][0]);
            assertEquals(2, edits.get());
        });
    }

    @Test
    public void metricEditsRespectResolvedLimitsAndNeighbours() throws Throwable {
        checkCoolantLimits(1, 200);
    }

    @Test
    public void imperialEditsRespectResolvedLimitsAndNeighbours() throws Throwable {
        checkCoolantLimits(0, 200);
    }

    private static void checkCoolantLimits(int metric, int maximum) throws Throwable {
        IniFileModel ini = readIni();
        ConfigurationImage image = new ConfigurationImage(20);
        image.getContent()[16] = (byte) metric;
        ArrayIniField xField = (ArrayIniField) ini.findIniField("cltRevLimitRpmBins").get();
        ConfigurationImageGetterSetter.setArrayValues(xField, image, new Double[]{-20.0, 0.0, 11.0, 115.0});
        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = new CurveWidget(ini.getCurves().get("cltRevLimitCurve"), ini, image);
            JTable table = findTable(widget.getContentPane());
            edit(table, 3, 0, "999");
            assertEquals(Integer.toString(maximum), table.getValueAt(3, 0));
            edit(table, 0, 0, "-100");
            assertEquals("-40", table.getValueAt(0, 0));
            edit(table, 1, 0, "100");
            assertEquals("11", table.getValueAt(1, 0));
            edit(table, 2, 1, "9000");
            assertEquals("9000", table.getValueAt(2, 1));
            assertEquals((double) maximum, ConfigurationImageGetterSetter.getArrayValues(xField, image)[3][0]);
        });
    }

    @Test
    public void axisExpressionsUseEachTuneWithoutChangingIniMetadata() throws Throwable {
        IniFileModel ini = readIni();
        ConfigurationImage image = new ConfigurationImage(20);
        image.getContent()[16] = 1;
        assertEquals(120, ini.getCurves().get("cltRevLimitCurve").getxAxis().resolve(ini, image).getMax());
        image.getContent()[16] = 0;
        assertEquals(248, ini.getCurves().get("cltRevLimitCurve").getxAxis().resolve(ini, image).getMax());
        assertEquals(496.0, ExpressionEvaluator.evaluateNumericExpression("{ nestedLimit }", ini, image));
        assertNull(ExpressionEvaluator.evaluateNumericExpression("{ cyclicA }", ini, image));
        assertNull(ExpressionEvaluator.evaluateNumericExpression("{ unknownLimit }", ini, image));
    }

    private static void edit(JTable table, int row, int column, String value) {
        table.changeSelection(row, column, false, false);
        table.setValueAt(value, row, column);
    }

    private static IniFileModel readIni() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "cltRevLimitRpmBins = array, S16, 0, [4], \"C\", 1, 0, -40, 200, 0\n"
            + "cltRevLimitRpm = array, U16, 8, [4], \"RPM\", 1, 0, 0, 20000, 0\n"
            + "useMetricOnInterface = scalar, U08, 16, \"\", 1, 0, 0, 1, 0\n"
            + "[OutputChannels]\ncltHighXaxis = { useMetricOnInterface ? 120 : 248 }\n"
            + "nestedLimit = { cltHighXaxis * 2 }\ncyclicA = { cyclicB }\ncyclicB = { cyclicA }\n"
            + "[CurveEditor]\ncurve = cltRevLimitCurve, \"Engine Temperature RPM Limit\"\n"
            + "columnLabel = \"Coolant\", \"RPM Limit\"\n"
            + "xAxis = -40, { cltHighXaxis }, 9\nyAxis = 0, 8000, 9\n"
            + "xBins = cltRevLimitRpmBins\nyBins = cltRevLimitRpm\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(20);
        when(meta.getSignature()).thenReturn("test");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "test", meta);
    }

    private static JTable findTable(Container container) {
        for (Component child : container.getComponents()) {
            if (child instanceof JTable) {
                return (JTable) child;
            }
            if (child instanceof Container) {
                JTable table = findTable((Container) child);
                if (table != null) {
                    return table;
                }
            }
        }
        return null;
    }
}
