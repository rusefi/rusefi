package com.rusefi.ui.console;

import com.rusefi.PaneSettings;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.ConnectionStatusLogic;
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
                        break;
                    case LOADING:
                        text = "Loading";
                        break;
                    default:
                        text = "";
                }
            }
            int labelWidth = g.getFontMetrics().stringWidth(text);
            g.drawString(text, (d.width - labelWidth) / 2, d.height / 2);
        }
    };

    public TabbedPanel(UIContext uiContext) {
        MessagesCentral.getInstance().addListener((clazz, message) -> {
            if (message.startsWith(Integration.CRITICAL_PREFIX))
                criticalError = message;
        });

//        settingsTab = new SettingsTab(uiContext);
        logsManager = new LogDownloader(uiContext);
    }

    public void addTab(String title, Component component) {
        tabbedPane.addTab(title, component);
    }
}
