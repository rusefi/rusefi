package com.rusefi.ts_plugin;

import org.json.simple.JSONArray;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;

public class UploadView {
    private final JComponent content = new JPanel(new VerticalFlowLayout());

    private final JLabel uploadState = new JLabel();
    private final JLabel projectWarning = new JLabel(UploaderStatus.NO_PROJECT);
    private final JLabel tuneInfo = new JLabel();

    public UploadView() {
        content.add(projectWarning);
        content.add(tuneInfo);
        content.add(uploadState);

        uploadState.setVisible(false);
    }

    public void setResult(JSONArray array) {
        uploadState.setText(array.get(0).toString());
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
