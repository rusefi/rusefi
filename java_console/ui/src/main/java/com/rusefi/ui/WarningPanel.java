package com.rusefi.ui;

import com.devexperts.logging.FileLogger;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class WarningPanel {

    public static final String LOG_INFO_TEXT = "Writing logs to '" + FileLogger.DIR + "'";
    private static final String WARNING = "WARNING";
    private static final String ERROR = "firmware error";
    private final JPanel panel = new JPanel(new FlowLayout());

    private final JLabel label = new JLabel();
    private final JButton reset = new JButton("clear warning");
    private boolean haveFatalError = false;
    private final Timer criticalErrorBlinking = new Timer(1000, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            label.setVisible(!label.isVisible());
            AutoupdateUtil.trueLayoutAndRepaint(label);
        }
    });

    public WarningPanel(Node config) {
        label.setForeground(Color.red);
        panel.add(label);

        Font currentFont = label.getFont();
        label.setFont(currentFont.deriveFont((float) (currentFont.getSize() * 1.5)));

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
                    label.setText(message);
                    reset.setEnabled(true);
                    return;
                }
                if (message.startsWith(WARNING) || message.startsWith(ERROR)) {
                    label.setText(message);
                    reset.setEnabled(true);
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

        panel.add(reset);
        // todo: only display label if logs are being recorded
        panel.add(new JLabel(LOG_INFO_TEXT));
        panel.add(new LogSizeControl(config).getContent());
    }

    private void clear() {
        label.setText("");
        label.setVisible(true);
        reset.setEnabled(false);
        haveFatalError = false;
        criticalErrorBlinking.stop();
    }

    public JPanel getPanel(Node config) {
        return panel;
    }
}
