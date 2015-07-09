package com.rusefi.ui.config;

import com.rusefi.config.Field;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Pair;
import com.rusefi.ui.util.JTextFieldWithWidth;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

public class ConfigField extends BaseConfigField {
    private final JTextField view = new JTextFieldWithWidth(200);

    public ConfigField(final Field field, String topLabel) {
        super(field);
        createUi(topLabel, view);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (Field.isIntValueMessage(message) || Field.isFloatValueMessage(message)) {
                    Pair<Integer, ?> p = Field.parseResponse(message);
                    if (p != null && p.first == field.getOffset()) {
                        view.setEnabled(true);
                        view.setText("" + p.second);
                        onValueArrived();
                    }
                }
            }
        });

        view.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    sendValue(field, ConfigField.this.view.getText());
                }
            }
        });
    }

}