package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.ui.widgets.StatusPanel;

import javax.swing.*;
import java.awt.*;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

public class FirmwareUpdateTab {
    private final JPanel content = new JPanel(new BorderLayout());
    private final BasicUpdaterPanel basicUpdaterPanel;
    private final JProgressBar progressBar = new JProgressBar();

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

        progressBar.setIndeterminate(false);
        progressBar.setStringPainted(true);
        progressBar.setBorder(BorderFactory.createLineBorder(Color.RED));

        JPanel statusAndProgress = new JPanel();
        statusAndProgress.setLayout(new BoxLayout(statusAndProgress, BoxLayout.Y_AXIS));
        statusAndProgress.add(statusPanelFirmwareTab);
        statusAndProgress.add(progressBar);

        content.add(basicUpdaterPanel.getContent(), BorderLayout.WEST);
        content.add(statusAndProgress, BorderLayout.EAST);
    }

    public BasicUpdaterPanel getBasicUpdaterPanel() {
        return basicUpdaterPanel;
    }

    public JPanel getContent() {
        return content;
    }
}
