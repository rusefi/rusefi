package com.rusefi.ui.config;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.Fields;
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
        requestInitialValue(field); // this is not in base constructor so that view is created by the time we invoke it

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (Field.isBitValueMessage(message)) {
                    String expectedPrefix = "bit" +
                            Fields.CONSOLE_DATA_PROTOCOL_TAG + field.getOffset() + "/" + field.getBitOffset() + " is ";
                    if (message.startsWith(expectedPrefix) && message.length() == expectedPrefix.length() + 1) {
                        message = message.substring(expectedPrefix.length());
                        Boolean value = message.equals("1");
                        setValue(value);
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

    private void setValue(boolean value) {
        ec = true;
        view.setEnabled(true);
        view.setSelected(value);
        onValueArrived();
        ec = false;
    }

    @Override
    protected void loadValue(ConfigurationImage ci) {
        int bits = getByteBuffer(ci).getInt();
        boolean bit = ((bits >> field.getBitOffset()) & 1) == 1;
        setValue(bit);
    }
}