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
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

/**
 * Andrey Belomutskiy, (c) 2013-2026
 */
public class TuningPane {
    private final JPanel content = new JPanel(new BorderLayout());
    private final MainMenuTreeWidget left;
    /** Fired when the user picks "Show in Pinout" on a pin-enum field. Wired from ConsoleUI after construction. */
    private Consumer<String> navigateToPinout;

    public TuningPane(UIContext uiContext) {
        left = new MainMenuTreeWidget(uiContext);

        CalibrationDialogWidget right = new CalibrationDialogWidget(uiContext);
        JScrollPane rightScrollPane = new JScrollPane(right.getContentPane());

        final AtomicReference<String> currentKey = new AtomicReference<>();

        // Accumulated tune edits across all dialogs for this session.
        final AtomicReference<ConfigurationImage> sessionImage = new AtomicReference<>();

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
                sessionImage.set(pending);
            } else if (sessionImage.get() == null) {
                sessionImage.set(bp.getControllerConfiguration().clone());
            }

            currentKey.set(subMenu.getKey());
            right.update(subMenu.getKey(), uiContext.iniFileState.getIniFileModel(), sessionImage.get());
        });

        // All edit events (dialog fields, table cells, curve drags) flow through onConfigChange.
        // Text fields fire per-keystroke; the toolbar widget coalesces them into one undo point.
        right.setOnConfigChange(image -> {
            toolbar.onEdit(sessionImage.get());
            // Clone because workingImage is mutated in-place by further edits.
            sessionImage.set(image.clone());
            left.refreshExpressions(image);
            uiContext.fireConfigImageChanged(image);
        });

        // Right-click "Show in Pinout" on a pin-enum combo fires the current value upward.
        right.setOnShowInPinout(value -> {
            if (navigateToPinout != null) navigateToPinout.accept(value);
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
                    sessionImage.set(null);
                });
            } else {
                SwingUtilities.invokeLater(() -> {
                    String key = currentKey.get();
                    if (key == null) return;
                    BinaryProtocol bp = uiContext.getBinaryProtocol();
                    if (bp == null || bp.getControllerConfiguration() == null) return;
                    sessionImage.set(bp.getControllerConfiguration().clone());
                    right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
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

    public void setNavigateToPinout(Consumer<String> navigateToPinout) {
        this.navigateToPinout = navigateToPinout;
    }

    /**
     * Selects the dialog containing {@code fieldKey} in the left tree, which triggers the
     * existing onSelect pipeline to render the dialog on the right.
     * Field-level scrolling within the dialog is deferred (see issue — "scroll into view").
     */
    public void navigateToField(String dialogKey, String fieldKey) {
        if (dialogKey == null) return;
        left.selectSubMenu(dialogKey);
    }
}
