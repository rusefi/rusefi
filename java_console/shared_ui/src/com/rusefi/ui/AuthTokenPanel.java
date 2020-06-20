package com.rusefi.ui;

import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class AuthTokenPanel {
    private static final String TOKEN_WARNING = "Please copy token from your forum profile";
    private static final String TOKEN_SUBSTRING = "token";
    private static final String AUTH_TOKEN = "auth_token";
    private static final String TOKEN_PROFILE_URL = "https://rusefi.com/forum/ucp.php?i=254";

    private JPanel content = new JPanel(new BorderLayout());
    private JTextField textField = new JTextField();

    public AuthTokenPanel() {

        JPanel top = new JPanel(new FlowLayout(FlowLayout.LEFT));

        content.setBorder(BorderFactory.createTitledBorder("rusEFI Online Authentication Token"));

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

        top.add(textField);
        top.add(save);

        content.add(top);
        if (authToken.trim().isEmpty()) {
            authToken = TOKEN_WARNING;
            content.add(new URLLabel("Get your token here", TOKEN_PROFILE_URL), BorderLayout.SOUTH);
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

    public boolean hasToken() {
        return textField.getText().trim().length() > 0 && !textField.getText().contains(TOKEN_SUBSTRING);
    }

    public String getToken() {
        return textField.getText();
    }

    public void showError(JComponent parent) {
        JOptionPane.showMessageDialog(parent, "Does not work without auth token, see below.");
    }
}
