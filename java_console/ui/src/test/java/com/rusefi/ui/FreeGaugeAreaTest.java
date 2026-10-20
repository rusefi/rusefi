package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.geom.Rectangle2D;

import static com.rusefi.ui.GaugesPanelLayoutTest.createContext;
import static com.rusefi.ui.GaugesPanelLayoutTest.hasDemand;
import static org.junit.jupiter.api.Assertions.*;

class FreeGaugeAreaTest {
    private static final double EPS = 1e-9;

    @Test
    void editLayoutOverlayTakesInputAndTheToolbarFollows() throws Exception {
        Node config = twoColumnGrid();
        onPanel(config, panel -> {
            assertFalse(panel.getEditLayoutItem().isEnabled(), "Editing is a free-layout feature");
            assertFalse(panel.getAddGaugeItem().isEnabled());
            assertFalse(panel.getDoneEditingButton().isVisible());
            panel.setFreeMode(true);
            assertTrue(panel.getEditLayoutItem().isEnabled());
            assertTrue(panel.getAddGaugeItem().isEnabled());
            assertFalse(panel.getDoneEditingButton().isVisible(), "Done only appears while arranging");

            panel.getEditLayoutItem().doClick(0);
            FreeGaugeArea area = panel.getFreeArea();
            JPanel free = panel.getFreePanel();
            layout(free);
            assertTrue(area.isEditing());
            assertTrue(panel.getDoneEditingButton().isVisible());
            assertFalse(panel.getEditLayoutItem().isEnabled(), "Already arranging");
            assertSame(area.getEditor(), SwingUtilities.getDeepestComponentAt(free, 100, 100),
                "Gauges cannot be clicked while editing");

            key(area.getEditor(), KeyEvent.VK_ESCAPE, 0);
            assertFalse(area.isEditing(), "Esc finishes editing");
            assertFalse(panel.getDoneEditingButton().isVisible());
            assertTrue(panel.getEditLayoutItem().isEnabled());
            assertNotSame(area.getEditor(), SwingUtilities.getDeepestComponentAt(free, 100, 100));

            panel.getEditLayoutItem().doClick(0);
            panel.getDoneEditingButton().doClick(0);
            assertFalse(area.isEditing(), "Done finishes editing");

            panel.getEditLayoutItem().doClick(0);
            panel.setFreeMode(false);
            assertFalse(area.isEditing(), "Leaving Free mode ends editing");
            assertFalse(panel.getEditLayoutItem().isEnabled());
            assertFalse(panel.getDoneEditingButton().isVisible());
        });
    }

    @Test
    void dragMovesAndResizesWithSnappingInsideTheArea() throws Exception {
        Node config = twoColumnGrid();
        onPanel(config, panel -> {
            FreeGaugeArea area = startEditing(panel);
            FreeGaugeArea.Editor editor = area.getEditor();
            FreeGaugeArea.Item gauge = area.getItem(0);

            mouse(editor, MouseEvent.MOUSE_PRESSED, 100, 100);
            assertSame(gauge, area.getSelected());
            mouse(editor, MouseEvent.MOUSE_DRAGGED, 141, 100);
            assertBounds(0.10, 0, 0.5, 1, area.boundsOf(gauge), "41 px of 400 snaps to 10%");
            assertBounds(0.10, 0, 0.5, 1, GaugeLayout.getBounds(config.getChild("free_0")), "Moves are saved");
            mouse(editor, MouseEvent.MOUSE_DRAGGED, 2000, -500);
            assertBounds(0.5, 0, 0.5, 1, area.boundsOf(gauge), "A gauge cannot leave the area");
            mouse(editor, MouseEvent.MOUSE_RELEASED, 2000, -500);

            // The resize handle of the selected gauge wins even where it overlaps another gauge.
            Rectangle handle = editor.resizeHandle(gauge);
            Point start = new Point(handle.x + handle.width / 2, handle.y + handle.height / 2);
            mouse(editor, MouseEvent.MOUSE_PRESSED, start.x, start.y);
            mouse(editor, MouseEvent.MOUSE_DRAGGED, start.x - 100, start.y - 100);
            assertBounds(0.5, 0, 0.25, 0.5, area.boundsOf(gauge), "Corner drag resizes");
            mouse(editor, MouseEvent.MOUSE_DRAGGED, -1000, -1000);
            assertBounds(0.5, 0, FreeGaugeArea.MIN_EDIT_SIZE, FreeGaugeArea.MIN_EDIT_SIZE,
                area.boundsOf(gauge), "Resizing stops at the minimum size");
            mouse(editor, MouseEvent.MOUSE_RELEASED, -1000, -1000);
            assertBounds(0.5, 0, FreeGaugeArea.MIN_EDIT_SIZE, FreeGaugeArea.MIN_EDIT_SIZE,
                GaugeLayout.getBounds(config.getChild("free_0")), "Resizes are saved");

            key(editor, KeyEvent.VK_RIGHT, 0);
            assertBounds(0.51, 0, FreeGaugeArea.MIN_EDIT_SIZE, FreeGaugeArea.MIN_EDIT_SIZE, area.boundsOf(gauge),
                "Arrow keys nudge by 1%");
            key(editor, KeyEvent.VK_DOWN, InputEvent.SHIFT_DOWN_MASK);
            assertBounds(0.51, 0.05, FreeGaugeArea.MIN_EDIT_SIZE, FreeGaugeArea.MIN_EDIT_SIZE, area.boundsOf(gauge),
                "Shift nudges by 5%");

            mouse(editor, MouseEvent.MOUSE_PRESSED, 5, 195);
            assertNull(area.getSelected(), "Clicking empty space clears the selection");
        });
    }

    @Test
    void stackingAndRemovalKeepTheConfigurationInDrawingOrder() throws Exception {
        Node config = twoColumnGrid();
        onPanel(config, panel -> {
            panel.setActive(true);
            FreeGaugeArea area = startEditing(panel);
            FreeGaugeArea.Editor editor = area.getEditor();
            FreeGaugeArea.Item custom = area.getItem(0);
            FreeGaugeArea.Item rpm = area.getItem(1);
            JPopupMenu arrange = editor.createArrangeMenu(custom);
            assertEquals("Bring to front", ((JMenuItem) arrange.getComponent(0)).getText());
            assertTrue(arrange.getComponent(0).isEnabled());
            assertFalse(arrange.getComponent(1).isEnabled(), "The bottom gauge cannot go further back");

            ((JMenuItem) arrange.getComponent(0)).doClick(0);
            assertSame(custom, area.getItem(1));
            assertEquals("CustomGauge", config.getChild("free_1").getProperty("gauge"), "Topmost is saved last");
            assertEquals("RPMGauge", config.getChild("free_0").getProperty("gauge"));
            JPanel free = panel.getFreePanel();
            assertSame(area.getEditor(), free.getComponent(0), "The editor stays above every gauge");
            assertSame(custom.element.getContent(), free.getComponent(1));

            area.sendToBack(custom);
            assertSame(custom, area.getItem(0));
            assertSame(rpm.element.getContent(), free.getComponent(1));

            // The cross of the selected gauge removes it, its subscription and its saved slot.
            mouse(editor, MouseEvent.MOUSE_PRESSED, 100, 100);
            assertSame(custom, area.getSelected());
            Rectangle close = editor.closeButton(custom);
            mouse(editor, MouseEvent.MOUSE_PRESSED, close.x + close.width / 2, close.y + close.height / 2);
            assertEquals(1, area.getGaugeCount());
            assertEquals(1, config.getIntProperty("free_count", 0));
            assertEquals("RPMGauge", config.getChild("free_0").getProperty("gauge"));
            assertFalse(config.getConfig().containsKey("free_1"));
            assertFalse(hasDemand("layoutCustomChannel"));

            mouse(editor, MouseEvent.MOUSE_PRESSED, 300, 100);
            key(editor, KeyEvent.VK_DELETE, 0);
            assertEquals(0, area.getGaugeCount(), "Delete removes the selected gauge");
            assertEquals(0, config.getIntProperty("free_count", -1));
        });
    }

    @Test
    void addedGaugesGoToTheFirstEmptySpotOnTop() throws Exception {
        Node config = twoColumnGrid();
        onPanel(config, panel -> {
            FreeGaugeArea area = startEditing(panel);
            // The two copied grid cells fill the area: the new gauge goes in the middle, on top.
            area.addGauge("CustomGauge");
            FreeGaugeArea.Item added = area.getItem(2);
            assertSame(added, area.getSelected(), "A gauge added while editing is selected");
            // 400x200 px: a quarter of the width is half of the height, square on screen.
            assertBounds(0.375, 0.25, 0.25, 0.5, area.boundsOf(added), "No room: centered");
            assertEquals(3, config.getIntProperty("free_count", 0));
            assertEquals("CustomGauge", config.getChild("free_2").getProperty("gauge"));
            assertSame(added.element.getContent(), panel.getFreePanel().getComponent(1));

            area.removeGauge(area.getItem(0));
            area.removeGauge(area.getItem(0));
            area.removeGauge(area.getItem(0));
            area.addGauge("RPMGauge");
            assertBounds(0, 0, 0.25, 0.5, area.boundsOf(area.getItem(0)), "An empty area starts top left");
            area.addGauge("RPMGauge");
            assertBounds(0.25, 0, 0.25, 0.5, area.boundsOf(area.getItem(1)), "Next to the first gauge");

            while (area.canAddGauge()) {
                area.addGauge("RPMGauge");
            }
            assertEquals(GaugeLayout.MAX_FREE_GAUGES, area.getGaugeCount());
            JPopupMenu full = panel.createAddGaugeMenu();
            assertFalse(full.getComponent(0).isEnabled(), "A full layout explains instead of offering gauges");
        });
    }

    private interface PanelTest {
        void run(GaugesPanel panel) throws Exception;
    }

    private static void onPanel(Node config, PanelTest test) throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            GaugesPanel panel = new GaugesPanel(createContext(), config);
            try {
                test.run(panel);
            } catch (Exception e) {
                throw new AssertionError(e);
            } finally {
                panel.destroy();
            }
        });
    }

    private static FreeGaugeArea startEditing(GaugesPanel panel) {
        panel.setFreeMode(true);
        panel.getEditLayoutItem().doClick(0);
        layout(panel.getFreePanel());
        return panel.getFreeArea();
    }

    private static void layout(JPanel free) {
        free.setSize(400, 200);
        free.doLayout();
    }

    private static void mouse(Component target, int id, int x, int y) {
        int modifiers = id == MouseEvent.MOUSE_RELEASED ? 0 : InputEvent.BUTTON1_DOWN_MASK;
        target.dispatchEvent(new MouseEvent(target, id, System.currentTimeMillis(), modifiers, x, y, 1, false,
            id == MouseEvent.MOUSE_DRAGGED ? MouseEvent.NOBUTTON : MouseEvent.BUTTON1));
    }

    private static void key(Component target, int keyCode, int modifiers) {
        // AWT routes dispatched key events to the focus owner, and a headless test has none.
        KeyEvent event = new KeyEvent(target, KeyEvent.KEY_PRESSED, System.currentTimeMillis(), modifiers,
            keyCode, KeyEvent.CHAR_UNDEFINED);
        for (KeyListener listener : target.getKeyListeners()) {
            listener.keyPressed(event);
        }
    }

    private static void assertBounds(double x, double y, double width, double height, Rectangle2D.Double actual,
                                     String message) {
        assertEquals(x, actual.x, EPS, message + " (x)");
        assertEquals(y, actual.y, EPS, message + " (y)");
        assertEquals(width, actual.width, EPS, message + " (width)");
        assertEquals(height, actual.height, EPS, message + " (height)");
    }

    private static Node twoColumnGrid() {
        Node config = new Node();
        config.setProperty("gauges_rows", 1);
        config.setProperty("gauges_cols", 2);
        config.getChild("element_0").setProperty("gauge", "CustomGauge");
        config.getChild("element_1").setProperty("gauge", "RPMGauge");
        return config;
    }
}
