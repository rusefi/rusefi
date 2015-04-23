package com.rusefi.ui.config;

import com.rusefi.config.Field;
import com.rusefi.core.MessagesCentral;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class BitConfigField extends BaseConfigField {
    private final JCheckBox view = new JCheckBox();
    private boolean ec;

    public BitConfigField(final Field field, String caption) {
        super(field);

        createUi(caption, view);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (Field.isBitValueMessage(message)) {
                    String expectedPrefix = "bit @" + field.getOffset() + "/" + field.getBitOffset() + " is ";
                    if (message.startsWith(expectedPrefix) && message.length() == expectedPrefix.length() + 1) {
                        message = message.substring(expectedPrefix.length());
                        System.out.println("Bit arrived " + message);
                        Boolean value = message.equals("1");
                        ec = true;
                        view.setSelected(value);
                        onValueArrived();
                        ec = false;
                    }
                }
            }
        });

        view.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (ec)
                    return;
                sendValue(field, view.isSelected() ? "1" : "0");
            }
        });
    }
}