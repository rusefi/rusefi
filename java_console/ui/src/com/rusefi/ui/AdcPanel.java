package com.rusefi.ui;

import com.irnems.core.SensorCentral;
import com.irnems.core.Pair;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;

import static com.irnems.core.EngineState.SNIFFED_ADC_COUNT;

/**
 * Date: 1/7/13
 * (c) Andrey Belomutskiy
 */
public class AdcPanel {
    protected final Map<Integer, JLabel> sniffedAdcLabels = new HashMap<Integer, JLabel>();
    public final Map<Integer, JLabel> inputLabels = new HashMap<Integer, JLabel>();
    public final Map<Integer, JLabel> internalAdcLabels = new HashMap<Integer, JLabel>();

    public AdcPanel(BooleanInputsModel inputs) {
    }

    public JComponent createAdcPanel() {
        JComponent container = new JPanel(new MigLayout());
        JButton saveAdc = new JButton("save ADC");
        saveAdc.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
//                model.saveToFile();
            }
        });
        container.add(saveAdc, "grow, wrap");

        for (int channel1 = 0; channel1 < SNIFFED_ADC_COUNT / 2; channel1++) {
//            {
//                Pair<JPanel, JLabel> pair1 = createAdcLabel(channel1);
//                // 2nd column of channels
//                int channel2 = channel1 + SNIFFED_ADC_COUNT / 2;
//                Pair<JPanel, JLabel> pair2 = createAdcLabel(channel2);
//                sniffedAdcLabels.put(channel1, pair1.second);
//                sniffedAdcLabels.put(channel2, pair2.second);
//
//                container.add(pair1.first);
//                container.add(pair2.first);
//            }

            {
                Pair<JPanel, JLabel> inputPair = createLabelWithCaption(BooleanInputsModel.getTitle(channel1));
                inputLabels.put(channel1, inputPair.second);
                container.add(inputPair.first);
            }
            {
                Pair<JPanel, JLabel> pair = createLabelWithCaption("adc " + channel1);
                internalAdcLabels.put(channel1, pair.second);
                container.add(pair.first, "grow, wrap");
            }

        }
        return container;
    }

//    private Pair<JPanel, JLabel> createAdcLabel(final int channel) {
//        final Pair<JPanel, JLabel> result = createLabelWithCaption(SensorCentral.getTitle(channel));
//        model.addListener(new SensorCentral.SensorListener() {
//            public void onAdcUpdate(SensorCentral model, Sensor sensor, double value) {
//                if (sensor.getMazdaIndex() != channel)
//                    return;
//                JLabel label = result.second;
//                String representation = model.getSniffedAdcRepresentation(sensor);
//                label.setText(representation);
//            }
//        });
//        return result;
//    }

    private Pair<JPanel, JLabel> createLabelWithCaption(String title) {
        JPanel panel = new JPanel(new FlowLayout());
        panel.add(new JLabel(title));
        JLabel value = new JLabel("");
        panel.add(value);
        return new Pair<JPanel, JLabel>(panel, value);
    }

}
