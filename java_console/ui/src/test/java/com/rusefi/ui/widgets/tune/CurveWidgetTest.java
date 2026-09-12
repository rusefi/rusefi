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
        checkCoolantLimits(1, 120);
    }

    @Test
    public void imperialEditsRespectResolvedLimitsAndNeighbours() throws Throwable {
        checkCoolantLimits(0, 248);
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
            assertEquals("8000", table.getValueAt(2, 1));
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
