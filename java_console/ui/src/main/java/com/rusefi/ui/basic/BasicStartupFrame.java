package com.rusefi.ui.basic;

import com.rusefi.Launcher;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.maintenance.UpdateStatusWindow;
import com.rusefi.ui.LogoHelper;
import com.rusefi.ui.util.HorizontalLine;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.ToolButtons;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

import static com.rusefi.FileLog.isWindows;

/**
 * Focuses on firmware updater
 * Much simpler than {@link com.rusefi.StartupFrame}
 */
public class BasicStartupFrame {
    private final FrameHelper frame;

    public static void runTool(String[] args) {
        new BasicStartupFrame().runTool();
    }

    public BasicStartupFrame() {
        String title = "rusEFI basic console " + Launcher.CONSOLE_VERSION;
        frame = FrameHelper.createFrame(title);
        JPanel panel = new JPanel(new VerticalFlowLayout());
        if (isWindows()) {
            panel.add(ToolButtons.createShowDeviceManagerButton());

            JButton update = ProgramSelector.createUpdateFirmwareButton();
            update.addActionListener(e -> DfuFlasher.doAutoDfu(update, PortDetector.AUTO, new UpdateStatusWindow("Update")));
            panel.add(update);
        } else {
            panel.add(new JLabel("Sorry only works on Windows"));
        }

        panel.add(new HorizontalLine());
        JLabel logoLabel = LogoHelper.createLogoLabel();
        if (logoLabel != null)
            panel.add(logoLabel);
        panel.add(LogoHelper.createUrlLabel());

        frame.showFrame(panel, false);
        UiUtils.centerWindow(frame.getFrame());
    }

    private void runTool() {
    }
}
