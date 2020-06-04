package com.rusefi.ui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class AuthTokenPanel {
    public static final String TOKEN_WARNING = "Please copy token from your forum profile";
    public static final String AUTH_TOKEN = "auth_token";

    private JPanel content = new JPanel(new FlowLayout(FlowLayout.LEFT));
    private JTextField textField = new JTextField();

    public AuthTokenPanel() {
        textField.setPreferredSize(new Dimension(200, 24));

        String authToken = getConfig().getRoot().getProperty(AUTH_TOKEN);
        if (authToken.trim().isEmpty())
            authToken = TOKEN_WARNING;

        textField.setText(authToken);

        JButton save = new JButton("Save");
        save.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                getConfig().getRoot().setProperty(AUTH_TOKEN, textField.getText());
            }
        });


        content.add(textField);
        content.add(save);

    }

    public JPanel getContent() {
        return content;
    }

    public String getToken() {
        return textField.getText();
    }
}
