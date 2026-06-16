package com.rusefi.ui;

import com.rusefi.core.ui.FrameHelper;

import javax.swing.*;

/**
 * Visual sandbox that shows the live-data pane.
 */
public class LiveDataPaneSandbox {
    public static void main(String[] args) {
        UIContext uiContext = new UIContext();
        InitOnFirstPaintPanel panel = LiveDataPane.createLazy(uiContext);

        new FrameHelper().showFrame(panel.getContent());
    }
}
