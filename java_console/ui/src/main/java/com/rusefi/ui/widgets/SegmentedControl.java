package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;

/**
 * Mutually exclusive options drawn as one control: a grey track with the selected option raised
 * on a lighter background, e.g. the Grid / Free gauge layout switch. Callers keep their own
 * {@link ButtonGroup} and listeners; this only arranges and paints the toggles.
 * Colours: Console.Segmented.* theme keys [tag:console_theme].
 */
public class SegmentedControl extends JPanel {
    private static final int PAD = 3;
    private static final int ARC = 8;

    public SegmentedControl(JToggleButton... segments) {
        super(new FlowLayout(FlowLayout.LEFT, 0, 0));
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(PAD, PAD, PAD, PAD));
        for (JToggleButton segment : segments) {
            add(segment);
        }
    }

    /** A toggle that paints itself as one segment; plain JToggleButton behaviour otherwise. */
    public static JToggleButton segment(String text) {
        return new JToggleButton(text) {
            {
                setContentAreaFilled(false);
                setBorderPainted(false);
                setFocusPainted(false);
                setBorder(BorderFactory.createEmptyBorder(0, 14, 0, 14));
                setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
                addItemListener(e -> updateLook());
                updateLook();
            }

            private void updateLook() {
                Font base = UIManager.getFont("ToggleButton.font");
                if (base != null) {
                    setFont(base.deriveFont(isSelected() ? Font.BOLD : Font.PLAIN));
                }
                Color fg = UIManager.getColor(isSelected() ? "ToggleButton.foreground" : "Label.disabledForeground");
                if (fg != null) {
                    setForeground(fg);
                }
            }

            @Override
            public void updateUI() {
                super.updateUI();
                updateLook();
            }

            @Override
            public Dimension getPreferredSize() {
                // reserve the bold width so the control does not resize on selection;
                // the whole control is as tall as the other toolbar controls
                Dimension size = super.getPreferredSize();
                size.height = JoinedButtons.HEIGHT - 2 * PAD;
                Font font = getFont();
                if (font != null && !font.isBold() && getText() != null) {
                    size.width += getFontMetrics(font.deriveFont(Font.BOLD)).stringWidth(getText())
                        - getFontMetrics(font).stringWidth(getText());
                }
                return size;
            }

            @Override
            protected void paintComponent(Graphics g) {
                if (isSelected()) {
                    Graphics2D g2 = (Graphics2D) g.create();
                    try {
                        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                        g2.setColor(themed("Console.Segmented.selectedBorder", new Color(0, 0, 0, 30)));
                        g2.fillRoundRect(0, 1, getWidth(), getHeight() - 1, ARC - 2, ARC - 2);
                        g2.setColor(themed("Console.Segmented.selectedBackground", Color.WHITE));
                        g2.fillRoundRect(0, 0, getWidth(), getHeight() - 1, ARC - 2, ARC - 2);
                    } finally {
                        g2.dispose();
                    }
                }
                super.paintComponent(g);
            }
        };
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(themed("Console.Segmented.background", new Color(0xeef0f3)));
            g2.fillRoundRect(0, 0, getWidth(), getHeight(), ARC, ARC);
        } finally {
            g2.dispose();
        }
    }

    static Color themed(String key, Color fallback) {
        Color c = UIManager.getColor(key);
        return c != null ? c : fallback;
    }
}
