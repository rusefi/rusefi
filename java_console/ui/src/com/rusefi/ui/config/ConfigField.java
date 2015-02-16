package com.rusefi.ui.config;

import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Pair;
import com.rusefi.io.CommandQueue;
import com.rusefi.ui.ConnectionStatus;
import com.rusefi.ui.util.JTextFieldWithWidth;

import javax.swing.*;
import java.awt.*;

public class ConfigField {
    private final Field field;

    private final JPanel panel = new JPanel(new BorderLayout());
    private final JLabel status = new JLabel("P");
    private final JTextField view = new JTextFieldWithWidth(200);

    public ConfigField(final Field field, String topLabel) {
        this.field = field;

        ConnectionStatus.INSTANCE.addListener(new ConnectionStatus.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                CommandQueue.getInstance().write(field.getType().getCommand() + " " + field.getOffset());
            }
        });

        JPanel center = new JPanel(new FlowLayout());

        center.add(status);
        status.setToolTipText("Pending...");

        center.add(view);

        panel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.black),
                BorderFactory.createEmptyBorder(2, 2, 2, 2)));
        panel.add(new JLabel(topLabel), BorderLayout.NORTH);
        panel.add(center, BorderLayout.CENTER);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (Field.isIntValueMessage(message) || Field.isFloatValueMessage(message) ) {
                    Pair<Integer, ?> p = Field.parseResponse(message);
                    if (p != null && p.first == field.getOffset()) {
                        view.setText("" + p.second);
                        status.setText("");
                        status.setToolTipText(null);
                    }
                }
            }
        });
    }

    public JPanel getContent() {
        return panel;
    }
}