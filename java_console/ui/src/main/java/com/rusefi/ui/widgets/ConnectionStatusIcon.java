package com.rusefi.ui.widgets;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;

import javax.swing.*;
import java.awt.*;

/**
 * Connection badge in the tab row: a coloured dot and state name on a rounded background.
 * Clicking it connects or disconnects. Colours come from the Console.Status.* keys in the theme
 * files [tag:console_theme], with built-in light defaults when no theme is installed.
 */
public class ConnectionStatusIcon extends JButton {
    private static final int DOT = 8;

    enum State {
        CONNECTED("connected", "#16a34a", "#e7f6ec", "#15803d"),
        DISCONNECTED("disconnected", "#dc2626", "#fdecec", "#b91c1c"),
        // connecting, or an offline tune without an ECU
        PENDING("pending", "#d97706", "#fdf3e3", "#b45309"),
        // Board sitting in a DFU/OpenBLT bootloader [tag:better_ux_for_flashing] — distinct from connected/disconnected.
        BOOTLOADER("bootloader", "#9628d2", "#f3e8fb", "#7e22ce");

        private final String key;
        private final Color dot;
        private final Color background;
        private final Color foreground;

        State(String key, String dot, String background, String foreground) {
            this.key = key;
            this.dot = Color.decode(dot);
            this.background = Color.decode(background);
            this.foreground = Color.decode(foreground);
        }

        Color dot() {
            return themed("dot", dot);
        }

        Color background() {
            return themed("background", background);
        }

        Color foreground() {
            return themed("foreground", foreground);
        }

        private Color themed(String part, Color fallback) {
            Color c = UIManager.getColor("Console.Status." + key + "." + part);
            return c != null ? c : fallback;
        }
    }

    private final JTabbedPane tabbedPane;
    private State state = State.DISCONNECTED;

    public ConnectionStatusIcon(LinkManager linkManager, JTabbedPane tabbedPane) {
        this.tabbedPane = tabbedPane;
        setIcon(new Icon() {
            @Override
            public void paintIcon(Component c, Graphics g, int x, int y) {
                Graphics2D g2 = (Graphics2D) g.create();
                try {
                    g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                    g2.setColor(currentState().dot());
                    g2.fillOval(x, y, DOT, DOT);
                } finally {
                    g2.dispose();
                }
            }

            @Override
            public int getIconWidth() {
                return DOT;
            }

            @Override
            public int getIconHeight() {
                return DOT;
            }
        });
        setIconTextGap(8);
        setBorderPainted(false);
        setContentAreaFilled(false);
        setFocusPainted(false);
        setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        setBorder(BorderFactory.createEmptyBorder(5, 12, 5, 12));

        Runnable updateButton = () -> {
            state = currentState();
            final Object bootloaderMode = tabbedPane == null ? null : tabbedPane.getClientProperty("bootloaderMode");
            final Object offlineMode = tabbedPane == null ? null : tabbedPane.getClientProperty("offlineMode");
            if (bootloaderMode != null) {
                setText(bootloaderMode + " bootloader");
                setToolTipText(bootloaderMode + " bootloader — use the Device tab to update firmware.");
            } else if (offlineMode != null && !ConnectionStatusLogic.INSTANCE.isConnected()) {
                setText("Offline");
                setToolTipText("Offline tune loaded. Connect ECU to burn or compare.");
            } else {
                // isConnected() is also true while LOADING (link open, ECU data still arriving)
                boolean isConnected = ConnectionStatusLogic.INSTANCE.isConnected();
                if (ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.LOADING) {
                    setText("Connecting…");
                } else if (isConnected) {
                    setText("Connected");
                } else {
                    setText("Disconnected");
                }
                setToolTipText(isConnected ? "Connected. Click or Ctrl+D to disconnect." : "Disconnected. Click or Ctrl+R to connect.");
            }
            setForeground(state.foreground());
            repaint();
        };

        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> SwingUtilities.invokeLater(updateButton));
        if (tabbedPane != null) {
            tabbedPane.addPropertyChangeListener("bootloaderMode", e -> SwingUtilities.invokeLater(updateButton));
            tabbedPane.addPropertyChangeListener("offlineMode", e -> SwingUtilities.invokeLater(updateButton));
        }
        updateButton.run();

        addActionListener(e -> {
            if (ConnectionStatusLogic.INSTANCE.isConnected()) {
                linkManager.disconnect();
            } else {
                linkManager.reconnect();
            }
        });
    }

    private State currentState() {
        if (tabbedPane != null && tabbedPane.getClientProperty("bootloaderMode") != null) {
            return State.BOOTLOADER;
        }
        if (ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.LOADING) {
            return State.PENDING;
        }
        if (ConnectionStatusLogic.INSTANCE.isConnected()) {
            return State.CONNECTED;
        }
        if (tabbedPane != null && tabbedPane.getClientProperty("offlineMode") != null) {
            return State.PENDING;
        }
        return State.DISCONNECTED;
    }

    @Override
    public void updateUI() {
        super.updateUI();
        Font base = UIManager.getFont("Button.font");
        if (base != null) {
            setFont(base.deriveFont(Font.BOLD));
        }
        if (state != null) {
            setForeground(state.foreground());
        }
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(currentState().background());
            g2.fillRoundRect(0, 0, getWidth(), getHeight(), getHeight(), getHeight());
        } finally {
            g2.dispose();
        }
        super.paintComponent(g);
    }
}
