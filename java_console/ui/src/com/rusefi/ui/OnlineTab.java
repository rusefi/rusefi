package com.rusefi.ui;

import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

import java.awt.*;
import java.awt.event.ActionEvent;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class OnlineTab {
    private static final String AUTH_TOKEN = "auth_token";

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public OnlineTab() {
        JTextField textField = new JTextField();
        textField.setPreferredSize(new Dimension(200, 24));

        textField.setText(getConfig().getRoot().getProperty(AUTH_TOKEN));

        content.add(textField);

        JButton save = new JButton("Save");
        save.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                getConfig().getRoot().setProperty(AUTH_TOKEN, textField.getText());
            }
        });
        content.add(save);
    }

    public JPanel getContent() {
        return content;
    }
}
