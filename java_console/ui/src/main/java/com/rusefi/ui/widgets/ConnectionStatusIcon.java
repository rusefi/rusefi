package com.rusefi.ui.widgets;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;

import javax.swing.*;
import java.awt.*;

public class ConnectionStatusIcon extends JButton {
    // Board sitting in a DFU/OpenBLT bootloader [tag:better_ux_for_flashing] — distinct from connected (green)/disconnected (red).
    private static final Color BOOTLOADER_PURPLE = new Color(150, 40, 210);

    public ConnectionStatusIcon(LinkManager linkManager, JTabbedPane tabbedPane) {
        setIcon(new Icon() {
            @Override
            public void paintIcon(Component c, Graphics g, int x, int y) {
                final Color color;
                if (tabbedPane != null && tabbedPane.getClientProperty("bootloaderMode") != null) {
                    color = BOOTLOADER_PURPLE;
                } else if (ConnectionStatusLogic.INSTANCE.isConnected()) {
                    color = Color.GREEN;
                } else {
                    color = Color.RED;
                }
                g.setColor(color);
                g.fillOval(x, y, getIconWidth(), getIconHeight());
            }

            @Override
            public int getIconWidth() {
                return 16;
            }

            @Override
            public int getIconHeight() {
                return 16;
            }
        });
        setBorderPainted(false);
        setContentAreaFilled(false);

        Runnable updateButton = () -> {
            final Object bootloaderMode = tabbedPane == null ? null : tabbedPane.getClientProperty("bootloaderMode");
            final Object offlineMode = tabbedPane == null ? null : tabbedPane.getClientProperty("offlineMode");
            if (bootloaderMode != null) {
                setToolTipText(bootloaderMode + " bootloader — use the Device tab to update firmware.");
            } else if (offlineMode != null && !ConnectionStatusLogic.INSTANCE.isConnected()) {
                setToolTipText("Offline tune loaded. Connect ECU to burn or compare.");
            } else {
                boolean isConnected = ConnectionStatusLogic.INSTANCE.isConnected();
                setToolTipText(isConnected ? "Connected. Click or Ctrl+D to disconnect." : "Disconnected. Click or Ctrl+R to connect.");
            }
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
}
