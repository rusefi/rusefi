package com.rusefi.ui.basic;

import com.rusefi.*;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.maintenance.StatusAnimation;
import com.rusefi.ui.BasicLogoHelper;
import com.rusefi.ui.util.DefaultExceptionHandler;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;

import java.lang.reflect.InvocationTargetException;

/**
 * Focuses on firmware updater
 * Much simpler than {@link com.rusefi.StartupFrame}
 */
public class BasicStartupFrame {
    private final String whiteLabel = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties());
    private final BasicUpdaterPanel basicUpdaterPanel = new BasicUpdaterPanel(
        ConnectionAndMeta.isDefaultWhitelabel(whiteLabel)
    );
    private final FrameHelper frame = FrameHelper.createFrame(
        whiteLabel + " basic console " + Launcher.CONSOLE_VERSION
    );

    private final StatusAnimation status = new StatusAnimation(this::updateStatus, StartupFrame.SCANNING_PORTS);

    public static void main(String[] args) throws InterruptedException, InvocationTargetException {
        runTool(null);
    }

    public static void runTool(String[] args) throws InterruptedException, InvocationTargetException {
        DefaultExceptionHandler.install();
        SwingUtilities.invokeAndWait(BasicStartupFrame::new);
    }

    public BasicStartupFrame() {
        final JPanel panel = new JPanel();
        panel.add(basicUpdaterPanel);

        SerialPortScanner.INSTANCE.addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
            onHardwareUpdated(currentHardware);
        }));

        BasicLogoHelper.setGenericFrameIcon(frame.getFrame());
        frame.showFrame(panel, false);
        UiUtils.centerWindow(frame.getFrame());
        packFrame();
    }

    private void packFrame() {
        frame.getFrame().pack();
    }

    private void updateStatus(final String niceStatus) {
        basicUpdaterPanel.updateStatus(niceStatus);

        // I'm not sure why it works, but it looks like the following frame packing helps to avoid displaying of logo on
        // the right side of frame
        packFrame();
    }

    public void onHardwareUpdated(final AvailableHardware currentHardware) {
        status.stop();

        basicUpdaterPanel.onHardwareUpdated(currentHardware);

        // I'm not sure if the following frame packing is really necessary, but I'm adding it just in case if frame was
        // not packed in updateStatus method
        packFrame();
    }
}
