package com.rusefi;

import com.irnems.core.MessagesCentral;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class AverageAnglePanel {
    final JPanel panel = new JPanel(new BorderLayout());

    AverageAngles aa = new AverageAngles();

    public AverageAnglePanel() {
        JButton reset = new JButton("reset");
        reset.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                aa.clear();
            }
        });

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {

            }
        });

    }
}