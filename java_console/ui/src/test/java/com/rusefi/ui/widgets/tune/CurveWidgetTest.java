package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
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

            // Issue #10218: positive coolant edits must reach the configuration image.
            assertEquals("115", table.getValueAt(3, 0));
            assertArrayEquals(new Double[][]{{-20.0}, {0.0}, {11.0}, {115.0}},
                ConfigurationImageGetterSetter.getArrayValues(xField, image));
            assertArrayEquals(new Double[][]{{3000.0}, {6000.0}, {6000.0}, {2000.0}},
                ConfigurationImageGetterSetter.getArrayValues(yField, image));
            assertEquals(1, edits.get());
        });
    }

    private static IniFileModel readIni() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "cltRevLimitRpmBins = array, S16, 0, [4], \"C\", 1, 0, -40, 200, 0\n"
            + "cltRevLimitRpm = array, U16, 8, [4], \"RPM\", 1, 0, 0, 20000, 0\n"
            + "useMetricOnInterface = scalar, U08, 16, \"\", 1, 0, 0, 1, 0\n"
            + "[OutputChannels]\ncltHighXaxis = { useMetricOnInterface ? 120 : 248 }\n"
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
