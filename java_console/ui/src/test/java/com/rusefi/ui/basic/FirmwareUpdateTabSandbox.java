package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.io.DoubleCallbacks;
import com.rusefi.ui.widgets.StatusPanel;

import javax.swing.*;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.ui.basic.UiHelper.commonUiStartup;

public class FirmwareUpdateTabSandbox {
    public static void main(String[] args) {
        commonUiStartup();
        SwingUtilities.invokeLater(FirmwareUpdateTabSandbox::runAwt);
    }

    private static void runAwt() {
        String whiteLabel = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties());

        StatusPanelWithProgressBar statusPanelFirmwareTab = new StatusPanelWithProgressBar();
        StatusPanel statusPanelTuneTab = new StatusPanel(250);
        SingleAsyncJobExecutor singleAsyncJobExecutor = new SingleAsyncJobExecutor(
            new DoubleCallbacks(statusPanelFirmwareTab, statusPanelTuneTab)
        );

        AtomicReference<Optional<PortResult>> ecuPortToUse = new AtomicReference<>(Optional.empty());
        FirmwareUpdateTab firmwareUpdateTab = new FirmwareUpdateTab(
            ConnectivityContext.INSTANCE,
            whiteLabel,
            statusPanelFirmwareTab,
            singleAsyncJobExecutor,
            ecuPortToUse
        );

        FrameHelper frameHelper = new FrameHelper(JDialog.EXIT_ON_CLOSE);
        frameHelper.showFrame(firmwareUpdateTab.getContent());

        int[] progress = {0};
        new Timer(50, e -> {
            statusPanelFirmwareTab.updateProgress(progress[0]);
            progress[0] = (progress[0] + 1) % 101;
        }).start();
    }
}
