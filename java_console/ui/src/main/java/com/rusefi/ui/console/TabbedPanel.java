package com.rusefi.ui.console;

import com.rusefi.PaneSettings;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.ui.LogDownloader;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;

public class TabbedPanel {
    // todo: the logic around 'criticalError' could be implemented nicer
    private String criticalError;

//    public final SettingsTab settingsTab;
    public final LogDownloader logsManager;

    public final JTabbedPane tabbedPane = new JTabbedPane() {
        @Override
        public void paint(Graphics g) {
            super.paint(g);
            paintStatusText(g);
        }

        private void paintStatusText(Graphics g) {
            Font f = g.getFont();
            g.setFont(new Font(f.getName(), f.getStyle(), f.getSize() * 4));
            Dimension d = getSize();
            String text;
            if (criticalError != null) {
                text = criticalError;
                g.setColor(Color.red);
            } else {
                switch (ConnectionStatusLogic.INSTANCE.getValue()) {
                    case NOT_CONNECTED:
                        text = "Not connected";
                        g.setColor(Color.white);
                        break;
                    case LOADING:
                        text = "Loading";
                        g.setColor(Color.white);
                        break;
                    default:
                        text = "";
                }
            }
            if (text.isEmpty())
                return;
            FontMetrics fm = g.getFontMetrics();
            int labelWidth = fm.stringWidth(text);
            int x = (d.width - labelWidth) / 2;
            int y = d.height / 2;
            int padding = 12;
            Color textColor = g.getColor();
            g.setColor(new Color(0, 0, 0, 180));
            g.fillRoundRect(x - padding, y - fm.getAscent() - padding, labelWidth + padding * 2, fm.getHeight() + padding * 2, 16, 16);
            g.setColor(textColor);
            g.drawString(text, x, y);
        }
    };

    public TabbedPanel(UIContext uiContext) {
        MessagesCentral.getInstance().addListener((clazz, message) -> {
            if (message.startsWith(Integration.CRITICAL_PREFIX))
                criticalError = message;
        });

        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> SwingUtilities.invokeLater(() -> {
            // clear the overlay so the UI becomes usable again
            if (ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                criticalError = null;
            }
            tabbedPane.repaint();
        }));

//        settingsTab = new SettingsTab(uiContext);
        logsManager = new LogDownloader(uiContext);
    }

    public void addTab(String title, Component component) {
        tabbedPane.addTab(title, component);
    }

    public JComponent getContent() {
        return tabbedPane;
    }

    public void setCornerComponent(JComponent component) {
        tabbedPane.putClientProperty("JTabbedPane.trailingComponent", component);
    }
}
