package com.rusefi;

import javax.swing.*;
import java.awt.*;

public class ValueRangeControl {
    private final JSpinner minSpinner;
    private final JSpinner maxSpinner;

    private final JPanel panel = new JPanel(new FlowLayout());
    private final double increment;

    public ValueRangeControl(String title, double min, double increment, double max) {
        this.increment = increment;
        minSpinner = new JSpinner(new SpinnerNumberModel(min, min, max, increment));
        maxSpinner = new JSpinner(new SpinnerNumberModel(max, min, max, increment));

        panel.add(new JLabel(title + " from"));
        panel.add(minSpinner);
        panel.add(new JLabel("to"));
        panel.add(maxSpinner);
    }

    public double getFrom() {
        return (double) minSpinner.getModel().getValue();
    }

    public double getTo() {
        return (double) maxSpinner.getModel().getValue();
    }

    public double getStep() {
        return increment;
    }

    public Component getContent() {
        return panel;
    }
}