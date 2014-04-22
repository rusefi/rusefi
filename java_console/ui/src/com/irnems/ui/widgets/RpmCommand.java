package com.irnems.ui.widgets;

import com.rusefi.io.CommandQueue;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;

/**
 * Date: 3/17/13
 * (c) Andrey Belomutskiy
 */
public class RpmCommand extends JPanel {
    public RpmCommand() {
        setBorder(BorderFactory.createLineBorder(Color.ORANGE));
        setLayout(new FlowLayout(FlowLayout.LEFT));
        add(new JLabel("set RPM: "));

        final JSpinner spinner = new JSpinner() {
            @Override
            public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                return new Dimension(100, size.height);
            }
        };
        SpinnerNumberModel m = (SpinnerNumberModel) spinner.getModel();
        m.setStepSize(100);
        spinner.setValue(600);

        spinner.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                requestRpmChange((Integer) spinner.getValue());
            }
        });
        add(spinner);
    }

    public static void requestRpmChange(int rpm) {
        CommandQueue.getInstance().write("rpm " + rpm);
    }
}
