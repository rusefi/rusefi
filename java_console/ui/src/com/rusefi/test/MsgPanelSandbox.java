package com.rusefi.test;

import com.rusefi.ui.FrameHelper;
import com.rusefi.ui.MessagesPane;
import com.rusefi.ui.MessagesPanel;

public class MsgPanelSandbox extends FrameHelper {
    private MsgPanelSandbox() {
        showFrame(new MessagesPane().getContent());
    }

    public static void main(String[] args) {
        new MsgPanelSandbox();
    }
}