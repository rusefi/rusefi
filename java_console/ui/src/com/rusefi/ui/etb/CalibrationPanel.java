package com.rusefi.ui.etb;

import com.rusefi.io.CommandQueue;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.event.ActionEvent;

/**
 * (c) Andrey Belomutskiy
 */
public class CalibrationPanel {
    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public CalibrationPanel() {
        content.setBorder(BorderFactory.createTitledBorder("Calibration"));
        content.add(createCommandButton("Grab TPS#1 fully closed", "calibrate_tps_1_closed"));
        content.add(createCommandButton("Grab TPS#1 Wide Open", "calibrate_tps_1_wot"));

        content.add(createCommandButton("Grab Pedal Up", "calibrate_pedal_up"));
        content.add(createCommandButton("Grab Pedal Down", "calibrate_pedal_down"));
    }

    @NotNull
    private JButton createCommandButton(String caption, String command) {
        JButton button = new JButton(caption);
        button.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                CommandQueue.getInstance().write(command);
            }
        });
        return button;
    }

    public JPanel getContent() {
        return content;
    }
}
