package com.rusefi.test;

import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ui.MessagesPane;
import com.rusefi.core.preferences.storage.PersistentConfiguration;

public class MsgPanelSandbox extends FrameHelper {
    public static void main(String[] args) {
        new FrameHelper().showFrame(new MessagesPane(null, PersistentConfiguration.getConfig().getRoot()).getContent());
    }
}