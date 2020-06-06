package com.rusefi.ui.light;

import com.rusefi.StartupFrame;
import com.rusefi.rusEFIVersion;
import com.rusefi.ui.util.FrameHelper;

import javax.swing.*;
import java.awt.*;

public class LightweightGUI {
    public static void start() {
        FrameHelper frameHelper = new FrameHelper();
        JPanel content = new JPanel(new BorderLayout());
        frameHelper.getFrame().setTitle("rusEFI Lightweight " + rusEFIVersion.CONSOLE_VERSION);

        JPanel topPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        topPanel.add(new InternetStatus().getContent());

        content.add(topPanel, BorderLayout.NORTH);
        content.add(new JLabel(StartupFrame.LINK_TEXT), BorderLayout.CENTER);

        frameHelper.showFrame(content, true);
    }
}
