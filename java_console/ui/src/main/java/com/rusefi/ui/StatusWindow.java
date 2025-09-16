package com.rusefi.ui;

import com.rusefi.core.ui.FrameHelper;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.StatusPanel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/7/2015
 */
public class StatusWindow {
    // todo: extract driver from console bundle? find a separate driver bundle?
    StatusPanel content = new StatusPanel(400);

    public static UpdateOperationCallbacks createAndShowFrame(final String frameTitle) {
        final StatusWindow statusWindow = new StatusWindow();
        statusWindow.showFrame(frameTitle);
        return statusWindow.getContent();
    }

    @NotNull
    protected final FrameHelper frameHelper = new FrameHelper();

    public StatusWindow() {
    }

    @NotNull
    public FrameHelper getFrameHelper() {
        return frameHelper;
    }

    public StatusPanel getContent() {
        return content;
    }

    public JFrame getFrame() {
        return frameHelper.getFrame();
    }

    public void showFrame(String title) {
        frameHelper.getFrame().setTitle(title);
        frameHelper.showFrame(content, false);
        UiUtils.centerWindow(frameHelper.getFrame());
        content.clear(); // let's remove stuff from previous invocation
    }
}
