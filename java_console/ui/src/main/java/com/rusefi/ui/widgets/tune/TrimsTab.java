package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.PageReader;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.BinaryProtocolExecutor;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;
import java.util.Objects;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

public class TrimsTab {
    private final JPanel content = new JPanel(new BorderLayout());
    private final TuningTableView view1 = new TuningTableView("Bank 1");
    private final TuningTableView view2 = new TuningTableView("Bank 2");
    private final ConnectivityContext connectivityContext;
    private final AtomicReference<Optional<PortResult>> ecuPortToUse;

    public TrimsTab(ConnectivityContext connectivityContext,
                    AtomicReference<Optional<PortResult>> ecuPortToUse) {
        this.connectivityContext = connectivityContext;
        this.ecuPortToUse = ecuPortToUse;

        JPanel tablesPanel = new JPanel();
        tablesPanel.setLayout(new BoxLayout(tablesPanel, BoxLayout.Y_AXIS));
        tablesPanel.add(view1.getContent());
        tablesPanel.add(Box.createVerticalStrut(20));
        tablesPanel.add(view2.getContent());

        content.add(new JScrollPane(tablesPanel), BorderLayout.CENTER);

        content.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 && content.isShowing()) {
                loadTrims();
            }
        });
    }

    private void loadTrims() {
        Optional<PortResult> portResult = ecuPortToUse.get();
        if (!portResult.isPresent()) {
            return;
        }

        new Thread(() -> {
            try {
                Optional<CalibrationsInfo> info = CalibrationsHelper.readCurrentCalibrations(
                        portResult.get().port,
                        UpdateOperationCallbacks.DUMMY,
                        connectivityContext
                );

                if (info.isPresent()) {
                    String port = portResult.get().port;
                    CalibrationsInfo calibInfo = info.get();

                    Optional<byte[]> ltftData = BinaryProtocolExecutor.executeWithSuspendedPortScanner(
                        port,
                        UpdateOperationCallbacks.DUMMY,
                        (binaryProtocol) -> {

                            // TODO: get this from the .ini file
                            int iniPageNumber = 3;
                            int page3Size = calibInfo.getIniFile().getMetaInfo().getPageSize(iniPageNumber - 1);
                            ConfigurationImageWithMeta page3 = PageReader.readPageData(binaryProtocol, iniPageNumber, page3Size,
                                calibInfo.getIniFile().getMetaInfo().getSignature());

                            if (page3 != null && !page3.isEmpty()) {
                                return Optional.of(Objects.requireNonNull(page3.getConfigurationImage()).getContent());
                            }
                            return Optional.empty();
                        },
                        Optional.empty(),
                        connectivityContext,
                        "read LTFT"
                    );

                    if (ltftData.isPresent()) {
                        SwingUtilities.invokeLater(() -> {
                            displayTrims(calibInfo, ltftData.get());
                        });
                    }
                }
            } finally {
            }
        }).start();
    }

    private void displayTrims(CalibrationsInfo info, byte[] zBinsBuffer) {
        // Page 1 contains the axis bins (RPM and load)
        ConfigurationImage page1Image = info.getImage().getConfigurationImage();

        view1.displayTable(info, zBinsBuffer, page1Image, "ltftBank1Tbl");
        view2.displayTable(info, zBinsBuffer, page1Image, "ltftBank2Tbl");
    }

    public Component getContent() {
        return content;
    }
}
