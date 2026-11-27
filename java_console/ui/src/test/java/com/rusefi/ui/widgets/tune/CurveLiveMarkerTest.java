package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.core.SensorCentral;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.atomic.AtomicLong;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Issue #10670: a curve marks where the engine is, from the output channel its INI definition
 * names for the X axis ({@code xBins = fanBins, coolantTemperature}).
 */
public class CurveLiveMarkerTest {
    private static final String CHANNEL = "issue10670Coolant";

    @Test
    public void curveFollowsTheChannelOfItsXAxis() throws Throwable {
        IniFileModel ini = readFanIni();
        ConfigurationImage image = fanImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            assertArrayEquals(new String[]{CHANNEL}, widget(ini, "fanCurve", image).liveFeedForUnitTest().getChannels());
            assertArrayEquals(new String[]{null}, widget(ini, "plainCurve", image).liveFeedForUnitTest().getChannels(),
                "A curve without a channel has nothing to follow");
        });
    }

    @Test
    public void nearestRowFollowsTheLiveValue() throws Throwable {
        IniFileModel ini = readFanIni();
        ConfigurationImage image = fanImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = widget(ini, "fanCurve", image);
            LiveChannelFeed live = widget.liveFeedForUnitTest();
            assertEquals(-1, widget.liveRow(), "No live data, no marker");
            live.setValues(83);
            assertEquals(1, widget.liveRow());
            live.setValues(94);
            assertEquals(2, widget.liveRow());
            live.setValues(10);
            assertEquals(0, widget.liveRow(), "Below the first bin clamps to it");
            live.setValues(150);
            assertEquals(3, widget.liveRow());
            live.setValues(Double.NaN);
            assertEquals(-1, widget.liveRow());
        });
    }

    @Test
    public void liveRowIsOutlinedInTheTable() throws Throwable {
        IniFileModel ini = readFanIni();
        ConfigurationImage image = fanImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = widget(ini, "fanCurve", image);
            JTable table = findTable(widget.getContentPane());
            widget.liveFeedForUnitTest().setValues(83);
            assertNotNull(borderOf(table, 1));
            assertNotEquals(borderOf(table, 1), borderOf(table, 0));
        });
    }

    @Test
    public void livePointIsDrawnOnThePlot() throws Throwable {
        IniFileModel ini = readFanIni();
        ConfigurationImage image = fanImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = widget(ini, "fanCurve", image);
            CurveWidget.CurveCanvas canvas = (CurveWidget.CurveCanvas) widget.getContentPane().getComponent(0);
            canvas.setSize(470, 360);
            // Halfway between 80 C (20 %) and 90 C (60 %): the dot is on the curve at 40 %.
            Point dot = canvas.worldToCanvas(85, 40);
            assertNotEquals(CurveWidget.LIVE_POINT_COLOR.getRGB(), render(canvas).getRGB(dot.x, dot.y),
                "without data the curve line runs through that spot");
            widget.liveFeedForUnitTest().setValues(85);
            BufferedImage rendered = render(canvas);
            assertEquals(CurveWidget.LIVE_POINT_COLOR.getRGB(), rendered.getRGB(dot.x, dot.y));
        });
    }

    @Test
    public void markerDisappearsWhenDataStopsAndPollingStopsWhenHidden() throws Throwable {
        IniFileModel ini = readFanIni();
        ConfigurationImage image = fanImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            CurveWidget widget = widget(ini, "fanCurve", image);
            LiveChannelFeed live = widget.liveFeedForUnitTest();
            AtomicLong now = new AtomicLong(5_000);
            live.setClockForUnitTest(now::get);
            try {
                live.setActive(true);
                assertTrue(demanded());
                live.setValues(85);
                now.addAndGet(LiveChannelFeed.STALE_MILLIS + 1);
                live.expire();
                assertEquals(-1, widget.liveRow());

                live.setValues(85);
                live.setActive(false);
                assertFalse(demanded());
                assertEquals(-1, widget.liveRow());
            } finally {
                live.setActive(false);
            }
        });
    }

    private static CurveWidget widget(IniFileModel ini, String curve, ConfigurationImage image) {
        return new CurveWidget(ini.getCurves().get(curve), ini, image);
    }

    private static IniFileModel readFanIni() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "fanBins = array, U08, 0, [4], \"C\", 1, 0, 0, 200, 0\n"
            + "fanDuty = array, U08, 4, [4], \"%\", 1, 0, 0, 100, 0\n"
            + "[CurveEditor]\ncurve = fanCurve, \"Fan duty\"\n"
            + "columnLabel = \"Coolant\", \"Duty\"\n"
            + "xAxis = 0, 120, 7\nyAxis = 0, 100, 6\n"
            + "xBins = fanBins, " + CHANNEL + "\nyBins = fanDuty\n"
            + "curve = plainCurve, \"No channel\"\n"
            + "columnLabel = \"Coolant\", \"Duty\"\n"
            + "xAxis = 0, 120, 7\nyAxis = 0, 100, 6\n"
            + "xBins = fanBins\nyBins = fanDuty\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(8);
        when(meta.getSignature()).thenReturn("test");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "test", meta);
    }

    /** Bins 70/80/90/100 C, duty 0/20/60/100 %. */
    private static ConfigurationImage fanImage(IniFileModel ini) {
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(8);
        ConfigurationImageGetterSetter.setArrayValues((ArrayIniField) ini.findIniField("fanBins").get(), image,
            new Double[]{70.0, 80.0, 90.0, 100.0});
        ConfigurationImageGetterSetter.setArrayValues((ArrayIniField) ini.findIniField("fanDuty").get(), image,
            new Double[]{0.0, 20.0, 60.0, 100.0});
        return image;
    }

    private static Object borderOf(JTable table, int row) {
        Component c = table.prepareRenderer(table.getCellRenderer(row, 1), row, 1);
        return ((JComponent) c).getBorder();
    }

    private static boolean demanded() {
        return SensorCentral.getInstance().getOutputChannelDemand().getChannels().contains(CHANNEL.toLowerCase());
    }

    private static BufferedImage render(JComponent component) {
        BufferedImage image = new BufferedImage(component.getWidth(), component.getHeight(), BufferedImage.TYPE_INT_RGB);
        Graphics2D g = image.createGraphics();
        component.paint(g);
        g.dispose();
        return image;
    }

    private static boolean hasColor(BufferedImage image, Color color) {
        for (int x = 0; x < image.getWidth(); x++) {
            for (int y = 0; y < image.getHeight(); y++) {
                if (image.getRGB(x, y) == color.getRGB()) {
                    return true;
                }
            }
        }
        return false;
    }

    private static JTable findTable(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JTable) {
                return (JTable) component;
            }
            if (component instanceof Container) {
                JTable found = findTable((Container) component);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
