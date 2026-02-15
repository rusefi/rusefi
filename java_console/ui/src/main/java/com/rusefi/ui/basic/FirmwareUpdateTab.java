package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.core.net.ConnectionAndMeta;

import javax.swing.*;
import java.awt.*;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

public class FirmwareUpdateTab {
    private final JPanel content = new JPanel(new BorderLayout());
    private final BasicUpdaterPanel basicUpdaterPanel;

    public FirmwareUpdateTab(ConnectivityContext connectivityContext,
                             String whiteLabel,
                             StatusPanelWithProgressBar statusPanelFirmwareTab,
                             SingleAsyncJobExecutor singleAsyncJobExecutor,
                             AtomicReference<Optional<PortResult>> ecuPortToUse) {
        basicUpdaterPanel = new BasicUpdaterPanel(connectivityContext,
            ConnectionAndMeta.isDefaultWhitelabel(whiteLabel),
            statusPanelFirmwareTab,
            singleAsyncJobExecutor, ecuPortToUse
        );


        content.add(basicUpdaterPanel.getContent(), BorderLayout.WEST);
        content.add(statusPanelFirmwareTab.getContent(), BorderLayout.EAST);
    }

    public BasicUpdaterPanel getBasicUpdaterPanel() {
        return basicUpdaterPanel;
    }

    public JPanel getContent() {
        return content;
    }
}
