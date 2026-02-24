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

        JPanel toolbar = getToolbar(uiContext, right, currentKey);

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, left.getContentPane(), rightScrollPane);
        splitPane.setResizeWeight(0.3);

        left.setOnSelect(subMenu -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null || bp.getControllerConfiguration() == null) {
                return;
            }
            currentKey[0] = subMenu.getKey();
            right.update(subMenu.getKey());
        });

        right.setOnConfigChange(left::refreshExpressions);

        content.add(toolbar, BorderLayout.NORTH);
        content.add(splitPane, BorderLayout.CENTER);
    }

    private static @NotNull JPanel getToolbar(UIContext uiContext, CalibrationDialogWidget right, String[] currentKey) {
        JButton burnButton = new JButton("Burn to ECU");
        burnButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            ConfigurationImage workingImage = right.getWorkingImage();
            if (bp == null || workingImage == null) return;
            uiContext.getLinkManager().submit(() -> bp.uploadChanges(workingImage));
        });

        JButton discardButton = new JButton("Discard Changes");
        discardButton.addActionListener(e -> {
            if (currentKey[0] != null) {
                right.update(currentKey[0]);
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
