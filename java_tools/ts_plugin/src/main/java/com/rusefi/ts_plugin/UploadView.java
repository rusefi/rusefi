package com.rusefi.ts_plugin;

import com.rusefi.tools.online.UploadResult;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
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
        content.add(autoUpload);

        autoUpload.setSelected(isAutoUpload());
        autoUpload.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                PersistentConfiguration.getConfig().getRoot().setProperty(AUTO_UPLOAD, autoUpload.isSelected());
                PersistentConfiguration.getConfig().save();
            }
        });
        uploadState.setVisible(false);
    }

    public static boolean isAutoUpload() {
        return PersistentConfiguration.getConfig().getRoot().getBoolProperty(AUTO_UPLOAD, false);
    }

    public static void setResult(UploadResult result, JLabel uploadState) {
        uploadState.setText(result.getFirstMessage());
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
