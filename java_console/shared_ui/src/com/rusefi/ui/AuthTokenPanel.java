package com.rusefi.ui;

import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class AuthTokenPanel {
    public static final String TOKEN_WARNING = "Please copy token from your forum profile";
    private static final String AUTH_TOKEN = "auth_token";
    private static final String TOKEN_PROFILE_URL = "https://rusefi.com/forum/ucp.php?i=254";

    private JPanel content = new JPanel(new FlowLayout(FlowLayout.LEFT));
    private JTextField textField = new JTextField();

    public AuthTokenPanel() {
        textField.setPreferredSize(new Dimension(200, 24));

        String authToken = getAuthToken();
        System.out.println("Got from settings: " + authToken);

        JButton save = new JButton("Save");
        save.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setAuthToken(AuthTokenPanel.this.textField.getText());
                PersistentConfiguration.getConfig().save();
            }
        });

        content.add(textField);
        content.add(save);

        if (authToken.trim().isEmpty()) {
            authToken = TOKEN_WARNING;
            content.add(new URLLabel("Get it here", TOKEN_PROFILE_URL));
        }
        textField.setText(authToken);
    }

    public static void setAuthToken(String value) {
        getConfig().getRoot().setProperty(AUTH_TOKEN, value);
    }

    @NotNull
    public static String getAuthToken() {
        return getConfig().getRoot().getProperty(AUTH_TOKEN);
    }

    public JPanel getContent() {
        return content;
    }

    public String getToken() {
        return textField.getText();
    }
}
