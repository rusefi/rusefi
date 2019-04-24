package com.rusefi.ui.etb;

import com.rusefi.ui.util.UiUtils;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

public class CommandsPanel {
    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public CommandsPanel() {


        JPanel testParameters = new JPanel(new VerticalFlowLayout());
        testParameters.setBorder(BorderFactory.createTitledBorder("Try PID settings"));
        EtbTestSequence etbTestSequence = new EtbTestSequence();
        testParameters.add(UiUtils.wrap(etbTestSequence.getButton()));
        testParameters.add(etbTestSequence.getResult());
        testParameters.add(new JLabel("To change setting use following commands:"));
        testParameters.add(new JLabel("set etb_p X"));
        testParameters.add(new JLabel("set etb_i X"));
        testParameters.add(new JLabel("set etb_d X"));
        testParameters.add(new JLabel("set etb_o X"));
        testParameters.add(new JLabel("For example:"));
        testParameters.add(new JLabel("set etb_p 1.1"));



        content.setBorder(BorderFactory.createTitledBorder("Commands"));

        content.add(testParameters);
//        content.add(UiUtils.wrap(new EtbMonteCarloSequence().getButton()));
//        content.add(UiUtils.wrap(new MagicSpotsFinder().getButton()));
    }

    public JPanel getContent() {
        return content;
    }
}
