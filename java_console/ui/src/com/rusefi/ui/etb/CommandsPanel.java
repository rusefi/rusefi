package com.rusefi.ui.etb;

import com.rusefi.ETBPane;
import com.rusefi.io.CommandQueue;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

public class CommandsPanel {
    private final JPanel content = new JPanel(new VerticalFlowLayout());
    private final JLabel currentOverride = new JLabel();

    public CommandsPanel() {
        content.add(currentOverride);
        CommandQueue.getInstance().addListener(command -> {
            if (command.startsWith(ETBPane.SET_ETB)) {
                command = command.substring(ETBPane.SET_ETB.length());
                String finalCommand = command;
                SwingUtilities.invokeLater(() -> currentOverride.setText("PWM override " + finalCommand));
            }
        });

        JPanel spotsPane = new JPanel(new VerticalFlowLayout());
        spotsPane.setBorder(BorderFactory.createTitledBorder("Magic Spots"));
        MagicSpotsFinder magicSpotsFinder = new MagicSpotsFinder();
        spotsPane.add(UiUtils.wrap(magicSpotsFinder.getButton()));
        spotsPane.add(magicSpotsFinder.getPoints());

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
        content.add(spotsPane);
//        content.add(UiUtils.wrap(new EtbMonteCarloSequence().getButton()));

        content.add(AnyCommand.createArea(new Node(), ETBPane.SET_ETB + "10", false, false).getContent());
    }

    public JPanel getContent() {
        return content;
    }
}
