package com.rusefi.test;

import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.MessagesPane;
import com.rusefi.ui.storage.PersistentConfiguration;

public class MsgPanelSandbox extends FrameHelper {
    private MsgPanelSandbox() {
        showFrame(new MessagesPane(PersistentConfiguration.getConfig().getRoot()).getContent());
    }

    public static void main(String[] args) {
        new MsgPanelSandbox();
    }
}