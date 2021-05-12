package com.rusefi.ui.etb;

import com.rusefi.config.generated.Fields;
import com.rusefi.io.CommandQueue;
import com.rusefi.ui.UIContext;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.event.ActionEvent;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class CalibrationPanel {
    private final JPanel content = new JPanel(new VerticalFlowLayout());
    private final UIContext uiContext;

    public CalibrationPanel(UIContext uiContext) {
        this.uiContext = uiContext;
        content.setBorder(BorderFactory.createTitledBorder("Calibration"));
        content.add(createCommandButton("Grab TPS#1 fully closed", "calibrate_tps_1_closed"));
        content.add(createCommandButton("Grab TPS#1 Wide Open", "calibrate_tps_1_wot"));

        content.add(createCommandButton("Grab Pedal Up", Fields.CMD_CALIBRATE_PEDAL_UP));
        content.add(createCommandButton("Grab Pedal Down", Fields.CMD_CALIBRATE_PEDAL_DOWN));
    }

    @NotNull
    private JButton createCommandButton(String caption, String command) {
        JButton button = new JButton(caption);
        button.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                uiContext.getCommandQueue().write(command);
            }
        });
        return button;
    }

    public JPanel getContent() {
        return content;
    }
}
