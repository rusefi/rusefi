package com.rusefi.ui.console;

import com.rusefi.core.preferences.storage.Node;

import javax.swing.*;
import java.awt.*;
import java.awt.dnd.DragSource;
import java.awt.event.AWTEventListener;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Lets the user drag the main tabs into their own order, which the console keeps between runs.
 * <p>
 * Pressing a tab and moving more than {@link #START_DISTANCE} pixels starts a drag; a line shows where the
 * tab will go, and it moves when the mouse is released. Moving there and back, or a plain click, changes
 * nothing. The order is stored by title: tabs missing from it (new ones, or tool tabs opened later) keep
 * their place after the stored ones. Right-clicking a tab offers {@link #RESET}, back to the console's own
 * order.
 * <p>
 * With the one-row (scroll) tab layout, presses on a tab reach a component inside the look and feel, not
 * the tab pane, so the mouse is followed through an application-wide listener that only acts on presses
 * that land on a tab.
 */
final class TabReorder {
    static final String ORDER_KEY = "main_tab_order";
    static final String RESET = "Reset tab order";
    private static final String SEPARATOR = "|";
    /** Larger than a twitch during a click, so a click never moves a tab. */
    static final int START_DISTANCE = Math.max(5, DragSource.getDragThreshold());
    private static final int LINE_WIDTH = 3;

    private final TabbedPanel tabs;
    private final Node config;
    private int dragged = -1;
    private Point pressedAt;
    private boolean dragging;
    /** Where the tab would be inserted, counted before it is removed; -1 when no line is shown. */
    private int insertBefore = -1;
    /** The console's own order, as the tabs were built before the stored order was applied. */
    private List<String> defaultOrder;

    // Package-private so tests drive it without the application-wide mouse listener.
    TabReorder(TabbedPanel tabs, Node config) {
        this.tabs = tabs;
        this.config = config;
    }

    static TabReorder install(TabbedPanel tabs, Node config) {
        TabReorder reorder = new TabReorder(tabs, config);
        AWTEventListener listener = event -> reorder.mouse((MouseEvent) event);
        Toolkit.getDefaultToolkit().addAWTEventListener(listener,
            AWTEvent.MOUSE_EVENT_MASK | AWTEvent.MOUSE_MOTION_EVENT_MASK);
        return reorder;
    }

    private void mouse(MouseEvent event) {
        JTabbedPane pane = tabs.tabbedPane;
        Component source = event.getComponent();
        if (source == null || !SwingUtilities.isDescendingFrom(source, pane)) {
            return;
        }
        Point point = SwingUtilities.convertPoint(source, event.getPoint(), pane);
        switch (event.getID()) {
            case MouseEvent.MOUSE_PRESSED:
                // the platform's menu trigger: press on macOS and Linux, release on Windows
                if (event.isPopupTrigger()) {
                    showMenu(point);
                } else {
                    pressed(point, SwingUtilities.isLeftMouseButton(event));
                }
                break;
            case MouseEvent.MOUSE_DRAGGED:
                dragged(point);
                break;
            case MouseEvent.MOUSE_RELEASED:
                if (event.isPopupTrigger()) {
                    showMenu(point);
                }
                released();
                break;
            default:
                break;
        }
    }

    void pressed(Point point, boolean leftButton) {
        cancel();
        int index = leftButton ? tabs.tabbedPane.indexAtLocation(point.x, point.y) : -1;
        if (index >= 0) {
            dragged = index;
            pressedAt = point;
        }
    }

    void dragged(Point point) {
        if (dragged < 0) {
            return;
        }
        if (!dragging && point.distance(pressedAt) < START_DISTANCE) {
            return;
        }
        dragging = true;
        tabs.tabbedPane.setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
        int before = insertionPoint(point.x);
        if (before != insertBefore) {
            insertBefore = before;
            tabs.tabbedPane.repaint();
        }
    }

    void released() {
        if (dragging && insertBefore >= 0) {
            int target = insertBefore > dragged ? insertBefore - 1 : insertBefore;
            if (target != dragged) {
                tabs.moveTab(dragged, target);
                save();
            }
        }
        cancel();
    }

    private void cancel() {
        boolean wasShowingLine = insertBefore >= 0;
        dragged = -1;
        pressedAt = null;
        dragging = false;
        insertBefore = -1;
        tabs.tabbedPane.setCursor(null);
        if (wasShowingLine) {
            tabs.tabbedPane.repaint();
        }
    }

    /** The tab the dragged one would go before: the first whose middle is right of {@code x}, or the count. */
    private int insertionPoint(int x) {
        JTabbedPane pane = tabs.tabbedPane;
        for (int i = 0; i < pane.getTabCount(); i++) {
            Rectangle bounds = pane.getBoundsAt(i);
            if (bounds != null && x < bounds.x + bounds.width / 2) {
                return i;
            }
        }
        return pane.getTabCount();
    }

    /** Draws the insertion line, unless dropping there would leave the tab where it is. */
    void paintLine(Graphics g) {
        JTabbedPane pane = tabs.tabbedPane;
        if (!dragging || insertBefore < 0 || insertBefore == dragged || insertBefore == dragged + 1
            || pane.getTabCount() == 0) {
            return;
        }
        Rectangle bounds = insertBefore < pane.getTabCount() ? pane.getBoundsAt(insertBefore)
            : pane.getBoundsAt(pane.getTabCount() - 1);
        if (bounds == null) {
            return;
        }
        int x = insertBefore < pane.getTabCount() ? bounds.x : bounds.x + bounds.width;
        Color line = UIManager.getColor("Component.accentColor");
        g.setColor(line != null ? line : pane.getForeground());
        g.fillRect(x - LINE_WIDTH / 2, bounds.y + 2, LINE_WIDTH, bounds.height - 4);
    }

    boolean isDragging() {
        return dragging;
    }

    private void save() {
        config.setProperty(ORDER_KEY, String.join(SEPARATOR, titles()));
    }

    /**
     * Puts the tabs in the stored order; tabs not in it follow the stored ones in their current order.
     * The order before that is remembered as the console's own, for {@link #RESET}.
     */
    void applySavedOrder() {
        defaultOrder = titles();
        String saved = config.getProperty(ORDER_KEY, "");
        if (saved.isEmpty()) {
            return;
        }
        applyOrder(Arrays.asList(saved.split(java.util.regex.Pattern.quote(SEPARATOR))));
    }

    /** The right-click menu on a tab, when one is under {@code point}. */
    private void showMenu(Point point) {
        JTabbedPane pane = tabs.tabbedPane;
        if (pane.isShowing() && pane.indexAtLocation(point.x, point.y) >= 0) {
            createMenu().show(pane, point.x, point.y);
        }
    }

    JPopupMenu createMenu() {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem reset = new JMenuItem(RESET);
        reset.setToolTipText("Put the tabs back in the console's own order");
        reset.setEnabled(defaultOrder != null && !isDefaultOrder());
        reset.addActionListener(e -> resetOrder());
        menu.add(reset);
        return menu;
    }

    /** Back to the console's own order; the stored order is forgotten. */
    void resetOrder() {
        config.getConfig().remove(ORDER_KEY);
        if (defaultOrder != null) {
            applyOrder(defaultOrder);
        }
    }

    /** Whether the tabs of the console's own order are in that order (tabs opened later do not count). */
    boolean isDefaultOrder() {
        List<String> current = titles();
        current.retainAll(defaultOrder);
        return current.equals(defaultOrder);
    }

    private List<String> titles() {
        JTabbedPane pane = tabs.tabbedPane;
        List<String> titles = new ArrayList<>();
        for (int i = 0; i < pane.getTabCount(); i++) {
            titles.add(pane.getTitleAt(i));
        }
        return titles;
    }

    private void applyOrder(List<String> order) {
        JTabbedPane pane = tabs.tabbedPane;
        int position = 0;
        for (String title : order) {
            int index = pane.indexOfTab(title);
            if (index >= 0) {
                if (index != position) {
                    tabs.moveTab(index, position);
                }
                position++;
            }
        }
    }
}
