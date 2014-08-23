package com.rusefi.test;

import com.rusefi.ui.FrameHelper;
import com.rusefi.ui.MsgPanel;

public class MsgPanelSandbox extends FrameHelper {
    private MsgPanelSandbox() {
        showFrame(new MsgPanel(true).getContent());

    }

    public static void main(String[] args) {

        new MsgPanelSandbox();

    }
}