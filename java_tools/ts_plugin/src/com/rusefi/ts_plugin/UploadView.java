package com.rusefi.ts_plugin;

import com.rusefi.tools.online.UploadResult;
import com.rusefi.ui.storage.PersistentConfiguration;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class UploadView {
    private final JComponent content = new JPanel(new VerticalFlowLayout());

    private static final String AUTO_UPLOAD = "AUTO_UPLOAD";

    public final JLabel uploadState = new JLabel();
    private final JLabel projectWarning = new JLabel(UploaderStatus.NO_PROJECT);
    private final JLabel tuneInfo = new JLabel();
    private final JCheckBox autoUpload = new JCheckBox("Continuous auto-upload");

    public UploadView() {
        content.add(projectWarning);
        content.add(tuneInfo);
        content.add(uploadState);

        autoUpload.setSelected(isAutoUpload());
        autoUpload.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                PersistentConfiguration.getConfig().getRoot().setProperty(AUTO_UPLOAD, autoUpload.isSelected());
            }
        });
        uploadState.setVisible(false);
    }

    public static boolean isAutoUpload() {
        return PersistentConfiguration.getConfig().getRoot().getBoolProperty(AUTO_UPLOAD, false);
    }

    public void setResult(UploadResult result) {
        uploadState.setText(result.getMessage().get(0).toString());
        uploadState.setVisible(true);
    }

    public JComponent getContent() {
        return content;
    }

    public void update(UploaderStatus uploaderStatus) {
        if (uploaderStatus.isTuneOk()) {
            tuneInfo.setText(uploaderStatus.tuneInfo);
            tuneInfo.setForeground(Color.black);
        } else {
            tuneInfo.setText(uploaderStatus.tuneWarning);
            tuneInfo.setForeground(Color.red);
        }

        if (uploaderStatus.isProjectIsOk()) {
            projectWarning.setVisible(false);
        } else {
            projectWarning.setVisible(true);
            projectWarning.setText(uploaderStatus.projectWarning);
        }
    }
}
