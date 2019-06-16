package com.rusefi.ui.widgets;

import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.EcuStimulator;
import com.rusefi.io.CommandQueue;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;

/**
 * Date: 3/17/13
 * (c) Andrey Belomutskiy
 */
public class PotCommand {
    public static final double VOLTAGE_CORRECTION = 2.9 / 3;
    public final JPanel panel;
    final JSpinner potSpinner;

    public PotCommand(final int channel) {
        final JLabel rValue = new JLabel();

        final JSpinner voltageSpinner = new JSpinner(new SpinnerNumberModel(0.0, 0, 5, 0.1)) {
            @Override
            public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                return new Dimension(100, size.height);
            }
        };
        ((SpinnerNumberModel) voltageSpinner.getModel()).setStepSize(0.1);
        voltageSpinner.setValue(1.0);
/*
        voltageSpinner.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                Double Vout = (Double) voltageSpinner.getValue();
                int d = getPotResistance(Vout, SensorCentral.getInstance().getValue(Sensor.VREF) * VOLTAGE_CORRECTION);
                potSpinner.setValue(d);
            }
        });
*/

        potSpinner = new JSpinner() {
            @Override
            public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                return new Dimension(100, size.height);
            }
        };
        potSpinner.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                Integer value = (Integer) potSpinner.getValue();
                try {
                    requestPotChange(channel, value);
                } catch (IllegalArgumentException ignore) {
                    return;
                }
                int r = getRbyD(value);
                rValue.setText("R=" + r);
            }
        });
        potSpinner.setValue(10);


        JPanel upper = new JPanel(new FlowLayout(FlowLayout.LEFT));
        upper.add(new JLabel("set pot" + channel + ": "));
        upper.add(potSpinner);
        upper.add(rValue);

        JPanel center = new JPanel(new FlowLayout(FlowLayout.LEFT));
        center.add(new JLabel("volts: "));
        center.add(voltageSpinner);

        panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createLineBorder(Color.CYAN));
        panel.add(upper, BorderLayout.NORTH);
        panel.add(center, BorderLayout.CENTER);
    }

    public static void requestPotChange(int channel, int resistance) {
        if (resistance < 0 || resistance > 10000)
            throw new IllegalArgumentException("resistance: " + resistance);
        CommandQueue.getInstance().write("pot " + channel + " " + resistance);
    }

    public static int getPotResistance(double vout, double vRef) {
        double r = getR1InVoltageDivider3(vout, vRef, EcuStimulator.getInstance().getInputs().getEngineLoadR2Resistance());
        MessagesCentral.getInstance().postMessage(PotCommand.class, "VRef=" + vRef + ", needed resistance: " + r);
        // pot command accept resistance and does the conversion itself
        return (int) r;
    }

    private static int getRbyD(Integer value) {
        return (int) (10000.0 * (256 - value) / 256) + 52;
    }

//    private static int getDbyR(double Rwa) {
//        return (int) (256 - (Rwa - 52) * 256 / 10000);
//    }

    public static double getR1InVoltageDivider3(double Vout, double Vin, double r2) {
        return r2 * Vout / Vin;
    }

}
