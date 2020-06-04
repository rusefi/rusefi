package com.rusefi.ui;

import com.rusefi.io.LinkManager;
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.util.Misc;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import javax.xml.bind.JAXBException;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;

import static com.rusefi.ui.AuthTokenPanel.TOKEN_WARNING;
import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class OnlineTab {

    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public OnlineTab() {
        AuthTokenPanel authTokenPanel = new AuthTokenPanel();

        content.add(Misc.getRusEFI_online_manual());

        content.add(authTokenPanel.getContent());

        JButton upload = new JButton("Upload");
        upload.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String text = authTokenPanel.getToken();
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
