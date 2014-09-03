package com.rusefi;

import com.irnems.Launcher;

import javax.swing.*;
import java.awt.*;

/**
 * 8/13/14
 * (c) Andrey Belomutskiy
 */
public class StimulationInputs {
    private final JPanel content = new JPanel(new GridLayout(7, 1));

    private final JSpinner elResistance2 = new JSpinner(new SpinnerNumberModel(10000, 0, 100000, 1));
    private final ValueRangeControl elRange = new ValueRangeControl("engine load", 1, 0.1, 4.6);

    private final ValueRangeControl rpmRange = new ValueRangeControl("RPM", 400, 100, 6000);
    private final ValueRangeControl cltRange = new ValueRangeControl("CLR r", 100, 100, 100);
    private final ValueRangeControl iatRange = new ValueRangeControl("IAT r", 100, 100, 9900);
    private final ValueRangeControl tpsRange = new ValueRangeControl("TPS", 1, 0.1, 4.5);

    public StimulationInputs(EcuStimulator ecuStimulator) {

        JPanel channelPanel = new JPanel(new FlowLayout());
        channelPanel.add(new JLabel("Input channel"));
        content.add(channelPanel);


        content.add(rpmRange.getContent());
        content.add(elRange.getContent());
        content.add(cltRange.getContent());
        content.add(iatRange.getContent());
        content.add(tpsRange.getContent());

        content.add(new JLabel("EL resistance"));
        content.add(wrap(elResistance2));

        JButton button = ecuStimulator.createButton();
        if (Launcher.SHOW_STIMULATOR)
            content.add(wrap(button));
    }

    public double getEngineLoadMin() {
        return elRange.getFrom();
    }

    public double getEngineLoadMax() {
        return elRange.getTo();
    }

    public int getRpmFrom() {
        return (int) rpmRange.getFrom();
    }

    public int getRpmTo() {
        return (int) rpmRange.getTo();
    }

    public int getCltFrom() {
        return (int) cltRange.getFrom();
    }

    public int getCltTo() {
        return (int) cltRange.getTo();
    }

    public static JComponent wrap(JComponent component) {
        JPanel result = new JPanel();
        result.add(component);
        return result;
    }

    public JPanel getContent() {
        return content;
    }

    public double getEngineLoadR2Resistance() {
        return (Integer) elResistance2.getValue();
    }
}

