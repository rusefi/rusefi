package com.rusefi.ui.startup;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Path2D;
import java.util.List;

/**
 * Collapsible "Connection help" row: a header with a chevron and optional tool buttons (Device
 * Manager, driver install), and the troubleshooting tips underneath once expanded.
 * Collapsed by default. Colours from the theme files [tag:console_theme].
 */
public class ConnectionHelpSection extends JPanel {
    private static final int ARC = 10;
    private final JPanel body;
    private final JButton toggle;

    public ConnectionHelpSection(List<String> tips, List<Component> tools) {
        super(new BorderLayout());
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(6, 8, 6, 8));

        toggle = new JButton("Connection help", new ChevronIcon(false));
        toggle.setIconTextGap(12);
        toggle.setContentAreaFilled(false);
        toggle.setBorderPainted(false);
        toggle.setFocusPainted(false);
        toggle.setHorizontalAlignment(SwingConstants.LEFT);
        toggle.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        Font font = UIManager.getFont("Button.font");
        if (font != null) {
            toggle.setFont(font.deriveFont(Font.BOLD, font.getSize() + 1f));
        }
        toggle.getAccessibleContext().setAccessibleDescription("Show or hide connection troubleshooting tips");

        JPanel header = new JPanel(new BorderLayout());
        header.setOpaque(false);
        header.add(toggle, BorderLayout.CENTER);
        JPanel toolRow = new JPanel(new FlowLayout(FlowLayout.RIGHT, 8, 0));
        toolRow.setOpaque(false);
        for (Component tool : tools) {
            toolRow.add(tool);
        }
        header.add(toolRow, BorderLayout.EAST);
        add(header, BorderLayout.NORTH);

        body = new JPanel();
        body.setLayout(new BoxLayout(body, BoxLayout.Y_AXIS));
        body.setOpaque(false);
        body.setBorder(BorderFactory.createEmptyBorder(4, 40, 8, 8));
        Color muted = StartupCard.themed("Console.TitleBar.mutedForeground", Color.GRAY);
        for (String tip : tips) {
            JLabel line = new JLabel("•  " + tip);
            line.setForeground(muted);
            line.setBorder(BorderFactory.createEmptyBorder(3, 0, 3, 0));
            body.add(line);
        }
        body.setVisible(false);
        add(body, BorderLayout.CENTER);

        toggle.addActionListener(e -> setExpanded(!body.isVisible()));
    }

    public void setExpanded(boolean expanded) {
        body.setVisible(expanded);
        toggle.setIcon(new ChevronIcon(expanded));
        revalidate();
        repaint();
    }

    public boolean isExpanded() {
        return body.isVisible();
    }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(StartupCard.CONTENT_WIDTH, super.getPreferredSize().height);
    }

    @Override
    public Dimension getMaximumSize() {
        return getPreferredSize();
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(StartupCard.themed("Console.Card.sectionBackground", new Color(0xf7f8fa)));
            g2.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
            g2.setColor(StartupCard.themed("Console.Card.borderColor", new Color(0xdfe3e9)));
            g2.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
        } finally {
            g2.dispose();
        }
    }

    /** Right-pointing chevron when collapsed, down-pointing when expanded. */
    private static class ChevronIcon implements Icon {
        private final boolean expanded;

        ChevronIcon(boolean expanded) {
            this.expanded = expanded;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(c.getForeground());
                g2.setStroke(new BasicStroke(1.8f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                Path2D p = new Path2D.Float();
                if (expanded) {
                    p.moveTo(x + 2, y + 5);
                    p.lineTo(x + 6, y + 9);
                    p.lineTo(x + 10, y + 5);
                } else {
                    p.moveTo(x + 4, y + 2);
                    p.lineTo(x + 8, y + 6);
                    p.lineTo(x + 4, y + 10);
                }
                g2.draw(p);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return 12;
        }

        @Override
        public int getIconHeight() {
            return 12;
        }
    }
}
