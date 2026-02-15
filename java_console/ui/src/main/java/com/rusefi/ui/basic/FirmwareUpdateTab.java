package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.ui.widgets.StatusPanel;

import javax.swing.*;
import java.awt.*;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

public class FirmwareUpdateTab {
    private final JPanel content = new JPanel();
    private final BasicUpdaterPanel basicUpdaterPanel;

    public FirmwareUpdateTab(ConnectivityContext connectivityContext,
                             String whiteLabel,
                             StatusPanel statusPanelFirmwareTab,
                             SingleAsyncJobExecutor singleAsyncJobExecutor,
                             AtomicReference<Optional<PortResult>> ecuPortToUse) {
        basicUpdaterPanel = new BasicUpdaterPanel(connectivityContext,
            com.rusefi.core.net.ConnectionAndMeta.isDefaultWhitelabel(whiteLabel),
            statusPanelFirmwareTab,
            singleAsyncJobExecutor, ecuPortToUse
        );
        content.add(basicUpdaterPanel.getContent());
        content.add(statusPanelFirmwareTab);
    }

    public BasicUpdaterPanel getBasicUpdaterPanel() {
        return basicUpdaterPanel;
    }

    public JPanel getContent() {
        return content;
    }
}
