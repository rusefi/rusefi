package com.rusefi.ui.config;

import com.rusefi.config.Field;

import javax.swing.*;
import java.awt.*;

public class ConfigField {
    private final Field field;

    private final JPanel content = new JPanel(new FlowLayout());
    private final JLabel status = new JLabel("P");
    private final JTextField view = new JTextField();

    public ConfigField(Field field) {
        this.field = field;

        content.add(status);
        status.setToolTipText("Pending...");

        content.add(view);
    }

    public JPanel getContent() {
        return content;
    }
}