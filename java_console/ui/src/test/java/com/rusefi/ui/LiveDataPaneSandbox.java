package com.rusefi.ui;

import com.rusefi.ui.util.FrameHelper;

public class LiveDataPaneSandbox {
    public static void main(String[] args) {
        UIContext uiContext = new UIContext();
        new FrameHelper().showFrame(new LiveDataPane(uiContext).getContent());
    }
}
