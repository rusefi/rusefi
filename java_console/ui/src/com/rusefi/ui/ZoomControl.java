package com.rusefi.ui;

import com.irnems.waves.ZoomProvider;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * 7/7/13
 * (c) Andrey Belomutskiy
 */
public class ZoomControl extends JPanel implements ZoomProvider {
    private final JLabel currentValue = new JLabel();
    private double value;
    public ZoomControlListener listener = null;

    public ZoomControl() {
        super(new FlowLayout());
        setValue(1);

//        final JTextField text = new JTextField() {
//            @Override
//            public Dimension getPreferredSize() {
//                Dimension size = super.getPreferredSize();
//                return new Dimension(200, size.height);
//            }
//        };

        add(currentValue);

        JButton plus = new JButton("+");
        plus.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setValue(value * 1.1);
            }
        });
        add(plus);

        JButton minus = new JButton("-");
        minus.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setValue(value / 1.1);
            }
        });
        add(minus);
    }

    private void setValue(double value) {
        this.value = value;
        currentValue.setText(String.format(" %.4fms", value));
        if (listener != null)
            listener.onZoomChange();
    }

    @Override
    public double getZoomValue() {
        return value;
    }

    interface ZoomControlListener {
        void onZoomChange();
    }
}
