package com.rusefi.ui.config;

import com.rusefi.FileLog;
import com.rusefi.config.Field;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.InvocationConfirmationListener;
import com.rusefi.ui.ConnectionStatus;

import javax.swing.*;
import java.awt.*;

class BaseConfigField {
    protected final JLabel status = new JLabel("P");
    protected final JPanel panel = new JPanel(new BorderLayout());

    public BaseConfigField(final Field field) {
        /**
         * This would request initial value
         */
        ConnectionStatus.INSTANCE.addListener(new ConnectionStatus.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                CommandQueue.getInstance().write(field.getCommand(),
                        CommandQueue.DEFAULT_TIMEOUT,
                        InvocationConfirmationListener.VOID,
                        false);
            }
        });

        status.setToolTipText("Pending...");

    }

    protected void onValueArrived() {
        status.setText("");
        status.setToolTipText(null);
    }

    protected void sendValue(Field field, String newValue) {
        String msg = field.setCommand() + " " + newValue;
        FileLog.MAIN.logLine("Sending " + msg);
        CommandQueue.getInstance().write(msg);
        status.setText("S");
        status.setToolTipText("Storing...");
    }

    protected void createUi(String topLabel, Component control) {
        JPanel center = new JPanel(new FlowLayout());

        control.setEnabled(false);

        /**
         * I guess a nice status enum is coming soon
         */
        center.add(status);

        center.add(control);

        panel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.black),
                BorderFactory.createEmptyBorder(2, 2, 2, 2)));
        panel.add(new JLabel(topLabel), BorderLayout.NORTH);
        panel.add(center, BorderLayout.CENTER);
    }

    public JPanel getContent() {
        return panel;
    }
}