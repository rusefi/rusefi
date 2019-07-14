package com.rusefi;

import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;

/**
 * 8/13/14
 * (c) Andrey Belomutskiy
 */
public class StimulationInputs {
    static final int DEFAULT_RPM_MIN = 800;
    static final int DEFAULT_RPM_MAX = 7000;
    private final JPanel content = new JPanel(new GridLayout(7, 1));

    private final JSpinner elResistance2 = new JSpinner(new SpinnerNumberModel(10000, 0, 100000, 1));
    private final ValueRangeControl elRange = new ValueRangeControl("engine load", 1.2, 0.213333, 4.4);

    private final ValueRangeControl rpmRange = new ValueRangeControl("RPM", DEFAULT_RPM_MIN, 413.333374, DEFAULT_RPM_MAX);
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
        content.add(UiUtils.wrap(elResistance2));

        JButton button = ecuStimulator.createButton();
        content.add(UiUtils.wrap(button));
    }

    public double getEngineLoadMin() {
        return elRange.getFrom();
    }

    public double getEngineLoadMax() {
        return elRange.getTo();
    }

    public double getRpmFrom() {
        return (int) rpmRange.getFrom();
    }

    public double getRpmTo() {
        return (int) rpmRange.getTo();
    }

    public double getRpmStep() {
        return rpmRange.getStep();
    }

    public int getCltFrom() {
        return (int) cltRange.getFrom();
    }

    public int getCltTo() {
        return (int) cltRange.getTo();
    }

    public JPanel getContent() {
        return content;
    }

    public double getEngineLoadR2Resistance() {
        return (Integer) elResistance2.getValue();
    }

    public double getEngineLoadStep() {
        return elRange.getStep();
    }
}

