package com.rusefi.ui;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.widgets.ConsoleFonts;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.*;
import java.awt.*;

/**
 * Digital RPM gauge which stays green while rusEfi is connected
 * <p/>
 * 9/17/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class RpmLabel {
    private static final String NO_CONNECTION = "N/C";
    private static final int ARC = 10;
    /**
     * Grey rounded readout box, as tall as the toolbar controls next to it, caption and number
     * centred vertically; colours from the theme files [tag:console_theme].
     */
    private final JPanel content = new JPanel(new GridBagLayout()) {
        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            size.height = Math.max(size.height, JoinedButtons.HEIGHT);
            return size;
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(themed("Console.Readout.background", new Color(0xeef0f3)));
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), ARC, ARC);
            } finally {
                g2.dispose();
            }
        }
    };

    private final JLabel rpmValue = new JLabel();
    private final JLabel rpmCaption = new JLabel("RPM");
    private final RpmModel.RpmListener rpmListener;
    private final ConnectionStatusLogic.Listener connectionStatusListener;

    public RpmLabel(UIContext uiContext) {
        this(uiContext, 1);
    }

    public RpmLabel(UIContext uiContext, int size) {
        this(uiContext, size, true);
    }

    public RpmLabel(UIContext uiContext, int size, boolean withCaption) {
        String initialLabel = uiContext.getLinkManager().isLogViewer() ? "LOG" : NO_CONNECTION;
        rpmValue.setText(initialLabel);
        rpmValue.setForeground(disconnectedColor());
        rpmValue.setHorizontalAlignment(SwingConstants.RIGHT);
        rpmCaption.setForeground(themed("Console.TitleBar.mutedForeground", Color.GRAY));

        content.setOpaque(false);
        content.setBorder(BorderFactory.createEmptyBorder(0, 14, 0, 14));
        GridBagConstraints c = new GridBagConstraints();
        c.anchor = GridBagConstraints.CENTER;
        if (withCaption) {
            c.insets = new Insets(0, 0, 0, 12);
            content.add(rpmCaption, c);
            c.insets = new Insets(0, 0, 0, 0);
        }
        content.add(rpmValue, c);

        rpmListener = rpm -> {
            if (ConnectionStatusLogic.INSTANCE.isConnected()) {
                updateRpmValue(rpm.getSmoothedValue());
                rpmValue.setForeground(connectedColor());
            }
        };
        RpmModel.getInstance().addListener(rpmListener);

        connectionStatusListener = new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (isConnected) {
                    updateRpmValue(RpmModel.getInstance().getSmoothedValue());
                    rpmValue.setForeground(connectedColor());
                } else {
                    rpmValue.setText(NO_CONNECTION);
                    rpmValue.setForeground(disconnectedColor());
                }
            }
        };
        ConnectionStatusLogic.INSTANCE.addAndFireListener(connectionStatusListener);
        setSize(size);
    }

    private void updateRpmValue(int value) {
        if (value == -1) {
            rpmValue.setText("Noise");
        } else {
            rpmValue.setText(value + "");
        }
    }

    public JPanel getContent() {
        return content;
    }

    /**
     * Larger sizes enlarge the number: size 2 (Gauges, Messages, Engine Sniffer toolbars) is a
     * 22 px figure with the default 13 px theme font. The number keeps a fixed width (five
     * digits or "Noise"), so the readout does not change width while RPM changes.
     */
    public RpmLabel setSize(int size) {
        int base = ConsoleFonts.baseSize();
        Font label = UIManager.getFont("Label.font");
        // 17 px caption next to the 22 px number with the default 13 px theme font
        Font captionFont = (label != null ? label : rpmCaption.getFont()).deriveFont(Font.BOLD, (float) (base + 4 * (size - 1)));
        // small caps-style caption: slightly letter-spaced
        rpmCaption.setFont(captionFont.deriveFont(java.util.Collections.singletonMap(
            java.awt.font.TextAttribute.TRACKING, 0.08f)));
        Font font = (label != null ? label : rpmValue.getFont()).deriveFont(Font.BOLD, (float) (base + 9 * (size - 1)));
        rpmValue.setFont(font);
        FontMetrics fm = rpmValue.getFontMetrics(font);
        int width = Math.max(fm.stringWidth("Noise"), fm.stringWidth("88888"));
        rpmValue.setPreferredSize(new Dimension(width, fm.getHeight()));
        return this;
    }

    private static Color connectedColor() {
        return themed("Console.Status.connected.foreground", new Color(0x15803d));
    }

    private static Color disconnectedColor() {
        return themed("Console.Status.disconnected.foreground", new Color(0xb91c1c));
    }

    private static Color themed(String key, Color fallback) {
        Color c = UIManager.getColor(key);
        return c != null ? c : fallback;
    }

    public void destroy() {
        RpmModel.getInstance().removeListener(rpmListener);
        ConnectionStatusLogic.INSTANCE.removeListener(connectionStatusListener);
    }
}
