package com.rusefi.ui.basic;

import com.rusefi.*;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.io.DoubleCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.StatusAnimation;
import com.rusefi.tools.TunerStudioHelper;
import com.rusefi.ui.BasicLogoHelper;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.StatusPanel;

import javax.swing.*;

import java.lang.reflect.InvocationTargetException;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

/**
 * java -jar rusefi_console.jar basic-ui
 * java -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005 -jar rusefi_console.jar basic-ui
 * <p>
 * Focuses on firmware updater
 * Much simpler than {@link com.rusefi.StartupFrame}
 */
public class BasicStartupFrame {
    private final String whiteLabel = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties());

    private final BasicUpdaterPanel basicUpdaterPanel;
    private final FrameHelper frame = FrameHelper.createFrame(
        whiteLabel + " basic console " + Launcher.CONSOLE_VERSION
    );

    private final StatusAnimation status;

    public static void main(String[] args) throws InterruptedException, InvocationTargetException {
        runTool(null);
    }

    public static void runTool(@SuppressWarnings("unused") String[] args) throws InterruptedException, InvocationTargetException {
        UiHelper.commonUiStartup();
        SwingUtilities.invokeAndWait(() -> new BasicStartupFrame(ConnectivityContext.INSTANCE));
    }

    private BasicStartupFrame(ConnectivityContext connectivityContext) {
        DfuFlasher.dfuEnabledInCaseOfError = false;
        TunerStudioHelper.maybeCloseTs();

        JTabbedPane tabbedPane = new JTabbedPane();
        final JPanel firmwareUpdateContent = new JPanel();
        StatusPanel statusPanelFirmwareTab = new StatusPanel(500);
        StatusPanel statusPanelTuneTab = new StatusPanel(250);
        SingleAsyncJobExecutor singleAsyncJobExecutor = new SingleAsyncJobExecutor(new DoubleCallbacks(statusPanelFirmwareTab, statusPanelTuneTab));

        AtomicReference<Optional<PortResult>> ecuPortToUse = new AtomicReference<>(Optional.empty());
        basicUpdaterPanel = new BasicUpdaterPanel(connectivityContext,
            ConnectionAndMeta.isDefaultWhitelabel(whiteLabel),
            statusPanelFirmwareTab,
            singleAsyncJobExecutor, ecuPortToUse
        );
        firmwareUpdateContent.add(basicUpdaterPanel.getContent());
        firmwareUpdateContent.add(statusPanelFirmwareTab);

        connectivityContext.getSerialPortScanner().addListener(currentHardware -> SwingUtilities.invokeLater(() -> {
            onHardwareUpdated();
        }));

        tabbedPane.addTab("Firmware", firmwareUpdateContent);
        tabbedPane.addTab("Tunes", new TuneManagementTab(
            connectivityContext,
            ecuPortToUse,
            basicUpdaterPanel.getImportTuneButton().getContent(),
            singleAsyncJobExecutor,
            statusPanelTuneTab
        ).getContent());
        tabbedPane.addTab("Trims", new TrimsTab(
            connectivityContext,
            ecuPortToUse
        ).getContent());

        BasicLogoHelper.setGenericFrameIcon(frame.getFrame());
        frame.showFrame(tabbedPane, false);
        UiUtils.centerWindow(frame.getFrame());
        packFrame();

        status = new StatusAnimation(this::updateStatus, StartupFrame.SCANNING_PORTS);
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

    public void onHardwareUpdated() {
        status.stop();

        basicUpdaterPanel.onHardwareUpdated();

        // I'm not sure if the following frame packing is really necessary, but I'm adding it just in case if frame was
        // not packed in updateStatus method
        packFrame();
    }
}
