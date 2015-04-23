package com.rusefi.ui.config;

import com.rusefi.config.Field;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Pair;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class EnumConfigField extends BaseConfigField {
    private final JComboBox<String> view = new JComboBox<>();
    private boolean ec;

    public EnumConfigField(final Field field, String caption, String... options) {
        super(field);

        for (String option : options)
            view.addItem(option);

        createUi(caption, view);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (Field.isIntValueMessage(message)) {
                    Pair<Integer, ?> p = Field.parseResponse(message);
                    if (p != null && p.first == field.getOffset()) {
                        int value = (Integer) p.second;
                        ec = true;
                        view.setSelectedIndex(value);
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
                sendValue(field, Integer.toString(view.getSelectedIndex()));
            }
        });
    }
}