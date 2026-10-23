package com.rusefi.ui.wizard;

import com.formdev.flatlaf.FlatClientProperties;

import javax.swing.*;
import javax.swing.border.AbstractBorder;
import java.awt.*;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Path2D;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.function.IntConsumer;

final class WizardProgressPanel extends JPanel {
    enum State { COMPLETED, CURRENT, UPCOMING }

    static final class Item {
        final int catalogIndex;
        final int number;
        final String title;
        final State state;

        Item(int catalogIndex, int number, String title, State state) {
            this.catalogIndex = catalogIndex;
            this.number = number;
            this.title = title;
            this.state = state;
        }
    }

    private static final int SCROLL_AMOUNT = 160;
    private static final int DRAG_THRESHOLD = 4;

    private final JButton previous = new JButton("‹");
    private final JButton next = new JButton("›");
    private final JPanel track = new JPanel();
    private final JScrollPane scrollPane = new JScrollPane(track,
        ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER, ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER) {
        @Override
        public Dimension getPreferredSize() {
            Dimension preferred = super.getPreferredSize();
            return new Dimension(Math.min(preferred.width, 520), preferred.height);
        }
    };
    private final List<JButton> pills = new ArrayList<>();
    private List<Item> items = Collections.emptyList();
    private IntConsumer onCompletedSelected = ignored -> { };
    private boolean suppressSelection;
    private int dragStartX;
    private int dragStartViewX;

    WizardProgressPanel() {
        super(new BorderLayout(WizardStyle.GAP, 0));
        setOpaque(false);
        track.setOpaque(false);
        track.setLayout(new BoxLayout(track, BoxLayout.X_AXIS));
        scrollPane.setBorder(BorderFactory.createEmptyBorder());
        scrollPane.setOpaque(false);
        scrollPane.getViewport().setOpaque(false);

        previous.setToolTipText("Scroll to earlier setup steps");
        next.setToolTipText("Scroll to later setup steps");
        previous.getAccessibleContext().setAccessibleName("Scroll to earlier setup steps");
        next.getAccessibleContext().setAccessibleName("Scroll to later setup steps");
        for (JButton arrow : new JButton[]{previous, next}) {
            arrow.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
            arrow.setFont(arrow.getFont().deriveFont(Font.BOLD, arrow.getFont().getSize2D() * 1.6f));
            arrow.setForeground(WizardStyle.muted());
        }
        previous.setVisible(false);
        next.setVisible(false);
        previous.addActionListener(e -> scrollBy(-SCROLL_AMOUNT));
        next.addActionListener(e -> scrollBy(SCROLL_AMOUNT));

        add(previous, BorderLayout.WEST);
        add(scrollPane, BorderLayout.CENTER);
        add(next, BorderLayout.EAST);

        DragHandler dragHandler = new DragHandler();
        track.addMouseListener(dragHandler);
        track.addMouseMotionListener(dragHandler);
        scrollPane.getViewport().addMouseListener(dragHandler);
        scrollPane.getViewport().addMouseMotionListener(dragHandler);
        scrollPane.getViewport().addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                updateScrollControls();
            }
        });
    }

    void setOnCompletedSelected(IntConsumer callback) {
        onCompletedSelected = callback != null ? callback : ignored -> { };
    }

    void setItems(List<Item> newItems) {
        items = Collections.unmodifiableList(new ArrayList<>(newItems));
        pills.clear();
        track.removeAll();

        DragHandler dragHandler = new DragHandler();
        for (int i = 0; i < items.size(); i++) {
            Item item = items.get(i);
            JButton pill = createPill(item);
            pill.addMouseListener(dragHandler);
            pill.addMouseMotionListener(dragHandler);
            pills.add(pill);
            if (i > 0) {
                track.add(new Connector());
            }
            track.add(pill);
        }

        track.revalidate();
        track.repaint();
        revalidate();
        SwingUtilities.invokeLater(() -> {
            updateScrollControls();
            revealCurrent();
        });
    }

    private JButton createPill(Item item) {
        String status = item.state == State.COMPLETED ? "Done" :
            item.state == State.CURRENT ? "Current" : "Upcoming";
        JButton pill = new JButton(item.title, new StepIcon(item)) {
            @Override
            protected void paintComponent(Graphics g) {
                if (isEnabled() && (getModel().isRollover() || getModel().isPressed())) {
                    Graphics2D g2 = (Graphics2D) g.create();
                    try {
                        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                        g2.setColor(WizardStyle.color("Console.TitleBar.chipBackground", new Color(0xeef0f3)));
                        g2.fillRoundRect(0, 0, getWidth(), getHeight() - 2, WizardStyle.ARC, WizardStyle.ARC);
                    } finally {
                        g2.dispose();
                    }
                }
                super.paintComponent(g);
            }
        };
        pill.setHorizontalAlignment(SwingConstants.CENTER);
        pill.setIconTextGap(8);
        pill.setRolloverEnabled(true);
        // disabled pills (current and upcoming) take their text colour from the button style
        if (item.state == State.CURRENT) {
            pill.setFont(pill.getFont().deriveFont(Font.BOLD));
            pill.putClientProperty(FlatClientProperties.STYLE, "disabledText: " + hex(WizardStyle.accentText()));
        } else if (item.state == State.UPCOMING) {
            pill.putClientProperty(FlatClientProperties.STYLE, "disabledText: " + hex(WizardStyle.muted()));
        } else {
            pill.setForeground(WizardStyle.text());
        }
        pill.setEnabled(item.state == State.COMPLETED);
        pill.setFocusable(item.state == State.COMPLETED);
        if (item.state == State.COMPLETED) {
            pill.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
            pill.setToolTipText("Open completed step");
        }
        pill.setOpaque(false);
        pill.setContentAreaFilled(false);
        pill.setBorder(new PillBorder(item.state));
        pill.getAccessibleContext().setAccessibleName(item.number + ". " + item.title + ", " + status);
        pill.addActionListener(e -> {
            if (!suppressSelection && item.state == State.COMPLETED) {
                onCompletedSelected.accept(item.catalogIndex);
            }
            suppressSelection = false;
        });
        return pill;
    }

    private void scrollBy(int amount) {
        JViewport viewport = scrollPane.getViewport();
        int maximum = Math.max(0, viewport.getViewSize().width - viewport.getExtentSize().width);
        Point position = viewport.getViewPosition();
        position.x = Math.max(0, Math.min(maximum, position.x + amount));
        viewport.setViewPosition(position);
        updateScrollControls();
    }

    private void revealCurrent() {
        Dimension preferred = track.getPreferredSize();
        track.setSize(preferred);
        track.doLayout();
        for (int i = 0; i < items.size(); i++) {
            if (items.get(i).state == State.CURRENT) {
                Rectangle bounds = pills.get(i).getBounds();
                JViewport viewport = scrollPane.getViewport();
                int left = viewport.getViewPosition().x;
                int extentWidth = viewport.getExtentSize().width;
                if (previous.isVisible()) {
                    Insets insets = getInsets();
                    int postArrowWidth = getWidth() - insets.left - insets.right
                        - previous.getPreferredSize().width - next.getPreferredSize().width - 2 * WizardStyle.GAP;
                    extentWidth = Math.min(extentWidth, Math.max(0, postArrowWidth));
                }
                int right = left + extentWidth;
                if (bounds.x < left) {
                    viewport.setViewPosition(new Point(bounds.x, 0));
                } else if (bounds.x + bounds.width > right) {
                    int x = Math.max(0, bounds.x + bounds.width - extentWidth);
                    viewport.setViewPosition(new Point(x, 0));
                }
                updateScrollControls();
                return;
            }
        }
    }

    private void updateScrollControls() {
        JViewport viewport = scrollPane.getViewport();
        Insets insets = getInsets();
        int availableWithoutButtons = Math.max(0, getWidth() - insets.left - insets.right);
        boolean overflow = track.getPreferredSize().width > availableWithoutButtons;
        if (previous.isVisible() != overflow) {
            previous.setVisible(overflow);
            next.setVisible(overflow);
            revalidate();
        }
        int viewportWidth = viewport.getExtentSize().width;
        int maximum = Math.max(0, viewport.getViewSize().width - viewportWidth);
        int x = Math.min(viewport.getViewPosition().x, maximum);
        previous.setEnabled(overflow && x > 0);
        next.setEnabled(overflow && x < maximum);
    }

    @Override
    public void doLayout() {
        super.doLayout();
        updateScrollControls();
    }

    List<JButton> pillsForTests() {
        return Collections.unmodifiableList(pills);
    }

    JButton previousForTests() {
        return previous;
    }

    JButton nextForTests() {
        return next;
    }

    JViewport viewportForTests() {
        return scrollPane.getViewport();
    }

    private final class DragHandler extends MouseAdapter {
        @Override
        public void mousePressed(MouseEvent e) {
            dragStartX = SwingUtilities.convertPoint(e.getComponent(), e.getPoint(), scrollPane.getViewport()).x;
            dragStartViewX = scrollPane.getViewport().getViewPosition().x;
            suppressSelection = false;
        }

        @Override
        public void mouseDragged(MouseEvent e) {
            int x = SwingUtilities.convertPoint(e.getComponent(), e.getPoint(), scrollPane.getViewport()).x;
            int delta = dragStartX - x;
            if (Math.abs(delta) >= DRAG_THRESHOLD) {
                suppressSelection = true;
            }
            int current = scrollPane.getViewport().getViewPosition().x;
            scrollBy(dragStartViewX + delta - current);
        }
    }

    private static String hex(Color color) {
        return String.format("#%06x", color.getRGB() & 0xffffff);
    }

    /** Thin line joining neighbouring steps. */
    private static final class Connector extends JComponent {
        private static final Dimension SIZE = new Dimension(12, 1);

        @Override
        public Dimension getPreferredSize() {
            return SIZE;
        }

        @Override
        public Dimension getMaximumSize() {
            return new Dimension(SIZE.width, Integer.MAX_VALUE);
        }

        @Override
        protected void paintComponent(Graphics g) {
            g.setColor(WizardStyle.separator());
            int y = (getHeight() - 2) / 2;
            g.drawLine(2, y, getWidth() - 3, y);
        }
    }

    /** Step marker: a green tick when done, a filled accent number when current, an outlined number otherwise. */
    private static final class StepIcon implements Icon {
        private static final int SIZE = 22;
        private final Item item;

        private StepIcon(Item item) {
            this.item = item;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                g2.translate(x, y);
                Ellipse2D circle = new Ellipse2D.Float(0.5f, 0.5f, SIZE - 1, SIZE - 1);
                if (item.state == State.COMPLETED) {
                    g2.setColor(WizardStyle.successBackground());
                    g2.fill(circle);
                    g2.setColor(WizardStyle.success());
                    g2.setStroke(new BasicStroke(2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                    Path2D check = new Path2D.Float();
                    check.moveTo(6.5, 11.5);
                    check.lineTo(9.5, 14.5);
                    check.lineTo(15.5, 8);
                    g2.draw(check);
                    return;
                }
                Color numberColor;
                if (item.state == State.CURRENT) {
                    g2.setColor(WizardStyle.accent());
                    g2.fill(circle);
                    numberColor = Color.WHITE;
                } else {
                    g2.setColor(WizardStyle.border());
                    g2.draw(circle);
                    numberColor = WizardStyle.muted();
                }
                g2.setColor(numberColor);
                g2.setFont(c.getFont().deriveFont(Font.BOLD, c.getFont().getSize2D() * 0.85f));
                FontMetrics fm = g2.getFontMetrics();
                String number = String.valueOf(item.number);
                g2.drawString(number, (SIZE - fm.stringWidth(number)) / 2f,
                    (SIZE + fm.getAscent() - fm.getDescent()) / 2f);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return SIZE;
        }

        @Override
        public int getIconHeight() {
            return SIZE;
        }
    }

    /** Padding around a step, an accent underline under the current one and a focus ring. */
    private static final class PillBorder extends AbstractBorder {
        private static final int UNDERLINE = 2;
        private final State state;

        private PillBorder(State state) {
            this.state = state;
        }

        @Override
        public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                if (state == State.CURRENT) {
                    g2.setColor(WizardStyle.accent());
                    g2.fillRect(x + 4, y + height - UNDERLINE, width - 8, UNDERLINE);
                }
                if (c.hasFocus()) {
                    g2.setColor(WizardStyle.focus());
                    g2.setStroke(new BasicStroke(2f));
                    g2.drawRoundRect(x + 1, y + 1, width - 3, height - 5, WizardStyle.ARC, WizardStyle.ARC);
                }
            } finally {
                g2.dispose();
            }
        }

        @Override
        public Insets getBorderInsets(Component c) {
            return new Insets(8, 6, 8 + UNDERLINE, 8);
        }
    }
}
