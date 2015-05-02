package com.rusefi.ui.config;

import com.rusefi.config.Field;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Pair;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;

public class EnumConfigField extends BaseConfigField {
    private final JComboBox<String> view = new JComboBox<>();
    private boolean ec;
    private final Map<String, Integer> ordinals = new HashMap<>();

    public EnumConfigField(final Field field, String caption) {
        this(field, caption, field.getOptions());
    }

    public EnumConfigField(final Field field, String caption, final String... options) {
        super(field);

        int ordinal = 0;
        for (String option : options) {
            ordinals.put(option, ordinal++);

            if (!"invalid".equalsIgnoreCase(option))
                view.addItem(option);
        }

        createUi(caption, view);

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (Field.isIntValueMessage(message)) {
                    Pair<Integer, ?> p = Field.parseResponse(message);
                    if (p != null && p.first == field.getOffset()) {
                        int ordinal = (Integer) p.second;
                        if (ordinal >= options.length)
                            throw new IllegalStateException("Unexpected ordinal " + ordinal + " for " + field);
                        ec = true;
                        view.setEnabled(true);
                        view.setSelectedItem(options[ordinal]);
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
                String value = (String) view.getSelectedItem();
                int ordinal = ordinals.get(value);
                sendValue(field, Integer.toString(ordinal));
            }
        });
    }
}