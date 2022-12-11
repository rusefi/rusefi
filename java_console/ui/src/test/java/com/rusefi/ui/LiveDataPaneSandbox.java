package com.rusefi.ui;

import com.rusefi.core.ui.FrameHelper;

import javax.swing.*;

public class LiveDataPaneSandbox {
    public static void main(String[] args) {
        UIContext uiContext = new UIContext();
        InitOnFirstPaintPanel panel = LiveDataPane.createLazy(uiContext);

        new FrameHelper(WindowConstants.EXIT_ON_CLOSE).showFrame(panel.getContent());
    }
}
