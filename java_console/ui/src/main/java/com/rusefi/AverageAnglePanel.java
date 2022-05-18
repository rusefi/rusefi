package com.rusefi;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

import static com.rusefi.ui.util.LocalizedMessages.RESET;

public class AverageAnglePanel {
    private final JPanel panel = new JPanel(new BorderLayout());

    private final AverageAngles aa = new AverageAngles();
    private final JTextArea text = new JTextArea();

    public AverageAnglePanel(UIContext uiContext) {
        JButton reset = new JButton(RESET.getMessage());
        reset.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                aa.clear();
                showResults();
            }
        });

        panel.add(reset, BorderLayout.NORTH);
        panel.add(new JScrollPane(text, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension(400, 400);
            }
        }, BorderLayout.CENTER);

        uiContext.sensorSnifferCentral.addListener(new SensorSnifferCentral.AnalogChartListener() {
                                           @Override
                                           public void onAnalogChart(String message) {
                                               int rpm = (int) SensorCentral.getInstance().getValue(Sensor.RPMValue);
                                               aa.add(rpm, message);
                                               showResults();
                                           }
                                       }
        );
    }

    private void showResults() {
        String test = aa.getReport();
        text.setText(test);
    }

    public JPanel getPanel() {
        return panel;
    }
}