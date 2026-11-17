package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.ui.util.ScrollablePanel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class TuningTablePreviewTest {
    @Test
    void sandboxIgnitionTableShowsPreviewInTallViewport() throws Exception, com.rusefi.ini.reader.IniParsingException {
        IniFileModel ini;
        try (java.io.InputStream stream = getClass().getResourceAsStream("/january.ini")) {
            RawIniFile raw = IniFileReaderUtil.read(stream, "january.ini");
            ini = IniFileReaderUtil.readIniFile(raw, "january.ini", new IniFileMetaInfoImpl(raw));
        }
        ConfigurationImage image = Msq.readTune(new java.io.File(
            getClass().getResource("/january_tune.msq").toURI()).getAbsolutePath()).asImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("Ignition advance");
            view.displayTable(ini, "ignitionTableTbl", image);
            ScrollablePanel dialog = new ScrollablePanel();
            dialog.setLayout(new BoxLayout(dialog, BoxLayout.Y_AXIS));
            dialog.add(view.getContent());
            JScrollPane scroll = new JScrollPane(dialog);
            scroll.setSize(1100, 800);
            scroll.doLayout();
            scroll.getViewport().doLayout();
            dialog.doLayout();
            view.getContent().doLayout();
            JPanel cards = (JPanel) view.getContent().getComponent(1);
            resize(cards, cards.getWidth(), cards.getHeight());
            JPanel grid = (JPanel) cards.getComponent(0);
            JTable table = (JTable) ((JScrollPane) grid.getComponent(0)).getViewport().getView();
            assertEquals(16, table.getRowCount());
            assertEquals(17, table.getColumnCount());
            assertEquals("650", table.getColumnName(1));
            assertEquals("7000", table.getColumnName(16));
            assertEquals("120", table.getValueAt(0, 0));
            assertEquals("21", table.getValueAt(15, 0));
            assertEquals("10.5", table.getValueAt(0, 1));
            assertEquals("35", table.getValueAt(0, 16));
            assertEquals("12.2", table.getValueAt(15, 1));
            assertEquals("42.9", table.getValueAt(15, 16));
            assertTrue(grid.getComponent(1).isVisible(), "The sandbox's real 16x16 ignition table must have a preview");
            invalidateTree(scroll);
            layoutTree(scroll);
            Surface3DView preview = (Surface3DView) grid.getComponent(1);
            Point bottom = SwingUtilities.convertPoint(preview, 0, preview.getHeight(), dialog);
            assertEquals(scroll.getViewport().getExtentSize().height, bottom.y,
                "Preview must fill the viewport below the table");
            assertTrue(preview.getHeight() > 160);
        });
    }

    @Test
    void scrollableDialogHasSpaceBeyondTablesPreferredHeight() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("Ignition advance");
            loadTable(view, true);
            ScrollablePanel dialog = new ScrollablePanel();
            dialog.setLayout(new BoxLayout(dialog, BoxLayout.Y_AXIS));
            dialog.add(view.getContent());
            JScrollPane scroll = new JScrollPane(dialog);
            scroll.setSize(900, 800);
            scroll.doLayout();
            scroll.getViewport().doLayout();
            dialog.doLayout();
            view.getContent().doLayout();
            JPanel cards = (JPanel) view.getContent().getComponent(1);
            // Emulate the compact preferred height of a table inside a scrollable dialog.
            resize(cards, 800, 180);
            JPanel grid = (JPanel) cards.getComponent(0);
            assertTrue(grid.getComponent(1).isVisible(),
                "The outer viewport has space even though the table has a compact height");
            scroll.setSize(900, 180);
            scroll.doLayout();
            scroll.getViewport().doLayout();
            resize(cards, 800, 400);
            assertTrue(grid.getComponent(1).isVisible(), "A short viewport keeps the preview");
            assertEquals(240, grid.getComponent(1).getPreferredSize().height,
                "A short viewport keeps the preview at its minimum height");
            assertTrue(dialog.getPreferredSize().height > scroll.getViewport().getExtentSize().height,
                "The dialog grows past the short viewport, so it scrolls to the preview");
        });
    }

    @Test
    void previewFollowsAvailableSpaceAndViewMode() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("VE");
            JPanel cards = (JPanel) view.getContent().getComponent(1);
            JPanel grid = (JPanel) cards.getComponent(0);
            Surface3DView preview = (Surface3DView) grid.getComponent(1);
            loadTable(view, true);
            resize(cards, 600, 400);
            assertTrue(preview.isVisible());
            grid.doLayout();
            int shortHeight = preview.getHeight();
            resize(cards, 600, 700);
            grid.doLayout();
            assertEquals(300, preview.getHeight() - shortHeight,
                "All additional height should go to the surface");
            JCheckBox toggle = findToggle(view.getContent());
            assertNotNull(toggle);
            toggle.doClick();
            assertFalse(preview.isVisible());
            toggle.doClick();
            assertTrue(preview.isVisible());
            resize(cards, 600, 180);
            assertFalse(preview.isVisible());
            resize(cards, 600, 400);
            assertTrue(preview.isVisible());
            resize(cards, 250, 400);
            assertFalse(preview.isVisible());
            resize(cards, 600, 400);
            loadTable(view, false);
            assertFalse(preview.isVisible(), "Missing axes must not enable a misleading preview");
        });
    }

    @Test
    void tableSelectionIsShownOnBothSurfaces() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("VE");
            loadTable(view, true);
            JPanel cards = (JPanel) view.getContent().getComponent(1);
            JPanel grid = (JPanel) cards.getComponent(0);
            Surface3DView preview = (Surface3DView) grid.getComponent(1);
            Surface3DView full = (Surface3DView) cards.getComponent(1);
            JTable table = (JTable) ((JScrollPane) grid.getComponent(0)).getViewport().getView();
            assertFalse(preview.isSelected(1, 0), "A newly displayed table has no selection");

            // The top table row shows the last data row; column 0 holds the load axis.
            table.changeSelection(0, 1, false, false);
            table.changeSelection(0, 2, false, true);
            for (Surface3DView surface : new Surface3DView[]{preview, full}) {
                assertTrue(surface.isSelected(1, 0));
                assertTrue(surface.isSelected(1, 1));
                assertFalse(surface.isSelected(0, 0));
                assertFalse(surface.isSelected(1, 2));
            }

            table.changeSelection(1, 0, false, false);
            assertFalse(preview.isSelected(0, 0), "Selecting the axis column selects no surface point");
            table.clearSelection();
            assertFalse(preview.isSelected(1, 0));
        });
    }

    private static void resize(JPanel cards, int width, int height) {
        cards.setSize(width, height);
        cards.doLayout();
        for (ComponentListener listener : cards.getComponentListeners()) {
            listener.componentResized(new ComponentEvent(cards, ComponentEvent.COMPONENT_RESIZED));
        }
    }

    private static void layoutTree(Container container) {
        container.doLayout();
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                layoutTree((Container) component);
            }
        }
    }

    private static void invalidateTree(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                invalidateTree((Container) component);
            }
        }
        container.invalidate();
    }

    private static JCheckBox findToggle(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JCheckBox) {
                return (JCheckBox) component;
            }
            if (component instanceof Container) {
                JCheckBox found = findToggle((Container) component);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void loadTable(TuningTableView view, boolean withAxes) {
        IniFileModel ini = mock(IniFileModel.class);
        TableModel table = mock(TableModel.class);
        ArrayIniField z = new ArrayIniField("z", 0, FieldType.FLOAT, 3, 2, "", 1, "0", "100", "1");
        ArrayIniField x = new ArrayIniField("x", 24, FieldType.FLOAT, 3, 1, "", 1, "0", "8000", "0");
        ArrayIniField y = new ArrayIniField("y", 36, FieldType.FLOAT, 2, 1, "", 1, "0", "100", "0");
        ConfigurationImage image = new ConfigurationImage(44);
        ConfigurationImageGetterSetter.setArrayValues(z, image, new Double[][]{{10.0, 20.0, 30.0}, {40.0, 50.0, 60.0}});
        ConfigurationImageGetterSetter.setArrayValues(x, image, new Double[][]{{1000.0, 1500.0, 6000.0}});
        ConfigurationImageGetterSetter.setArrayValues(y, image, new Double[][]{{20.0, 100.0}});
        when(ini.getTable("table")).thenReturn(table);
        when(table.getZBinsConstant()).thenReturn("z");
        when(table.getXBinsConstant()).thenReturn("x");
        when(table.getYBinsConstant()).thenReturn("y");
        when(ini.findIniField("z")).thenReturn(Optional.of(z));
        when(ini.findIniField("x")).thenReturn(withAxes ? Optional.of(x) : Optional.empty());
        when(ini.findIniField("y")).thenReturn(withAxes ? Optional.of(y) : Optional.empty());
        view.displayTable(ini, "table", image);
    }
}
