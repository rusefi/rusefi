package com.rusefi.ui.widgets;

import com.irnems.core.Sensor;
import com.irnems.core.SensorCentral;
import com.rusefi.io.CommandQueue;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * This panel turns ON/OFF some rusefi configuration property
 * <p/>
 * 7/11/13
 * (c) Andrey Belomutskiy
 */
public class BooleanFlagControlPanel {
    private final JPanel content = new JPanel(new MigLayout());
    protected final JCheckBox checkBox;

    public BooleanFlagControlPanel(String labelCaption, String checkboxCaption) {
        content.add(new JLabel(labelCaption));
        checkBox = new JCheckBox(checkboxCaption);
        content.add(checkBox);
    }

    public JComponent getControl() {
        return content;
    }

    protected void installCommand(final String command) {
        checkBox.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int mode = checkBox.isSelected() ? 1 : 0;
                CommandQueue.getInstance().write(command + mode);
            }
        });
    }

    protected void installStatusReader(final Sensor statusSensor) {
        SensorCentral.getInstance().addListener(statusSensor, new SensorCentral.AdcListener() {
            @Override
            public void onAdcUpdate(SensorCentral model, double value) {
                checkBox.setSelected(value > 0);
            }
        });
    }
}
