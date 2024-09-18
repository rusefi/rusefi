package com.rusefi.core.ui;

import javax.swing.*;

class ProgressView {
    private final FrameHelper frameHelper;
    private final JProgressBar progressBar;

    public JProgressBar getProgressBar() {
        return progressBar;
    }

    ProgressView(FrameHelper frameHelper, JProgressBar progressBar) {
        this.frameHelper = frameHelper;
        this.progressBar = progressBar;
    }

    public void dispose() {
        if (frameHelper != null) {
            frameHelper.getFrame().dispose();
        }
    }
}
