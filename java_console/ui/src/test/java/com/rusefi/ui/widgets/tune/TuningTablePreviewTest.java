package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImageTestHelper;
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
            TuningTableView view = previewOn("Ignition advance");
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
            TuningTableView view = previewOn("Ignition advance");
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
            // Without a window, invalidation does not reach BoxLayout's cached sizes; a shown dialog's does.
            invalidateTree(scroll);
            assertTrue(dialog.getPreferredSize().height > scroll.getViewport().getExtentSize().height,
                "The dialog grows past the short viewport, so it scrolls to the preview");
        });
    }

    @Test
    void previewIsOffByDefaultAndTheChoiceIsRemembered() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            com.rusefi.core.preferences.storage.Node settings = new com.rusefi.core.preferences.storage.Node();
            TuningTableView view = new TuningTableView("VE", false, settings);
            JPanel cards = (JPanel) view.getContent().getComponent(1);
            Surface3DView preview = (Surface3DView) ((JPanel) cards.getComponent(0)).getComponent(1);
            int[] notified = {0};
            view.setOnOrientationChange(() -> notified[0]++);
            loadTable(view, true);
            resize(cards, 600, 700);
            JToggleButton table = findSegment(view.getContent(), "Table");
            JToggleButton both = findSegment(view.getContent(), "Both");
            JToggleButton surface = findSegment(view.getContent(), "3D");
            assertNotNull(both);
            assertTrue(table.isSelected(), "Tables open on the grid alone");
            assertFalse(preview.isVisible(), "Room below the grid does not show the preview by itself");

            both.doClick();
            assertTrue(preview.isVisible());
            assertFalse(view.is3DShown());
            assertTrue(settings.getBoolProperty(TuningTableView.PREVIEW_PROPERTY), "The choice is saved");
            assertEquals(1, notified[0], "Other open tables are told to follow");
            both.doClick();
            assertEquals(1, notified[0], "Choosing the same view again changes nothing");

            TuningTableView reopened = new TuningTableView("VE", false, settings);
            assertTrue(findSegment(reopened.getContent(), "Both").isSelected(), "Another table keeps the choice");

            surface.doClick();
            assertTrue(view.is3DShown());
            assertFalse(preview.isVisible());
            assertTrue(settings.getBoolProperty(TuningTableView.PREVIEW_PROPERTY), "3D is a look, not remembered");
            view.show3D(false);
            assertTrue(both.isSelected(), "Leaving 3D returns to the remembered grid view");

            table.doClick();
            assertFalse(preview.isVisible());
            assertFalse(settings.getBoolProperty(TuningTableView.PREVIEW_PROPERTY));
            assertEquals(2, notified[0]);
        });
    }

    /** [tag:table_window_fit] Without the preview the grid asks for its rows' height, not more. */
    @Test
    void gridAsksForItsRowsHeightNotTheDefaultOrThe3DView() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("VE");
            loadTable(view, true);
            JTable table = view.getTableForTests();
            JScrollPane grid = (JScrollPane) table.getParent().getParent();
            JPanel cards = (JPanel) view.getContent().getComponent(1);
            assertEquals(2 * table.getRowHeight(), table.getPreferredScrollableViewportSize().height,
                "Two rows, not JTable's default 400 px");
            Insets insets = cards.getInsets();
            assertEquals(grid.getParent().getPreferredSize().height + insets.top + insets.bottom,
                cards.getPreferredSize().height, "The grid card sets the height, not the larger 3D view");
            assertTrue(cards.getPreferredSize().height < 200);
            findSegment(view.getContent(), "3D").doClick();
            assertTrue(cards.getPreferredSize().height >= 400, "The 3D view keeps its own size");
        });
    }

    @Test
    void previewFollowsAvailableSpaceAndViewMode() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = previewOn("VE");
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
            JToggleButton surface = findSegment(view.getContent(), "3D");
            JToggleButton tableSegment = findSegment(view.getContent(), "Both");
            assertNotNull(surface);
            assertNotNull(tableSegment);
            assertTrue(tableSegment.isSelected(), "Tables open on the remembered grid view");
            surface.doClick();
            assertTrue(view.is3DShown());
            assertFalse(tableSegment.isSelected(), "Only one view is selected");
            assertFalse(preview.isVisible());
            assertFalse(findButton(view.getContent(), "Up").isEnabled(), "Grid editing is off while the grid is hidden");
            tableSegment.doClick();
            assertFalse(view.is3DShown());
            assertTrue(preview.isVisible());
            assertTrue(findButton(view.getContent(), "Up").isEnabled());
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

    @Test
    void narrowDialogScrollsTheGridSidewaysInsteadOfSqueezingValues() throws Exception, com.rusefi.ini.reader.IniParsingException {
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
            JTable table = view.getTableForTests();
            ScrollablePanel dialog = new ScrollablePanel();
            dialog.setLayout(new BoxLayout(dialog, BoxLayout.Y_AXIS));
            dialog.add(view.getContent());
            JScrollPane scroll = new JScrollPane(dialog);

            scroll.setSize(280, 700);
            invalidateTree(scroll);
            layoutTree(scroll);
            FontMetrics metrics = table.getFontMetrics(table.getFont());
            for (int column = 1; column < table.getColumnCount(); column++) {
                int width = table.getColumnModel().getColumn(column).getWidth();
                for (int row = 0; row < table.getRowCount(); row++) {
                    String text = String.valueOf(table.getValueAt(row, column));
                    assertTrue(metrics.stringWidth(text) < width, text + " must fit its " + width + " px column");
                }
            }
            JViewport gridViewport = (JViewport) table.getParent();
            assertTrue(table.getWidth() > gridViewport.getWidth(), "The narrow grid scrolls sideways");

            scroll.setSize(1600, 700);
            invalidateTree(scroll);
            layoutTree(scroll);
            assertEquals(gridViewport.getWidth(), table.getWidth(), "A wide grid stretches to fill the window");
        });
    }

    @Test
    void narrowDialogWrapsTheToolbarInsteadOfCuttingOffButtons() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuningTableView view = new TuningTableView("VE Table");
            loadTable(view, true);
            ScrollablePanel dialog = new ScrollablePanel();
            dialog.setLayout(new BoxLayout(dialog, BoxLayout.Y_AXIS));
            dialog.add(view.getContent());
            JScrollPane scroll = new JScrollPane(dialog);
            scroll.setSize(300, 700);
            for (int pass = 0; pass < 2; pass++) {
                // The second pass sees the toolbar's wrapped height for the actual width.
                invalidateTree(scroll);
                layoutTree(scroll);
            }
            JPanel toolbar = (JPanel) view.getContent().getComponent(0);
            for (String text : new String[]{"Table", "Both", "3D", "Up", "Down", "=", "H", "V", "Interpolate", "Smooth"}) {
                AbstractButton button = findButtonOfType(toolbar, text, AbstractButton.class);
                assertNotNull(button, text);
                Rectangle bounds = SwingUtilities.convertRectangle(button.getParent(), button.getBounds(), toolbar);
                assertTrue(bounds.x >= 0 && bounds.getMaxX() <= toolbar.getWidth(), text + " must be inside the toolbar: " + bounds);
                assertTrue(bounds.getMaxY() <= toolbar.getHeight(), text + " must not be cut off below: " + bounds);
                assertEquals(button.getPreferredSize().width, button.getWidth(), text + " keeps its full width");
            }
        });
    }

    private static TuningTableView previewOn(String title) {
        com.rusefi.core.preferences.storage.Node settings = new com.rusefi.core.preferences.storage.Node();
        settings.setBoolProperty(TuningTableView.PREVIEW_PROPERTY, true);
        return new TuningTableView(title, false, settings);
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

    private static JToggleButton findSegment(Container container, String text) {
        AbstractButton button = findButtonOfType(container, text, JToggleButton.class);
        return (JToggleButton) button;
    }

    private static AbstractButton findButton(Container container, String text) {
        return findButtonOfType(container, text, JButton.class);
    }

    private static AbstractButton findButtonOfType(Container container, String text, Class<? extends AbstractButton> type) {
        for (Component component : container.getComponents()) {
            if (type.isInstance(component) && text.equals(((AbstractButton) component).getText())) {
                return (AbstractButton) component;
            }
            if (component instanceof Container) {
                AbstractButton found = findButtonOfType((Container) component, text, type);
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
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(44);
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
