package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.IndicatorPanel;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;
import com.rusefi.ui.widgets.tune.TuningToolbarWidget;

import javax.swing.*;
import java.awt.*;
import java.util.HashMap;
import java.util.ArrayDeque;
import java.util.List;

/**
 * Andrey Belomutskiy, (c) 2013-2026
 */
public class TuningPane {
    private final JPanel content = new JPanel(new BorderLayout());

    public TuningPane(UIContext uiContext) {
        MainMenuTreeWidget left = new MainMenuTreeWidget(uiContext);

        CalibrationDialogWidget right = new CalibrationDialogWidget(uiContext);
        JScrollPane rightScrollPane = new JScrollPane(right.getContentPane());

        final String[] currentKey = {null};

        // Accumulated tune edits across all dialogs for this session.
        final ConfigurationImage[] sessionImage = {null};

        TuningToolbarWidget toolbar = new TuningToolbarWidget(uiContext, right, currentKey, sessionImage);

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, left.getContentPane(), rightScrollPane);
        splitPane.setResizeWeight(0.3);

        left.setOnSelect(subMenu -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null || bp.getControllerConfiguration() == null) {
                return;
            }

            // Flush any in-progress debounce window before loading a new section.
            toolbar.flushBeforeNavigate();

            // On first navigation, seed the session image from the live ECU state.
            // On subsequent navigations, carry forward whatever the user has edited so far
            // so that changes made in dialogs, tables, or curves are not lost when opening another.
            ConfigurationImage pending = right.getWorkingImage();
            if (pending != null) {
                sessionImage[0] = pending;
            } else if (sessionImage[0] == null) {
                sessionImage[0] = bp.getControllerConfiguration().clone();
            }

            currentKey[0] = subMenu.getKey();
            right.update(subMenu.getKey(), uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
        });

        // All edit events (dialog fields, table cells, curve drags) flow through onConfigChange.
        // Text fields fire per-keystroke; the toolbar widget coalesces them into one undo point.
        right.setOnConfigChange(image -> {
            toolbar.onEdit(sessionImage[0]);
            // Clone because workingImage is mutated in-place by further edits.
            sessionImage[0] = image.clone();
            left.refreshExpressions(image);
            uiContext.fireConfigImageChanged(image);
        });

        // When the ECU disconnects (e.g. after a firmware flash or board swap), drop all stale
        // session state so the next connection re-reads calibrations fresh from the new board.
        // Without this, sessionImage/workingImage from the old board (possibly a different config
        // page size) would be used as the diff baseline in uploadChangesWithoutBurn.
        // currentKey is preserved so that reconnection can automatically re-render the last section.
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> {
            if (!isConnected) {
                SwingUtilities.invokeLater(() -> {
                    toolbar.onDisconnect();
                    right.reset();
                    sessionImage[0] = null;
                });
            } else {
                SwingUtilities.invokeLater(() -> {
                    if (currentKey[0] == null) return;
                    BinaryProtocol bp = uiContext.getBinaryProtocol();
                    if (bp == null || bp.getControllerConfiguration() == null) return;
                    sessionImage[0] = bp.getControllerConfiguration().clone();
                    right.update(currentKey[0], uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
                });
            }
        });

        JPanel northPanel = new JPanel();
        northPanel.setLayout(new BoxLayout(northPanel, BoxLayout.Y_AXIS));
        northPanel.add(toolbar.getPanel());
        JPanel indicatorPanel = buildFrontendIndicatorPanel(uiContext);
        if (indicatorPanel != null) {
            northPanel.add(indicatorPanel);
        }

        content.add(northPanel, BorderLayout.NORTH);
        content.add(splitPane, BorderLayout.CENTER);
    }


    private static JPanel buildFrontendIndicatorPanel(UIContext uiContext) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null) return null;
        FrontPageModel frontPage = ini.getFrontPage();
        if (frontPage == null) return null;
        List<IndicatorModel> indicators = frontPage.getIndicators();
        if (indicators.isEmpty()) return null;
        return new IndicatorPanel(indicators, ini, 0).getPanel();
    }

    public JPanel getContent() {
        return content;
    }
}
