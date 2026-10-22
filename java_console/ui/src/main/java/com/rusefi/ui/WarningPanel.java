package com.rusefi.ui;

import com.devexperts.logging.FileLogger;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.ui.startup.StatusDotIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.WarningIcon;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Status strip at the bottom of the Gauges, Messages and Engine Sniffer tabs: the latest firmware
 * warning or error (critical errors blink), Clear warning, the log folder and the on-screen log
 * buffer size. Colours from the theme files [tag:console_theme].
 */
public class WarningPanel {

    public static final String LOG_INFO_TEXT = "Writing logs to '" + FileLogger.DIR + "'";
    static final String NO_WARNINGS = "No active warnings";
    private static final String WARNING = "WARNING";
    private static final String ERROR = "firmware error";
    private final JPanel panel = new JPanel(new BorderLayout()) {
        @Override
        protected void paintComponent(Graphics g) {
            g.setColor(themed("Console.StatusBar.background", getBackground()));
            g.fillRect(0, 0, getWidth(), getHeight());
            g.setColor(themed("Separator.foreground", Color.LIGHT_GRAY));
            g.drawLine(0, 0, getWidth(), 0);
        }
    };

    private final JLabel label = new JLabel("", new WarningIcon(), SwingConstants.LEADING);
    private final JButton reset = new JButton("Clear warning");
    private boolean haveFatalError = false;
    private final Timer criticalErrorBlinking = new Timer(1000, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            label.setVisible(!label.isVisible());
            AutoupdateUtil.trueLayoutAndRepaint(label);
        }
    });

    /** Warnings and log controls on the left; the strip's own controls (e.g. Record) on the right. */
    private final JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, 14, 0));

    public WarningPanel(Node config) {
        this(config, (JComponent) null);
    }

    /** With the console's data logging button at the right end, when the console provides one. */
    public WarningPanel(Node config, UIContext uiContext) {
        this(config, uiContext.recordButtonFactory.get());
    }

    private WarningPanel(Node config, JComponent trailing) {
        panel.setBorder(BorderFactory.createEmptyBorder(8, 2, 8, 2));
        left.setOpaque(false);
        panel.add(left, BorderLayout.CENTER);
        if (trailing != null) {
            // its own column on the right: a wider button grows into free space and moves nothing else
            JPanel right = new JPanel(new GridBagLayout());
            right.setOpaque(false);
            right.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, JoinedButtons.ROW_END_INSET));
            right.add(trailing);
            panel.add(right, BorderLayout.EAST);
        }
        label.setIconTextGap(10);
        left.add(label);

        clear();

        reset.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                clear();
            }
        });

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (haveFatalError)
                    return;

                if (message.startsWith(Integration.CRITICAL_PREFIX)) {
                    haveFatalError = true;
                    criticalErrorBlinking.start();
                    showWarning(message);
                    return;
                }
                if (message.startsWith(WARNING) || message.startsWith(ERROR)) {
                    showWarning(message);
                }
            }
        });

        // reset after ecu reset, in case that the critical error is still on the ecu, we have a small blink,
        // but we need this reset to happen in the happy path of: fix config error => reboot ecu => console doesn't show any error
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> {
            if (ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                SwingUtilities.invokeLater(this::clear);
            }
        });

        // same outlined style as the toolbar buttons (Edit dashboard)
        left.add(new JoinedButtons(reset));
        left.add(divider());
        // todo: only display label if logs are being recorded
        // the dot is drawn in its label's colour, so it gets its own label: green dot, normal text
        JPanel logRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        logRow.setOpaque(false);
        logRow.add(new JLabel(new StatusDotIcon()) {
            {
                setForeground(themed("Console.Status.connected.dot", new Color(0x16a34a)));
                setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 8));
            }
        });
        logRow.add(new JLabel(LOG_INFO_TEXT));
        left.add(logRow);
        left.add(divider());
        Component logSize = new LogSizeControl(config).getContent();
        if (logSize instanceof JComponent) {
            ((JComponent) logSize).setOpaque(false);
        }
        left.add(logSize);
    }

    private void showWarning(String message) {
        label.setText(message);
        label.setForeground(themed("Console.Status.disconnected.foreground", new Color(0xb91c1c)));
        label.setFont(label.getFont().deriveFont(Font.BOLD));
        reset.setEnabled(true);
    }

    private void clear() {
        label.setText(NO_WARNINGS);
        label.setForeground(themed("Console.TitleBar.mutedForeground", Color.GRAY));
        label.setFont(UIManager.getFont("Label.font"));
        label.setVisible(true);
        reset.setEnabled(false);
        haveFatalError = false;
        criticalErrorBlinking.stop();
    }

    private static JComponent divider() {
        JComponent line = new JComponent() {
            @Override
            protected void paintComponent(Graphics g) {
                g.setColor(themed("Separator.foreground", Color.LIGHT_GRAY));
                g.drawLine(getWidth() / 2, 2, getWidth() / 2, getHeight() - 3);
            }
        };
        line.setPreferredSize(new Dimension(1, 24));
        return line;
    }

    private static Color themed(String key, Color fallback) {
        Color c = UIManager.getColor(key);
        return c != null ? c : fallback;
    }

    // package-private so tests can read the warning text
    JLabel getWarningLabel() {
        return label;
    }

    public JPanel getPanel(Node config) {
        return panel;
    }
}
