package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;

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

        JPanel toolbar = getToolbar(uiContext, right, currentKey, sessionImage);

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, left.getContentPane(), rightScrollPane);
        splitPane.setResizeWeight(0.3);

        left.setOnSelect(subMenu -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null || bp.getControllerConfiguration() == null) {
                return;
            }

            // On first navigation, seed the session image from the live ECU state.
            // On subsequent navigations, carry forward whatever the user has edited so far
            // so that changes made in dialogs, tables, or curves are not lost when opening another.
            ConfigurationImage pending = right.getWorkingImage();
            if (pending != null) {
                sessionImage[0] = pending;
            } else if (sessionImage[0] == null) {
                sessionImage[0] = bp.getControllerConfiguration();
            }

            currentKey[0] = subMenu.getKey();
            right.update(subMenu.getKey(), uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
        });

        right.setOnConfigChange(left::refreshExpressions);

        content.add(toolbar, BorderLayout.NORTH);
        content.add(splitPane, BorderLayout.CENTER);
    }

    private static @NotNull JPanel getToolbar(UIContext uiContext, CalibrationDialogWidget right,
                                              String[] currentKey, ConfigurationImage[] sessionImage) {
        JButton burnButton = new JButton("Burn to ECU");
        burnButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            // Use the working image
            // fall back to the session image if no content has been opened yet.
            ConfigurationImage toBurn = right.getWorkingImage();
            if (toBurn == null) toBurn = sessionImage[0];
            if (bp == null || toBurn == null) return;
            final ConfigurationImage image = toBurn;
            sessionImage[0] = image;
            uiContext.getLinkManager().submit(() -> bp.uploadChanges(image));
        });

        JButton discardButton = new JButton("Discard Changes");
        discardButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null) return;
            // Reset session to the snapshot captured when we connected to this ECU.
            ConfigurationImage baseline = bp.getCachedImage();
            if (baseline == null) baseline = bp.getControllerConfiguration();
            sessionImage[0] = baseline;
            if (currentKey[0] != null) {
                right.update(currentKey[0], uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
            }
        });

        JPanel toolbar = new JPanel(new FlowLayout(FlowLayout.LEFT));
        toolbar.add(burnButton);
        toolbar.add(discardButton);
        return toolbar;
    }

    public JPanel getContent() {
        return content;
    }
}
