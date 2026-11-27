package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.Optional;
import java.util.Set;
import java.util.concurrent.atomic.AtomicLong;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Issue #10670: a table shows where the engine is running, from the output channels its INI
 * definition names for each axis ({@code xBins = veRpmBins, RPMValue}).
 */
public class TuningTableLiveMarkerTest {
    private static final String RPM_CHANNEL = "issue10670Rpm";
    private static final String LOAD_CHANNEL = "issue10670Load";

    @Test
    public void axisChannelsComeFromTheIniTableAndFollowTheFlipOption() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node settings = new Node();
            TuningTableView view = displayed(settings);
            assertArrayEquals(new String[]{RPM_CHANNEL, LOAD_CHANNEL}, view.liveFeedForUnitTest().getChannels());

            ((JCheckBoxMenuItem) view.orientationMenuFor(0).getComponent(0)).doClick();
            assertArrayEquals(new String[]{LOAD_CHANNEL, RPM_CHANNEL}, view.liveFeedForUnitTest().getChannels());
        });
    }

    @Test
    public void markerIsOnTheCellNearestTheOperatingPoint() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = displayed(new Node());
            assertNull(view.liveCell(), "No live data, no marker");

            // Load rows are shown top-down: row 1 is 40 kPa, row 0 is 120 kPa.
            view.liveFeedForUnitTest().setValues(2500, 40);
            assertEquals(new Point(1, 2), view.liveCell());
            view.liveFeedForUnitTest().setValues(1800, 100);
            assertEquals(new Point(0, 2), view.liveCell());
            view.liveFeedForUnitTest().setValues(1200, 60);
            assertEquals(new Point(1, 1), view.liveCell());
            view.liveFeedForUnitTest().setValues(9000, 10);
            assertEquals(new Point(1, 3), view.liveCell(), "Values beyond the axes clamp to the edge cells");
            view.liveFeedForUnitTest().setValues(Double.NaN, 40);
            assertNull(view.liveCell());
        });
    }

    @Test
    public void markerFollowsTheDisplayedOrientation() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node settings = new Node();
            TuningTableView view = displayed(settings);
            view.liveFeedForUnitTest().setValues(6000, 120);
            assertEquals(new Point(0, 3), view.liveCell());

            ((JCheckBoxMenuItem) view.orientationMenuFor(0).getComponent(0)).doClick();
            view.liveFeedForUnitTest().setValues(120, 6000); // values now arrive as (load, RPM)
            JTable table = findTable(view.getContent());
            Point cell = view.liveCell();
            assertEquals("120", table.getColumnName(cell.y));
            assertEquals("6000", table.getValueAt(cell.x, 0));
        });
    }

    @Test
    public void markerDisappearsWhenDataStopsArriving() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicLong now = new AtomicLong(10_000);
            TuningTableView view = displayed(new Node());
            view.liveFeedForUnitTest().setClockForUnitTest(now::get);
            view.liveFeedForUnitTest().setValues(2500, 40);

            now.addAndGet(LiveChannelFeed.STALE_MILLIS);
            view.liveFeedForUnitTest().expire();
            assertNotNull(view.liveCell());
            now.incrementAndGet();
            view.liveFeedForUnitTest().expire();
            assertNull(view.liveCell(), "A lost connection leaves no frozen marker");
        });
    }

    @Test
    public void axisChannelsArePolledOnlyWhileTheTableIsShown() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = displayed(new Node());
            try {
                assertFalse(demanded(RPM_CHANNEL));
                view.liveFeedForUnitTest().setActive(true);
                assertTrue(demanded(RPM_CHANNEL));
                assertTrue(demanded(LOAD_CHANNEL));
                view.liveFeedForUnitTest().setValues(2500, 40);

                view.liveFeedForUnitTest().setActive(false);
                assertFalse(demanded(RPM_CHANNEL));
                assertFalse(demanded(LOAD_CHANNEL));
                assertNull(view.liveCell(), "A hidden table drops its live point");
            } finally {
                view.liveFeedForUnitTest().setActive(false);
            }
        });
    }

    @Test
    public void markerIsPaintedOverTheCell() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = displayed(new Node());
            JTable table = findTable(view.getContent());
            table.setSize(400, table.getRowHeight() * table.getRowCount());
            table.doLayout();
            view.liveFeedForUnitTest().setValues(2500, 40);
            Point cell = view.liveCell();
            Rectangle bounds = table.getCellRect(cell.x, cell.y, true);

            BufferedImage image = new BufferedImage(table.getWidth(), table.getHeight(), BufferedImage.TYPE_INT_RGB);
            Graphics2D g = image.createGraphics();
            table.paint(g);
            g.dispose();

            int middle = bounds.y + bounds.height / 2;
            assertEquals(Color.BLACK.getRGB(), image.getRGB(bounds.x, middle), "Cell outline");
            assertEquals(Color.WHITE.getRGB(), image.getRGB(bounds.x + 1, middle));
            assertEquals(Color.BLACK.getRGB(), image.getRGB(bounds.x + 2, middle));
            int centre = (int) bounds.getCenterX();
            assertNotEquals(Color.WHITE.getRGB(), image.getRGB(centre, middle), "The dot is hollow, so the value shows");
            // the top of the ring, above the digits it circles
            Color ring = new Color(image.getRGB(centre, middle - TuningTableView.LIVE_DOT_RADIUS));
            assertTrue(Math.min(ring.getRed(), Math.min(ring.getGreen(), ring.getBlue())) > 200,
                "The dot's white ring sits on the bin values exactly: " + ring);
        });
    }

    @Test
    public void trailKeepsRecentPointsFadingWithAge() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicLong now = new AtomicLong(0);
            TuningTableView view = displayed(new Node());
            LiveChannelFeed live = view.liveFeedForUnitTest();
            live.setClockForUnitTest(now::get);

            live.setValues(1000, 40);
            now.set(500);
            live.setValues(2500, 40);
            live.setValues(2500, 40);
            java.util.List<double[]> trail = view.liveTrail();
            assertEquals(2, trail.size(), "A point that stays does not add to the trail");
            assertArrayEquals(new double[]{0, 0, 0.75}, trail.get(0), 1e-9, "Older point, x/y indexes and opacity");
            assertArrayEquals(new double[]{1, 0, 1}, trail.get(1), 1e-9, "The current point ends the trail");
            for (Surface3DView surface : surfaces(view.getContent())) {
                assertEquals(2, surface.liveTrailForUnitTest().length, "Both 3D views show the trail");
            }

            now.set(2_600);
            live.setValues(6000, 120);
            trail = view.liveTrail();
            assertEquals(1, trail.size(), "Points older than the trail time drop off");
            assertArrayEquals(new double[]{2, 1, 1}, trail.get(0), 1e-9);

            live.setValues(Double.NaN, 40);
            assertTrue(view.liveTrail().isEmpty());
            for (Surface3DView surface : surfaces(view.getContent())) {
                assertNull(surface.liveTrailForUnitTest(), "No live data, nothing on the 3D views");
            }
        });
    }

    @Test
    public void trailIsCappedAndClearedByANewOrientation() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = displayed(new Node());
            LiveChannelFeed live = view.liveFeedForUnitTest();
            live.setClockForUnitTest(() -> 1_000);
            for (int i = 0; i < TuningTableView.TRAIL_MAX_POINTS + 10; i++) {
                live.setValues(1000 + i, 40);
            }
            assertEquals(TuningTableView.TRAIL_MAX_POINTS, view.liveTrail().size());

            ((JCheckBoxMenuItem) view.orientationMenuFor(0).getComponent(0)).doClick();
            assertTrue(view.liveTrail().isEmpty(), "Flipped axes start a new trail");
        });
    }

    @Test
    public void trailIsPaintedBehindTheMarker() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicLong now = new AtomicLong(0);
            TuningTableView view = displayed(new Node());
            JTable table = findTable(view.getContent());
            table.setSize(400, table.getRowHeight() * table.getRowCount());
            table.doLayout();
            LiveChannelFeed live = view.liveFeedForUnitTest();
            live.setClockForUnitTest(now::get);
            live.setValues(1000, 40);
            live.setValues(6000, 40);
            Rectangle first = table.getCellRect(1, 1, true);

            BufferedImage image = new BufferedImage(table.getWidth(), table.getHeight(), BufferedImage.TYPE_INT_RGB);
            Graphics2D g = image.createGraphics();
            table.paint(g);
            g.dispose();
            assertEquals(Color.WHITE.getRGB(), image.getRGB((int) first.getCenterX(), (int) first.getCenterY()),
                "A fresh earlier point is drawn at full strength");
        });
    }

    private static java.util.List<Surface3DView> surfaces(Container container) {
        java.util.List<Surface3DView> found = new java.util.ArrayList<>();
        for (Component component : container.getComponents()) {
            if (component instanceof Surface3DView) {
                found.add((Surface3DView) component);
            } else if (component instanceof Container) {
                found.addAll(surfaces((Container) component));
            }
        }
        return found;
    }

    /** RPM bins 1000/2500/6000 on X, load bins 40/120 on Y. */
    private static TuningTableView displayed(Node settings) {
        IniFileModel ini = mock(IniFileModel.class);
        TableModel definition = mock(TableModel.class);
        ArrayIniField values = new ArrayIniField("ve", 0, FieldType.UINT8, 3, 2, "", 1, "0", "255", "0");
        ArrayIniField rpm = new ArrayIniField("rpm", 0, FieldType.UINT16, 3, 1, "RPM", 1, "0", "8000", "0");
        ArrayIniField load = new ArrayIniField("load", 6, FieldType.UINT16, 1, 2, "kPa", 1, "0", "300", "0");
        ConfigurationImage valuesImage = ConfigurationImageTestHelper.newLittleEndian(6);
        ConfigurationImage axisImage = ConfigurationImageTestHelper.newLittleEndian(10);
        ConfigurationImageGetterSetter.setArrayValues(values, valuesImage,
            new Double[][]{{11.0, 22.0, 33.0}, {44.0, 55.0, 66.0}});
        ConfigurationImageGetterSetter.setArrayValues(rpm, axisImage, new Double[][]{{1000.0, 2500.0, 6000.0}});
        ConfigurationImageGetterSetter.setArrayValues(load, axisImage, new Double[][]{{40.0}, {120.0}});
        when(ini.getTable("ve")).thenReturn(definition);
        when(definition.getZBinsConstant()).thenReturn("ve");
        when(definition.getXBinsConstant()).thenReturn("rpm");
        when(definition.getYBinsConstant()).thenReturn("load");
        when(definition.getXBinsChannel()).thenReturn(RPM_CHANNEL);
        when(definition.getYBinsChannel()).thenReturn(LOAD_CHANNEL);
        when(ini.findIniField("ve")).thenReturn(Optional.of(values));
        when(ini.findIniField("rpm")).thenReturn(Optional.of(rpm));
        when(ini.findIniField("load")).thenReturn(Optional.of(load));

        TuningTableView view = new TuningTableView("VE", false, settings);
        view.displayTable(ini, "ve", valuesImage, axisImage);
        return view;
    }

    private static boolean demanded(String channel) {
        Set<String> channels = SensorCentral.getInstance().getOutputChannelDemand().getChannels();
        return channels.contains(channel.toLowerCase());
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
