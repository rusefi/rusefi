package com.rusefi.ui.widgets;

import com.formdev.flatlaf.FlatClientProperties;

import javax.swing.*;
import javax.swing.text.JTextComponent;
import java.awt.*;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.geom.RoundRectangle2D;

/**
 * Console command input as one rounded control, "&gt; command | Go", as tall as the other
 * toolbar controls ({@link JoinedButtons#HEIGHT}). The outline turns to the focus colour while
 * typing and to the error colour when the field carries FlatLaf's error outline property
 * (set by AnyCommand for invalid input). Colours from the theme files [tag:console_theme].
 * The same frame serves other short inputs, e.g. "50000 | bytes" ({@link #withSuffix}).
 * A multi-line text area grows the box beyond that height, one text line at a time.
 */
public class CommandBox extends JPanel {
    private static final int ARC = 10;

    private final JTextComponent text;
    /** Shaded segment at the right end: the Go button or a unit label. */
    private final JComponent trailing;

    CommandBox(JTextComponent text, JButton go) {
        this(text, ">", styleGo(go));
    }

    /** A field with a unit shown in the shaded right segment, e.g. "bytes". */
    public static CommandBox withSuffix(JTextField text, String suffix) {
        JLabel unit = new JLabel(suffix);
        unit.setBorder(BorderFactory.createEmptyBorder(0, 14, 0, 14));
        return new CommandBox(text, null, unit);
    }

    private CommandBox(JTextComponent text, String prompt, JComponent trailing) {
        super(new BorderLayout());
        this.text = text;
        this.trailing = trailing;
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));

        if (prompt != null) {
            JLabel promptLabel = new JLabel(prompt);
            promptLabel.setFont(ConsoleFonts.mono(Font.BOLD, ConsoleFonts.baseSize()));
            promptLabel.setForeground(SegmentedControl.themed("Console.TitleBar.mutedForeground", Color.GRAY));
            promptLabel.setBorder(BorderFactory.createEmptyBorder(0, 12, 0, 6));
            add(promptLabel, BorderLayout.WEST);
        }

        // the box draws the frame; the field itself is bare text
        text.setBorder(BorderFactory.createEmptyBorder(0, prompt == null ? 12 : 0, 0, 8));
        text.setOpaque(false);
        text.setFont(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize()));
        add(text, BorderLayout.CENTER);
        add(trailing, BorderLayout.EAST);

        text.addFocusListener(new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent e) {
                repaint();
            }

            @Override
            public void focusLost(FocusEvent e) {
                repaint();
            }
        });
        text.addPropertyChangeListener(FlatClientProperties.OUTLINE, e -> repaint());
    }

    private static JButton styleGo(JButton go) {
        go.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
        go.setFocusPainted(false);
        go.setMargin(new Insets(0, 16, 0, 16));
        Font buttonFont = UIManager.getFont("Button.font");
        if (buttonFont != null) {
            go.setFont(buttonFont.deriveFont(Font.BOLD));
        }
        return go;
    }

    @Override
    public Dimension getPreferredSize() {
        Dimension size = super.getPreferredSize();
        size.height = Math.max(JoinedButtons.HEIGHT, size.height);
        return size;
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.clip(new RoundRectangle2D.Float(0, 0, getWidth(), getHeight(), ARC, ARC));
            g2.setColor(SegmentedControl.themed("TextField.background", Color.WHITE));
            g2.fillRect(0, 0, getWidth(), getHeight());
            // shaded Go segment
            g2.setColor(SegmentedControl.themed("Console.Readout.background", new Color(0xeef0f3)));
            g2.fillRect(trailing.getX(), 0, getWidth() - trailing.getX(), getHeight());
        } finally {
            g2.dispose();
        }
    }

    @Override
    protected void paintChildren(Graphics g) {
        Graphics2D clipped = (Graphics2D) g.create();
        try {
            clipped.clip(new RoundRectangle2D.Float(0, 0, getWidth(), getHeight(), ARC, ARC));
            super.paintChildren(clipped);
        } finally {
            clipped.dispose();
        }
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            Color border = SegmentedControl.themed("Component.borderColor", Color.LIGHT_GRAY);
            g2.setColor(border);
            g2.drawLine(trailing.getX() - 1, 1, trailing.getX() - 1, getHeight() - 2);
            boolean error = FlatClientProperties.OUTLINE_ERROR.equals(text.getClientProperty(FlatClientProperties.OUTLINE));
            if (error) {
                g2.setColor(SegmentedControl.themed("Component.error.focusedBorderColor", Color.RED));
                g2.setStroke(new BasicStroke(1.5f));
            } else if (text.isFocusOwner()) {
                g2.setColor(SegmentedControl.themed("Component.focusedBorderColor", Color.BLUE));
                g2.setStroke(new BasicStroke(1.5f));
            }
            g2.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, ARC, ARC);
        } finally {
            g2.dispose();
        }
    }
}
