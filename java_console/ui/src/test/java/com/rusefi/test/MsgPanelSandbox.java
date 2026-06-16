package com.rusefi.test;

import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ui.MessagesPane;
import com.rusefi.core.preferences.storage.PersistentConfiguration;

/**
 * Visual sandbox that shows the messages pane.
 */
public class MsgPanelSandbox extends FrameHelper {
    public static void main(String[] args) {
        new FrameHelper().showFrame(new MessagesPane(null, PersistentConfiguration.getConfig().getRoot()).getContent());
    }
}
