package com.rusefi.ui.console;

import com.rusefi.config.generated.Integration;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.ui.LogDownloader;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;

public class TabbedPanel {
    // todo: the logic around 'criticalError' could be implemented nicer
    private String criticalError;

//    public final SettingsTab settingsTab;
    public final LogDownloader logsManager;

    /**
     * Glass pane drawn above all content — always painted last by Swing, so child component
     * repaints (e.g. JProgressBar updates) can never cover it the way a paint() override can.
     * Mouse events pass through so the normal UI remains interactive.
     */
    private final JComponent statusGlassPane = new JComponent() {
        @Override
        public boolean contains(int x, int y) {
            return false; // pass mouse events through to components below
        }

        @Override
        protected void paintComponent(Graphics g) {
            paintStatusText(g, getSize());
        }

        private void paintStatusText(Graphics g, Dimension d) {
            Font f = g.getFont();
            g.setFont(new Font(f.getName(), f.getStyle(), f.getSize() * 4));
            String text;
            Color textColor;
            if (criticalError != null) {
                text = criticalError;
                textColor = Color.red;
            } else if (Boolean.TRUE.equals(tabbedPane.getClientProperty("isUpdating"))) {
                text = "Updating";
                textColor = Color.orange;
            } else {
                switch (ConnectionStatusLogic.INSTANCE.getValue()) {
                    case NOT_CONNECTED:
                        text = "Not connected";
                        textColor = Color.white;
                        break;
                    case LOADING:
                        text = "Loading";
                        textColor = Color.white;
                        break;
                    default:
                        return;
                }
            }
            FontMetrics fm = g.getFontMetrics();
            int labelWidth = fm.stringWidth(text);
            int x = (d.width - labelWidth) / 2;
            int y = d.height / 2;
            int padding = 12;
            g.setColor(new Color(0, 0, 0, 180));
            g.fillRoundRect(x - padding, y - fm.getAscent() - padding, labelWidth + padding * 2, fm.getHeight() + padding * 2, 16, 16);
            g.setColor(textColor);
            g.drawString(text, x, y);
        }
    };

    public final JTabbedPane tabbedPane = new JTabbedPane();

    private void installGlassPane() {
        JRootPane rootPane = tabbedPane.getRootPane();
        if (rootPane == null)
            return;
        statusGlassPane.setOpaque(false);
        rootPane.setGlassPane(statusGlassPane);
        statusGlassPane.setVisible(true);
    }

    private void refreshOverlay() {
        statusGlassPane.repaint();
    }

    public TabbedPanel(UIContext uiContext) {
        tabbedPane.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 && tabbedPane.isShowing())
                installGlassPane();
        });

        MessagesCentral.getInstance().addListener((clazz, message) -> {
            if (message.startsWith(Integration.CRITICAL_PREFIX))
                criticalError = message;
        });

        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> SwingUtilities.invokeLater(() -> {
            // clear the overlay so the UI becomes usable again
            if (ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                criticalError = null;
            }
            refreshOverlay();
        }));

        tabbedPane.addPropertyChangeListener("isUpdating", e -> refreshOverlay());

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
