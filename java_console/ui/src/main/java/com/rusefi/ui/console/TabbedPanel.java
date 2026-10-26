package com.rusefi.ui.console;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.ui.LogDownloader;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.logview.LogViewerHost;
import com.rusefi.ui.obd.ObdCodesPane;
import com.rusefi.ui.widgets.SparkleIcon;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;

public class TabbedPanel {
    private static final int TAB_ICON_SIZE = 18;
    // todo: the logic around 'criticalError' could be implemented nicer
    private String criticalError;

//    public final SettingsTab settingsTab;
    public final LogDownloader logsManager;

    /**
     * Glass pane drawn above all content — always painted last by Swing, so child component
     * repaints (e.g. JProgressBar updates) can never cover it the way a paint() override can.
     * Mouse events pass through so the normal UI remains interactive.
     * Visibility is toggled instead of trying to clear pixels, since AlphaComposite.Clear
     * punches through the entire window buffer on most rendering pipelines.
     */
    private final JComponent statusGlassPane = new JComponent() {
        @Override
        public boolean contains(int x, int y) {
            return false; // pass mouse events through to components below
        }

        @Override
        protected void paintComponent(Graphics g) {
            Dimension d = getSize();
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
                ConnectionStatusValue status = ConnectionStatusLogic.INSTANCE.getValue();
                if (status == ConnectionStatusValue.LOADING) {
                    text = "Loading";
                    textColor = Color.white;
                } else {
                    // Don't show "Not connected" overlay — the status is visible in the
                    // title bar and corner icon. Blocking the entire UI with a black overlay
                    // is too aggressive, especially for offline editing.
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

    public final JTabbedPane tabbedPane = new JTabbedPane() {
        @Override
        public void updateUI() {
            super.updateUI();
            // White tab row above the grey page [tag:console_theme]. A ColorUIResource keeps the
            // colour replaceable by the next theme, like the look-and-feel default it overrides.
            Color tabArea = UIManager.getColor("Console.TabArea.background");
            if (tabArea != null) {
                setBackground(new javax.swing.plaf.ColorUIResource(tabArea));
            }
        }
    };

    void installGlassPane() {
        JRootPane rootPane = tabbedPane.getRootPane();
        if (rootPane == null)
            return;
        statusGlassPane.setOpaque(false);
        statusGlassPane.setFocusable(false);
        rootPane.setGlassPane(statusGlassPane);
        refreshOverlay();
    }

    private boolean hasOverlay() {
        return criticalError != null
            || Boolean.TRUE.equals(tabbedPane.getClientProperty("isUpdating"))
            || ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.LOADING;
    }

    private void refreshOverlay() {
        statusGlassPane.setVisible(hasOverlay());
    }

    public TabbedPanel(UIContext uiContext) {
        // One row at any width. Wrapping moved the first tabs onto a second row and stretched
        // that row's tabs to fill it; tabs that do not fit are listed by FlatLaf's "more" button.
        tabbedPane.setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);
        tabbedPane.putClientProperty(FlatClientProperties.TABBED_PANE_TABS_POPUP_POLICY,
            FlatClientProperties.TABBED_PANE_POLICY_AS_NEEDED);
        tabbedPane.putClientProperty(FlatClientProperties.TABBED_PANE_SCROLL_BUTTONS_POLICY,
            FlatClientProperties.TABBED_PANE_POLICY_NEVER);
        tabbedPane.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 && tabbedPane.isShowing())
                SwingUtilities.invokeLater(this::installGlassPane);
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
        insertTab(title, component, tabbedPane.getTabCount());
    }

    public void insertTab(String title, Component component, int index) {
        Icon icon = loadTabIcon(title);
        tabbedPane.insertTab(title, icon, component, null, index);
        tabbedPane.setTabComponentAt(index, new TabTitle(tabbedPane, component, title, icon));
    }

    /** Adds a tab at the end of the row with an already built title, such as {@link #createClosableTitle}. */
    void addTab(String title, Component component, Component tabTitle) {
        tabbedPane.addTab(title, loadTabIcon(title), component);
        tabbedPane.setTabComponentAt(tabbedPane.indexOfComponent(component), tabTitle);
    }

    /**
     * Tab title with a close button. Build it once per tab content: each TabTitle listens to the
     * pane for as long as the pane lives.
     */
    JComponent createClosableTitle(String title, Component content, Runnable onClose) {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEADING, 0, 0));
        panel.setOpaque(false);
        panel.add(new TabTitle(tabbedPane, content, title, loadTabIcon(title)));
        JButton close = new JButton();
        Icon closeIcon = UIManager.getIcon("TabbedPane.closeIcon");
        if (closeIcon != null) {
            close.setIcon(closeIcon);
        } else {
            close.setText("×");
        }
        close.setToolTipText("Close " + title + " (reopen it from the Tools menu)");
        close.setBorder(BorderFactory.createEmptyBorder(0, 6, 0, 0));
        close.setContentAreaFilled(false);
        close.setFocusable(false);
        close.addActionListener(e -> onClose.run());
        panel.add(close);
        return panel;
    }

    /**
     * Tab title that turns bold and takes the selected colour on the selected tab, which FlatLaf
     * cannot do per tab. Its width always fits the bold text so the tabs do not shift on selection.
     * The tab keeps its own title, so indexOfTab() and the hidden-tabs menu still work.
     */
    static class TabTitle extends JLabel {
        private final JTabbedPane pane;
        private final Component content;

        TabTitle(JTabbedPane pane, Component content, String title, Icon icon) {
            super(title, icon, SwingConstants.LEADING);
            this.pane = pane;
            this.content = content;
            setIconTextGap(6);
            pane.addChangeListener(e -> refresh());
            refresh();
        }

        boolean isSelectedTab() {
            return pane.getSelectedComponent() == content;
        }

        private void refresh() {
            if (pane == null) {
                return;
            }
            boolean selected = isSelectedTab();
            Font base = UIManager.getFont("TabbedPane.font");
            if (base != null) {
                setFont(base.deriveFont(selected ? Font.BOLD : Font.PLAIN));
            }
            Color color = UIManager.getColor(selected ? "TabbedPane.selectedForeground" : "TabbedPane.foreground");
            if (color != null) {
                setForeground(color);
            }
            int index = pane.indexOfComponent(content);
            setEnabled(index < 0 || pane.isEnabledAt(index));
            revalidate();
            repaint();
        }

        @Override
        public void updateUI() {
            super.updateUI();
            refresh();
        }

        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            Font font = getFont();
            String text = getText();
            if (font != null && text != null && !font.isBold()) {
                size.width += getFontMetrics(font.deriveFont(Font.BOLD)).stringWidth(text)
                    - getFontMetrics(font).stringWidth(text);
            }
            return size;
        }
    }

    static Icon loadTabIcon(String title) {
        String iconName;
        switch (title) {
            case "Gauges":
                iconName = "idle";
                break;
            case LogViewerHost.TITLE:
                iconName = "view";
                break;
            case "Messages":
                iconName = "message";
                break;
            case "Lua Scripting":
                iconName = "braces";
                break;
            case "Engine Sniffer":
                iconName = "sensors";
                break;
            case "Tuning":
                iconName = "dialog";
                break;
            case "Knock Analyzer":
                iconName = "curve";
                break;
            case "Pinout":
                iconName = "resistor";
                break;
            case "Manage Tunes":
                iconName = "group";
                break;
            case "Device":
                iconName = "controller";
                break;
            case ObdCodesPane.TITLE:
                iconName = "file-info";
                break;
            case "AI Troubleshooting":
                return new SparkleIcon(TAB_ICON_SIZE);
            case "SLCAN Sniffer":
                iconName = "can";
                break;
            default:
                return null;
        }

        ImageIcon icon = AutoupdateUtil.loadIcon("icons/tuning/" + iconName + "48.png");
        if (icon == null) {
            return null;
        }
        return new ImageIcon(icon.getImage().getScaledInstance(TAB_ICON_SIZE, TAB_ICON_SIZE, Image.SCALE_SMOOTH));
    }

    public void selectTab(String title) {
        int idx = tabbedPane.indexOfTab(title);
        if (idx >= 0) tabbedPane.setSelectedIndex(idx);
    }

    public JComponent getContent() {
        return tabbedPane;
    }

    public void setCornerComponent(JComponent component) {
        tabbedPane.putClientProperty("JTabbedPane.trailingComponent", component);
    }
}
