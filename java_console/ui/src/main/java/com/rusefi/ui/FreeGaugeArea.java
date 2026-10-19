package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.List;

/**
 * The gauges of the free layout and their editor.
 * <p>
 * Gauges are kept bottom to top, matching their {@code free_N} order in the configuration.
 * While editing, a transparent overlay above every gauge takes all mouse and key input:
 * drag to move, drag the corner handle to resize, click the cross to remove. Gauges never
 * see those clicks, so editing cannot change a gauge by accident. The overlay lives inside
 * the gauge area rather than on the frame's glass pane, which other console code owns.
 */
final class FreeGaugeArea {
    /** Edits snap to this fraction of the gauge area. */
    static final double SNAP = 0.01;
    /** Smallest size reachable by resizing, as a fraction of the gauge area. */
    static final double MIN_EDIT_SIZE = 0.05;
    /** Width of a newly added gauge, as a fraction of the gauge area. */
    static final double NEW_GAUGE_WIDTH = 0.25;
    static final int HANDLE = 14;
    static final int CLOSE = 20;
    private static final Color ACCENT = new Color(0x1E88E5);
    private static final Color OUTLINE = new Color(30, 136, 229, 140);
    private static final Color CLOSE_FILL = new Color(0xE53935);
    private static final Color GRID_LINE = new Color(0, 0, 0, 18);

    private final UIContext uiContext;
    private final Node config;
    private final FreeGaugeLayout layout = new FreeGaugeLayout();
    private final JPanel panel = new JPanel(layout) {
        @Override
        public boolean isOptimizedDrawingEnabled() {
            // Free gauges may overlap: a repainting lower gauge must not draw over the ones above it.
            return false;
        }
    };
    private final Editor editor = new Editor();
    private final List<Item> items = new ArrayList<>();
    private final Runnable editingChanged;
    private boolean active;
    private boolean editing;
    private Item selected;

    FreeGaugeArea(UIContext uiContext, Node config, Runnable editingChanged) {
        this.uiContext = uiContext;
        this.config = config;
        this.editingChanged = editingChanged;
    }

    JPanel getPanel() {
        return panel;
    }

    /** Recreates the gauges from the configuration. */
    void rebuild(List<String> defaultGauges) {
        destroy();
        if (editing) {
            panel.add(editor, new Rectangle2D.Double(0, 0, 1, 1), 0);
        }
        for (int i = 0; i < GaugeLayout.freeCount(config); i++) {
            String defaultGauge = defaultGauges.get(Math.min(i, defaultGauges.size() - 1));
            addItem(config.getChild(GaugeLayout.FREE_ELEMENT + i), defaultGauge);
        }
        refresh();
    }

    void destroy() {
        for (Item item : items) {
            item.element.destroy();
        }
        items.clear();
        selected = null;
        panel.removeAll();
    }

    void setActive(boolean active) {
        this.active = active;
        for (Item item : items) {
            item.element.setActive(active);
        }
    }

    boolean isEditing() {
        return editing;
    }

    void setEditing(boolean editing) {
        if (this.editing == editing) {
            return;
        }
        this.editing = editing;
        selected = null;
        if (editing) {
            panel.add(editor, new Rectangle2D.Double(0, 0, 1, 1), 0);
            editor.requestFocusInWindow();
        } else {
            panel.remove(editor);
        }
        refresh();
        editingChanged.run();
    }

    int getGaugeCount() {
        return items.size();
    }

    boolean canAddGauge() {
        return items.size() < GaugeLayout.MAX_FREE_GAUGES;
    }

    /** Adds a gauge on top, in the first free spot, and selects it while editing. */
    void addGauge(String gaugeName) {
        if (!canAddGauge()) {
            return;
        }
        Node node = config.getChild(GaugeLayout.FREE_ELEMENT + items.size());
        node.getConfig().clear();
        node.setProperty("gauge", gaugeName);
        GaugeLayout.setBounds(node, findFreeSpot());
        Item item = addItem(node, gaugeName);
        config.setProperty(GaugeLayout.FREE_COUNT, items.size());
        applyZOrder();
        if (editing) {
            selected = item;
        }
        refresh();
    }

    void removeGauge(Item item) {
        item.element.destroy();
        panel.remove(item.element.getContent());
        items.remove(item);
        if (selected == item) {
            selected = null;
        }
        renumber();
        refresh();
    }

    void bringToFront(Item item) {
        items.remove(item);
        items.add(item);
        renumber();
        applyZOrder();
        refresh();
    }

    void sendToBack(Item item) {
        items.remove(item);
        items.add(0, item);
        renumber();
        applyZOrder();
        refresh();
    }

    Item getItem(int index) {
        return items.get(index);
    }

    Item getSelected() {
        return selected;
    }

    Editor getEditor() {
        return editor;
    }

    Rectangle2D.Double boundsOf(Item item) {
        return layout.getBounds(item.element.getContent());
    }

    private Item addItem(Node node, String defaultGauge) {
        GaugesGridElement element = GaugesGridElement.create(uiContext, node, defaultGauge);
        element.setActive(active);
        // Transparent around the dial, so an overlapped gauge shows outside the circle.
        element.getContent().setOpaque(false);
        Item item = new Item(node, element);
        items.add(item);
        // Index 0 is painted last, so later gauges stay on top of earlier ones.
        // While editing, the overlay stays above every gauge.
        panel.add(element.getContent(), GaugeLayout.getBounds(node), editor.getParent() == panel ? 1 : 0);
        return item;
    }

    /** Stores the gauges as free_0 (bottom) to free_N-1 (top), dropping any key left over. */
    private void renumber() {
        for (int i = 0; i < items.size(); i++) {
            Node node = items.get(i).node;
            node.setPrefix(GaugeLayout.FREE_ELEMENT + i);
            config.getConfig().put(GaugeLayout.FREE_ELEMENT + i, node);
        }
        config.getConfig().remove(GaugeLayout.FREE_ELEMENT + items.size());
        config.setProperty(GaugeLayout.FREE_COUNT, items.size());
    }

    private void applyZOrder() {
        int top = 0;
        if (editing) {
            panel.setComponentZOrder(editor, 0);
            top = 1;
        }
        for (int i = 0; i < items.size(); i++) {
            panel.setComponentZOrder(items.get(i).element.getContent(), top + items.size() - 1 - i);
        }
    }

    private void refresh() {
        panel.revalidate();
        panel.repaint();
    }

    private void place(Item item, Rectangle2D.Double bounds) {
        layout.setBounds(item.element.getContent(), bounds);
        GaugeLayout.setBounds(item.node, bounds);
        refresh();
    }

    /** First spot, scanning rows from the top left, where a new gauge overlaps nothing. */
    private Rectangle2D.Double findFreeSpot() {
        double width = NEW_GAUGE_WIDTH;
        // Square on screen: round gauges use the smaller side.
        double aspect = panel.getWidth() > 0 && panel.getHeight() > 0
            ? (double) panel.getWidth() / panel.getHeight() : 1.5;
        double height = Math.min(1, width * aspect);
        for (int row = 0; row * 0.05 <= 1 - height + 1e-9; row++) {
            for (int column = 0; column * 0.05 <= 1 - width + 1e-9; column++) {
                Rectangle2D.Double candidate = new Rectangle2D.Double(column * 0.05, row * 0.05, width, height);
                boolean empty = true;
                for (Item item : items) {
                    Rectangle2D.Double other = boundsOf(item);
                    Rectangle2D.Double inset = new Rectangle2D.Double(other.x + 1e-6, other.y + 1e-6,
                        other.width - 2e-6, other.height - 2e-6);
                    if (inset.intersects(candidate)) {
                        empty = false;
                        break;
                    }
                }
                if (empty) {
                    return candidate;
                }
            }
        }
        return new Rectangle2D.Double((1 - width) / 2, (1 - height) / 2, width, height);
    }

    static double snap(double value) {
        return Math.round(value / SNAP) * SNAP;
    }

    private static double clamp(double value, double min, double max) {
        return Math.max(min, Math.min(max, value));
    }

    static final class Item {
        final Node node;
        final GaugesGridElement element;

        Item(Node node, GaugesGridElement element) {
            this.node = node;
            this.element = element;
        }
    }

    private enum Drag {NONE, MOVE, RESIZE}

    /** Transparent overlay that edits the gauges below it. */
    final class Editor extends JComponent {
        private Drag drag = Drag.NONE;
        private Point dragStart;
        private Rectangle2D.Double startBounds;

        Editor() {
            setOpaque(false);
            setFocusable(true);
            setToolTipText("Drag to move, drag the corner to resize, right-click to arrange; Esc when done");
            MouseAdapter mouse = new MouseAdapter() {
                @Override
                public void mousePressed(MouseEvent event) {
                    pressed(event);
                }

                @Override
                public void mouseDragged(MouseEvent event) {
                    dragged(event.getPoint());
                }

                @Override
                public void mouseReleased(MouseEvent event) {
                    drag = Drag.NONE;
                }

                @Override
                public void mouseMoved(MouseEvent event) {
                    setCursor(cursorAt(event.getPoint()));
                }
            };
            addMouseListener(mouse);
            addMouseMotionListener(mouse);
            addKeyListener(new KeyAdapter() {
                @Override
                public void keyPressed(KeyEvent event) {
                    key(event);
                }
            });
        }

        Rectangle pixels(Rectangle2D.Double fraction) {
            int left = (int) Math.round(fraction.x * getWidth());
            int top = (int) Math.round(fraction.y * getHeight());
            int right = (int) Math.round((fraction.x + fraction.width) * getWidth());
            int bottom = (int) Math.round((fraction.y + fraction.height) * getHeight());
            return new Rectangle(left, top, right - left, bottom - top);
        }

        Rectangle closeButton(Item item) {
            Rectangle r = pixels(boundsOf(item));
            return new Rectangle(r.x + r.width - CLOSE - 4, r.y + 4, CLOSE, CLOSE);
        }

        Rectangle resizeHandle(Item item) {
            Rectangle r = pixels(boundsOf(item));
            return new Rectangle(r.x + r.width - HANDLE, r.y + r.height - HANDLE, HANDLE, HANDLE);
        }

        /** Topmost gauge under the point. */
        private Item itemAt(Point point) {
            for (int i = items.size() - 1; i >= 0; i--) {
                if (pixels(boundsOf(items.get(i))).contains(point)) {
                    return items.get(i);
                }
            }
            return null;
        }

        private Cursor cursorAt(Point point) {
            if (selected != null && closeButton(selected).contains(point)) {
                return Cursor.getPredefinedCursor(Cursor.HAND_CURSOR);
            }
            if (selected != null && resizeHandle(selected).contains(point)) {
                return Cursor.getPredefinedCursor(Cursor.SE_RESIZE_CURSOR);
            }
            return Cursor.getPredefinedCursor(itemAt(point) == null ? Cursor.DEFAULT_CURSOR : Cursor.MOVE_CURSOR);
        }

        private void pressed(MouseEvent event) {
            requestFocusInWindow();
            Point point = event.getPoint();
            drag = Drag.NONE;
            if (SwingUtilities.isLeftMouseButton(event) && selected != null) {
                if (closeButton(selected).contains(point)) {
                    removeGauge(selected);
                    return;
                }
                if (resizeHandle(selected).contains(point)) {
                    startDrag(Drag.RESIZE, point);
                    return;
                }
            }
            selected = itemAt(point);
            repaint();
            if (selected == null) {
                return;
            }
            if (SwingUtilities.isLeftMouseButton(event)) {
                startDrag(Drag.MOVE, point);
            } else if (SwingUtilities.isRightMouseButton(event)) {
                createArrangeMenu(selected).show(this, point.x, point.y);
            }
        }

        private void startDrag(Drag mode, Point point) {
            drag = mode;
            dragStart = point;
            startBounds = boundsOf(selected);
        }

        void dragged(Point point) {
            if (drag == Drag.NONE || selected == null || getWidth() <= 0 || getHeight() <= 0) {
                return;
            }
            double dx = (double) (point.x - dragStart.x) / getWidth();
            double dy = (double) (point.y - dragStart.y) / getHeight();
            Rectangle2D.Double b = (Rectangle2D.Double) startBounds.clone();
            if (drag == Drag.MOVE) {
                b.x = clamp(snap(b.x + dx), 0, 1 - b.width);
                b.y = clamp(snap(b.y + dy), 0, 1 - b.height);
            } else {
                b.width = clamp(snap(b.x + b.width + dx), b.x + MIN_EDIT_SIZE, 1) - b.x;
                b.height = clamp(snap(b.y + b.height + dy), b.y + MIN_EDIT_SIZE, 1) - b.y;
            }
            place(selected, b);
        }

        private void key(KeyEvent event) {
            if (event.getKeyCode() == KeyEvent.VK_ESCAPE) {
                setEditing(false);
                return;
            }
            if (selected == null) {
                return;
            }
            double step = event.isShiftDown() ? 5 * SNAP : SNAP;
            Rectangle2D.Double b = boundsOf(selected);
            switch (event.getKeyCode()) {
                case KeyEvent.VK_DELETE:
                case KeyEvent.VK_BACK_SPACE:
                    removeGauge(selected);
                    return;
                case KeyEvent.VK_LEFT:
                    b.x -= step;
                    break;
                case KeyEvent.VK_RIGHT:
                    b.x += step;
                    break;
                case KeyEvent.VK_UP:
                    b.y -= step;
                    break;
                case KeyEvent.VK_DOWN:
                    b.y += step;
                    break;
                default:
                    return;
            }
            b.x = clamp(snap(b.x), 0, 1 - b.width);
            b.y = clamp(snap(b.y), 0, 1 - b.height);
            place(selected, b);
            event.consume();
        }

        JPopupMenu createArrangeMenu(Item item) {
            JPopupMenu menu = new JPopupMenu();
            JMenuItem front = new JMenuItem("Bring to front");
            front.addActionListener(e -> bringToFront(item));
            front.setEnabled(items.indexOf(item) < items.size() - 1);
            menu.add(front);
            JMenuItem back = new JMenuItem("Send to back");
            back.addActionListener(e -> sendToBack(item));
            back.setEnabled(items.indexOf(item) > 0);
            menu.add(back);
            menu.addSeparator();
            JMenuItem remove = new JMenuItem("Remove gauge");
            remove.addActionListener(e -> removeGauge(item));
            menu.add(remove);
            return menu;
        }

        @Override
        protected void paintComponent(Graphics graphics) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                // Faint 10% guides show that the area is being edited.
                g.setColor(GRID_LINE);
                for (int i = 1; i < 10; i++) {
                    int x = (int) Math.round(getWidth() * i / 10.0);
                    int y = (int) Math.round(getHeight() * i / 10.0);
                    g.drawLine(x, 0, x, getHeight());
                    g.drawLine(0, y, getWidth(), y);
                }
                g.setStroke(new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER, 10, new float[]{4, 4}, 0));
                g.setColor(OUTLINE);
                for (Item item : items) {
                    Rectangle r = pixels(boundsOf(item));
                    g.drawRect(r.x, r.y, r.width - 1, r.height - 1);
                }
                if (selected != null) {
                    Rectangle r = pixels(boundsOf(selected));
                    g.setColor(ACCENT);
                    g.setStroke(new BasicStroke(2));
                    g.drawRect(r.x + 1, r.y + 1, r.width - 2, r.height - 2);
                    Rectangle handle = resizeHandle(selected);
                    g.fillRect(handle.x, handle.y, handle.width, handle.height);
                    g.setColor(Color.WHITE);
                    g.drawLine(handle.x + 4, handle.y + handle.height - 3, handle.x + handle.width - 3, handle.y + 4);
                    Rectangle close = closeButton(selected);
                    g.setColor(CLOSE_FILL);
                    g.fill(new Ellipse2D.Double(close.x, close.y, close.width, close.height));
                    g.setColor(Color.WHITE);
                    g.drawLine(close.x + 6, close.y + 6, close.x + close.width - 6, close.y + close.height - 6);
                    g.drawLine(close.x + close.width - 6, close.y + 6, close.x + 6, close.y + close.height - 6);
                }
            } finally {
                g.dispose();
            }
        }
    }
}
