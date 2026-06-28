package com.rusefi.core.ui;

import javax.swing.*;
import java.awt.*;

/**
 * Opaque, centered "loading" cover painted on a frame's glassPane (#9715). Used to bridge the
 * splash → console handoff as a single indicator: shown when the user connects, kept across the
 * ConsoleUI build, and removed once the new content has painted (FrameHelper.swapContent).
 *
 * <p>Note: the bar does not animate while the EDT is building/painting the console — it reads as a
 * static cover. That's a Swing single-thread limit; lazy-loading heavy tabs keeps the freeze short.
 */
public class LoadingOverlay {
    public static void show(JFrame frame, String message) {
        JPanel inner = new JPanel();
        inner.setLayout(new BoxLayout(inner, BoxLayout.Y_AXIS));

        JLabel label = new JLabel(message);
        label.setAlignmentX(Component.CENTER_ALIGNMENT);
        label.setFont(label.getFont().deriveFont(Font.BOLD, 18f));
        inner.add(label);
        inner.add(Box.createVerticalStrut(16));

        JProgressBar bar = new JProgressBar();
        bar.setIndeterminate(true);
        bar.setPreferredSize(new Dimension(320, 28));
        bar.setMaximumSize(bar.getPreferredSize());
        bar.setAlignmentX(Component.CENTER_ALIGNMENT);
        inner.add(bar);

        JPanel overlay = new JPanel(new GridBagLayout());
        overlay.setOpaque(true);
        overlay.add(inner);
        frame.setGlassPane(overlay);
        overlay.setVisible(true);
    }

    public static void hide(JFrame frame) {
        frame.getGlassPane().setVisible(false);
    }
}
