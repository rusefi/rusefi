package com.rusefi.ui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

/**
 * Gauge grid size picker in the Edit dashboard menu: hover previews a rows × columns range,
 * click applies it. Anti-aliased circular cells and a size caption; colours from the theme files
 * (Console.GridPicker.*) [tag:console_theme].
 */
public class SizeSelectorPanel extends JPanel {
    public static final int WIDTH = 5;
    public static final int HEIGHT = 3;
    private static final int CELL = 22;
    private static final int GAP = 4;

    private int selectedRow = 1;
    private int selectedColumn = 1;
    private final JLabel caption = new JLabel("", SwingConstants.CENTER);

    public SizeSelectorPanel(final SizeSelectorListener sizeSelectorListener) {
        super(new BorderLayout(0, 6));
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(8, 10, 8, 10));
        JPanel cells = new JPanel(new GridLayout(HEIGHT, WIDTH, GAP, GAP));
        cells.setOpaque(false);

        MouseListener listener = new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                Element selected = (Element) e.getSource();
                selectedColumn = selected.column;
                selectedRow = selected.row;
                updateCaption();
                repaint();
            }

            @Override
            public void mouseClicked(MouseEvent e) {
                // close the menu
                MenuSelectionManager.defaultManager().clearSelectedPath();
                Element selected = (Element) e.getSource();
                sizeSelectorListener.onSelected(selected.row, selected.column);
            }
        };

        for (int r = 1; r <= HEIGHT; r++) {
            for (int c = 1; c <= WIDTH; c++) {
                Element e = new Element(r, c);
                e.addMouseListener(listener);
                cells.add(e);
            }
        }
        add(cells, BorderLayout.CENTER);
        caption.setForeground(themed("Console.TitleBar.mutedForeground", Color.GRAY));
        add(caption, BorderLayout.SOUTH);
        updateCaption();
    }

    private void updateCaption() {
        caption.setText(selectedRow + " × " + selectedColumn);
    }

    // package-private for tests
    String getCaptionText() {
        return caption.getText();
    }

    class Element extends JComponent {
        private final int row;
        private final int column;

        public Element(int row, int column) {
            this.row = row;
            this.column = column;
            setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        }

        @Override
        public Dimension getPreferredSize() {
            return new Dimension(CELL, CELL);
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setRenderingHint(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_PURE);
                boolean isSelected = row <= selectedRow && column <= selectedColumn;
                float w = getWidth() - 1;
                float h = getHeight() - 1;
                java.awt.geom.Ellipse2D cell = new java.awt.geom.Ellipse2D.Float(0.75f, 0.75f, w - 0.5f, h - 0.5f);
                g2.setColor(isSelected
                    ? themed("Console.GridPicker.selectedBackground", new Color(0xdbe7fd))
                    : themed("Console.GridPicker.background", Color.WHITE));
                g2.fill(cell);
                g2.setStroke(new BasicStroke(isSelected ? 1.5f : 1f));
                g2.setColor(isSelected
                    ? themed("Console.GridPicker.selectedBorder", new Color(0x2563eb))
                    : themed("Component.borderColor", new Color(0xc9ced6)));
                g2.draw(cell);
            } finally {
                g2.dispose();
            }
        }
    }

    private static Color themed(String key, Color fallback) {
        Color c = UIManager.getColor(key);
        return c != null ? c : fallback;
    }

    interface SizeSelectorListener {
        void onSelected(int row, int column);
    }
}
