package com.rusefi.ui.widgets;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;

import javax.swing.*;
import java.awt.*;

public class ConnectionStatusIcon extends JButton {
    public ConnectionStatusIcon(LinkManager linkManager) {
        setIcon(new Icon() {
            @Override
            public void paintIcon(Component c, Graphics g, int x, int y) {
                g.setColor(ConnectionStatusLogic.INSTANCE.isConnected() ? Color.GREEN : Color.RED);
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
            boolean isConnected = ConnectionStatusLogic.INSTANCE.isConnected();
            setToolTipText(isConnected ? "Connected. Click or Ctrl+D to disconnect." : "Disconnected. Click or Ctrl+R to connect.");
            repaint();
        };

        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> SwingUtilities.invokeLater(updateButton));
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
