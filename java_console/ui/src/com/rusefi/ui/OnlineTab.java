package com.rusefi.ui;

import com.rusefi.io.LinkManager;
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.util.URLLabel;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import javax.xml.bind.JAXBException;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class OnlineTab {
    private static final String AUTH_TOKEN = "auth_token";
    private static final String TOKEN_WARNING = "Please copy token from your forum profile";

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public OnlineTab() {
        JTextField textField = new JTextField();
        textField.setPreferredSize(new Dimension(200, 24));

        String authToken = getConfig().getRoot().getProperty(AUTH_TOKEN);
        if (authToken.trim().isEmpty())
            authToken = TOKEN_WARNING;

        textField.setText(authToken);

        content.add(new URLLabel("rusEFI Online manual", "https://github.com/rusefi/rusefi/wiki/Online"));

        content.add(textField);

        JButton save = new JButton("Save");
        save.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                getConfig().getRoot().setProperty(AUTH_TOKEN, textField.getText());
            }
        });
        content.add(save);


        JButton upload = new JButton("Upload");
        upload.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String text = textField.getText();
                if (text.contains(TOKEN_WARNING)) {
                    JOptionPane.showMessageDialog(content, "Does not work without auth token");
                    return;
                }
                Msq tune = Msq.toMsq(LinkManager.connector.getBinaryProtocol().getControllerConfiguration());
                try {
                    tune.writeXmlFile(Msq.outputXmlFileName);
                    // todo: network upload should not happen on UI thread
                    Online.upload(new File(Msq.outputXmlFileName), text);
                } catch (JAXBException | IOException ex) {
                    throw new IllegalStateException(ex);
                }
            }
        });
        content.add(upload);
    }

    public JPanel getContent() {
        return content;
    }
}
