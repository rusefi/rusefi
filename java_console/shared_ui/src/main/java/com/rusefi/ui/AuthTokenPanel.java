package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.auth.AuthTokenUtil;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.awt.datatransfer.*;
import java.awt.event.ActionEvent;
import java.io.IOException;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class AuthTokenPanel {
    private final static Logging log = Logging.getLogging(AuthTokenPanel.class);

    private final JPanel content = new JPanel(new BorderLayout());
    private final JTextField authTokenTestField = new JTextField();

    public AuthTokenPanel() {

        JPanel top = new JPanel(new FlowLayout(FlowLayout.LEFT));

        content.setBorder(BorderFactory.createTitledBorder("rusEFI Online Authentication Token"));

        authTokenTestField.setPreferredSize(new Dimension(200, 24));

        String authToken = getAuthToken();
        System.out.println("Got from settings: " + authToken);

        authTokenTestField.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                onTextChange();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                onTextChange();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                onTextChange();
            }
        });

        final Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();

        JButton paste = new JButton("Paste from clipboard");
        paste.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String data = (String) clipboard.getData(DataFlavor.stringFlavor);
                    if (AuthTokenUtil.isToken(data)) {
                        authTokenTestField.setText(data);
                    }
                } catch (IOException | UnsupportedFlavorException ex) {
                    // ignoring this exception
                }
            }
        });

        clipboard.addFlavorListener(e -> {
            setPasteButtonEnabledBasedOnClipboardContent(clipboard, paste);
        });

        setPasteButtonEnabledBasedOnClipboardContent(clipboard, paste);

        top.add(authTokenTestField);
        top.add(paste);
/*
        JButton save = new JButton("Save");
        save.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                grabText();
            }
        });
        top.add(save);
*/
        content.add(top);
        if (authToken.trim().isEmpty()) {
            authToken = AuthTokenUtil.TOKEN_WARNING;
        }
        content.add(new URLLabel("Manage authentication token at your forum profile", AuthTokenUtil.TOKEN_PROFILE_URL), BorderLayout.SOUTH);
        authTokenTestField.setText(authToken);
    }

    public static void setAuthToken(String value) {
        getConfig().getRoot().setProperty(AuthTokenUtil.AUTH_TOKEN, value);
    }

    @NotNull
    public static String getAuthToken() {
        return getConfig().getRoot().getProperty(AuthTokenUtil.AUTH_TOKEN);
    }

    private void setPasteButtonEnabledBasedOnClipboardContent(Clipboard clipboard, JButton paste) {
        try {
            String data = (String) clipboard.getData(DataFlavor.stringFlavor);
            paste.setEnabled(AuthTokenUtil.isToken(data));
        } catch (IOException | IllegalStateException | UnsupportedFlavorException ex) {
            log.info("Ignoring " + ex);
        }
    }

    private void persistToken() {
        setAuthToken(AuthTokenPanel.this.authTokenTestField.getText());
        PersistentConfiguration.getConfig().save();
    }

    private void onTextChange() {
        if (AuthTokenUtil.isToken(authTokenTestField.getText())) {
            persistToken();
        }
    }

    public JPanel getContent() {
        return content;
    }

    public static boolean hasToken() {
        return AuthTokenUtil.isToken(getAuthToken());
    }

    public String getToken() {
        return authTokenTestField.getText();
    }

    public static void showError(JComponent parent) {
        JOptionPane.showMessageDialog(parent, "Does not work without auth token, see below.");
    }
}
