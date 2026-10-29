package com.rusefi.ui.console;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class TabReorderTest {
    private static final List<String> DEFAULT = Arrays.asList("Gauges", "Tuning", "Lua Scripting", "Pinout");

    @Test
    void movingATabKeepsItsTitleContentAndTheSelection() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TabbedPanel panel = panel();
            JTabbedPane pane = panel.tabbedPane;
            Component tuning = pane.getComponentAt(1);
            Component tuningTitle = pane.getTabComponentAt(1);
            pane.setSelectedComponent(tuning);
            List<Boolean> reorderingSeen = new ArrayList<>();
            pane.addChangeListener(e -> reorderingSeen.add(panel.isReordering()));

            panel.moveTab(1, 3);
            assertEquals(Arrays.asList("Gauges", "Lua Scripting", "Pinout", "Tuning"), titles(pane));
            assertSame(tuning, pane.getComponentAt(3));
            assertSame(tuningTitle, pane.getTabComponentAt(3), "the bold-when-selected title moves with it");
            assertSame(tuning, pane.getSelectedComponent(), "the moved tab stays selected");
            assertFalse(reorderingSeen.isEmpty());
            assertTrue(reorderingSeen.stream().allMatch(seen -> seen),
                "selection listeners can tell the move from a tab switch");
            assertFalse(panel.isReordering());
        });
    }

    @Test
    void draggingATabPastAnotherMovesItThereAndAClickDoesNot() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            TabbedPanel panel = panel();
            TabReorder reorder = new TabReorder(panel, config);
            JTabbedPane pane = panel.tabbedPane;

            Point gauges = centre(pane, 0);
            reorder.pressed(gauges, true);
            reorder.dragged(new Point(gauges.x + 2, gauges.y));
            reorder.released();
            assertEquals(DEFAULT, titles(pane), "a click, even with a small twitch, moves nothing");

            Rectangle pinout = pane.getBoundsAt(3);
            reorder.pressed(gauges, true);
            reorder.dragged(new Point(pinout.x + pinout.width - 2, gauges.y));
            assertTrue(reorder.isDragging());
            reorder.released();
            assertEquals(Arrays.asList("Tuning", "Lua Scripting", "Pinout", "Gauges"), titles(pane),
                "dropped past the middle of the last tab: it goes last");
            assertEquals("Tuning|Lua Scripting|Pinout|Gauges", config.getProperty(TabReorder.ORDER_KEY));

            Point lua = centre(pane, 1);
            reorder.pressed(lua, false);
            reorder.dragged(new Point(0, lua.y));
            reorder.released();
            assertEquals(Arrays.asList("Tuning", "Lua Scripting", "Pinout", "Gauges"), titles(pane),
                "only the left button drags");
        });
    }

    @Test
    void mouseEventsOnTheTabRowReachTheReorderThroughTheLookAndFeelsTabComponent() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            TabbedPanel panel = panel();
            panel.enableReordering(config);
            JTabbedPane pane = panel.tabbedPane;
            Point from = centre(pane, 3);
            Point to = new Point(pane.getBoundsAt(0).x + 2, from.y);
            // the scroll tab layout puts the tabs in a child component, which receives the presses
            Component target = SwingUtilities.getDeepestComponentAt(pane, from.x, from.y);
            assertNotNull(target);
            send(pane, target, MouseEvent.MOUSE_PRESSED, from);
            send(pane, target, MouseEvent.MOUSE_DRAGGED, to);
            send(pane, target, MouseEvent.MOUSE_RELEASED, to);
            assertEquals(Arrays.asList("Pinout", "Gauges", "Tuning", "Lua Scripting"), titles(pane));

            Component content = pane.getComponentAt(1);
            Point inside = SwingUtilities.convertPoint(pane, new Point(pane.getWidth() / 2, pane.getHeight() - 20), content);
            content.dispatchEvent(new MouseEvent(content, MouseEvent.MOUSE_PRESSED, 0,
                InputEvent.BUTTON1_DOWN_MASK, inside.x, inside.y, 1, false, MouseEvent.BUTTON1));
            content.dispatchEvent(new MouseEvent(content, MouseEvent.MOUSE_DRAGGED, 0,
                InputEvent.BUTTON1_DOWN_MASK, 0, inside.y, 0, false, MouseEvent.NOBUTTON));
            content.dispatchEvent(new MouseEvent(content, MouseEvent.MOUSE_RELEASED, 0, 0, 0, inside.y, 1, false,
                MouseEvent.BUTTON1));
            assertEquals(Arrays.asList("Pinout", "Gauges", "Tuning", "Lua Scripting"), titles(pane),
                "dragging inside a tab's page does not move tabs");
        });
    }

    private static void send(JTabbedPane pane, Component target, int id, Point inPane) {
        Point p = SwingUtilities.convertPoint(pane, inPane, target);
        int modifiers = id == MouseEvent.MOUSE_RELEASED ? 0 : InputEvent.BUTTON1_DOWN_MASK;
        int button = id == MouseEvent.MOUSE_DRAGGED ? MouseEvent.NOBUTTON : MouseEvent.BUTTON1;
        target.dispatchEvent(new MouseEvent(target, id, 0, modifiers, p.x, p.y, 1, false, button));
    }

    @Test
    void resetPutsTheTabsBackInTheConsolesOwnOrder() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            config.setProperty(TabReorder.ORDER_KEY, "Pinout|Gauges");
            TabbedPanel panel = panel();
            TabReorder reorder = new TabReorder(panel, config);
            reorder.applySavedOrder();
            JTabbedPane pane = panel.tabbedPane;
            assertEquals(Arrays.asList("Pinout", "Gauges", "Tuning", "Lua Scripting"), titles(pane));
            Component selected = pane.getComponentAt(2);
            pane.setSelectedComponent(selected);
            // a tool tab opened later does not make the order look changed
            panel.addTab("Messages", new JPanel());

            JMenuItem reset = (JMenuItem) reorder.createMenu().getComponent(0);
            assertEquals(TabReorder.RESET, reset.getText());
            assertTrue(reset.isEnabled());
            reset.doClick(0);
            assertEquals(Arrays.asList("Gauges", "Tuning", "Lua Scripting", "Pinout", "Messages"), titles(pane),
                "the order the console built, with later tabs after it");
            assertNull(config.getProperty(TabReorder.ORDER_KEY, null), "the dragged order is forgotten");
            assertSame(selected, pane.getSelectedComponent(), "the selected tab stays selected");
            assertFalse(((JMenuItem) reorder.createMenu().getComponent(0)).isEnabled(),
                "nothing to reset in the console's own order");
        });
    }

    @Test
    void rightClickingATabOpensTheMenu() throws Exception {
        org.junit.jupiter.api.Assumptions.assumeFalse(GraphicsEnvironment.isHeadless());
        JFrame[] frame = new JFrame[1];
        try {
            SwingUtilities.invokeAndWait(() -> {
                TabbedPanel panel = panel();
                panel.enableReordering(new Node());
                panel.applySavedOrder();
                frame[0] = new JFrame();
                frame[0].add(panel.getContent());
                frame[0].setSize(900, 300);
                frame[0].setVisible(true);
                frame[0].validate();
                JTabbedPane pane = panel.tabbedPane;
                Point tab = centre(pane, 1);
                Component target = SwingUtilities.getDeepestComponentAt(pane, tab.x, tab.y);
                Point p = SwingUtilities.convertPoint(pane, tab, target);
                // both, as platforms differ in which one is the menu trigger
                target.dispatchEvent(new MouseEvent(target, MouseEvent.MOUSE_PRESSED, 0, InputEvent.BUTTON3_DOWN_MASK,
                    p.x, p.y, 1, true, MouseEvent.BUTTON3));
                target.dispatchEvent(new MouseEvent(target, MouseEvent.MOUSE_RELEASED, 0, 0,
                    p.x, p.y, 1, true, MouseEvent.BUTTON3));
                MenuElement[] open = MenuSelectionManager.defaultManager().getSelectedPath();
                assertTrue(open.length > 0 && open[0] instanceof JPopupMenu, "the tab menu is open");
                JMenuItem reset = (JMenuItem) ((JPopupMenu) open[0]).getComponent(0);
                assertEquals(TabReorder.RESET, reset.getText());
                assertFalse(reset.isEnabled(), "already in the console's own order");
                MenuSelectionManager.defaultManager().clearSelectedPath();
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                if (frame[0] != null) {
                    frame[0].dispose();
                }
            });
        }
    }

    @Test
    void theSavedOrderIsRestoredAndTabsItDoesNotKnowFollowIt() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            config.setProperty(TabReorder.ORDER_KEY, "Pinout|Messages|Gauges");
            TabbedPanel panel = panel();
            panel.enableReordering(config);
            panel.applySavedOrder();
            assertEquals(Arrays.asList("Pinout", "Gauges", "Tuning", "Lua Scripting"), titles(panel.tabbedPane),
                "a closed tool tab in the saved order is skipped; unknown tabs keep their order after");
        });
    }

    private static TabbedPanel panel() {
        TabbedPanel panel = new TabbedPanel(mock(UIContext.class));
        for (String title : DEFAULT) {
            panel.addTab(title, new JPanel());
        }
        panel.tabbedPane.setSize(900, 300);
        panel.tabbedPane.doLayout();
        return panel;
    }

    private static Point centre(JTabbedPane pane, int index) {
        Rectangle bounds = pane.getBoundsAt(index);
        return new Point(bounds.x + bounds.width / 2, bounds.y + bounds.height / 2);
    }

    private static List<String> titles(JTabbedPane pane) {
        List<String> titles = new ArrayList<>();
        for (int i = 0; i < pane.getTabCount(); i++) {
            titles.add(pane.getTitleAt(i));
        }
        return titles;
    }
}
