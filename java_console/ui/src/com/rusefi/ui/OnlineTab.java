package com.rusefi.ui;

import com.rusefi.io.LinkManager;
import com.rusefi.tools.ConsoleTools;
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Msq;
import com.rusefi.xml.XmlUtil;
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


        JButton upload = new JButton("Upload");
        upload.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Msq tune = ConsoleTools.toMsq(LinkManager.connector.getBinaryProtocol().getControllerConfiguration());
                try {
                    XmlUtil.writeXml(tune, Msq.class, Msq.outputXmlFileName);
                    // todo: network upload should not happen on UI thread
                    Online.upload(new File(Msq.outputXmlFileName), textField.getText());
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
