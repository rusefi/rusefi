package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;

/**
 * White strip with a divider along its bottom edge, like the Gauges toolbar and the status bar
 * [tag:console_theme]. Children should be transparent so the strip's background shows.
 */
public class ToolbarStrip extends JPanel {
    public ToolbarStrip(LayoutManager layout) {
        super(layout);
    }

    /** Pane title on the left, optional controls on the right, e.g. "Output | Clear Pause". */
    public static JPanel header(String title, JComponent trailing) {
        JPanel header = new ToolbarStrip(new BorderLayout());
        header.setBorder(BorderFactory.createEmptyBorder(6, 12, 7, JoinedButtons.ROW_END_INSET));
        JLabel label = new JLabel(title);
        label.setFont(label.getFont().deriveFont(Font.BOLD));
        header.add(label, BorderLayout.WEST);
        // as tall as the controls, so headers with and without them line up
        header.add(Box.createVerticalStrut(JoinedButtons.HEIGHT), BorderLayout.CENTER);
        if (trailing != null) {
            header.add(trailing, BorderLayout.EAST);
        }
        return header;
    }

    @Override
    protected void paintComponent(Graphics g) {
        Color background = UIManager.getColor("Console.Toolbar.background");
        g.setColor(background != null ? background : getBackground());
        g.fillRect(0, 0, getWidth(), getHeight());
        Color line = UIManager.getColor("Separator.foreground");
        g.setColor(line != null ? line : Color.LIGHT_GRAY);
        g.drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1);
    }
}
