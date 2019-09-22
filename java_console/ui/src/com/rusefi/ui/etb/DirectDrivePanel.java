package com.rusefi.ui.etb;

import com.rusefi.config.generated.Fields;
import com.rusefi.io.CommandQueue;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

import static com.rusefi.config.generated.Fields.CMD_ETB_DUTY;

/**
 * Little panel to drive ETB duty cycle directly
 *
 * (c) Andrey Belomutskiy
 * @see Fields#CMD_ETB_DUTY
 */
public class DirectDrivePanel {
    public static final String CANCEL_DIRECT_DRIVE_COMMAND = CMD_ETB_DUTY + " NaN";
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel currentOverride = new JLabel("NaN");
    private final JTextArea increment = new JTextArea("0.5");
    private final JButton reset = new JButton("Cancel Direct Drive");
    private double directDriverValue;

    public DirectDrivePanel() {
        content.setBorder(BorderFactory.createTitledBorder("Direct Drive"));

        CommandQueue.getInstance().addListener(command -> {
            if (command.startsWith(CMD_ETB_DUTY + " ")) {
                command = command.substring((CMD_ETB_DUTY + " ").length());
                directDriverValue = parseDouble(command, Double.NaN);
                SwingUtilities.invokeLater(() -> currentOverride.setText("PWM override " + directDriverValue));
                reset.setEnabled(!Double.isNaN(directDriverValue));
            }
        });

        JPanel upDownPanel = new JPanel(new GridLayout(2, 1));

        JButton more = new JButton("More");
        more.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                double newValue = getCurrent() + getIncrement();
                CommandQueue.getInstance().write(CMD_ETB_DUTY + " " + newValue);
            }
        });
        upDownPanel.add(more);
        JButton less = new JButton("Less");
        upDownPanel.add(less);
        less.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                double newValue = getCurrent() - getIncrement();
                CommandQueue.getInstance().write(CMD_ETB_DUTY + " " + newValue);
            }
        });

        JPanel leftPanel = new JPanel(new VerticalFlowLayout());
        reset.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                CommandQueue.getInstance().write(CANCEL_DIRECT_DRIVE_COMMAND);
            }
        });

        leftPanel.add(reset);
        leftPanel.add(currentOverride);
        leftPanel.add(new JLabel("Increment:"));
        leftPanel.add(increment);

        content.add(leftPanel, BorderLayout.CENTER);
        content.add(upDownPanel, BorderLayout.EAST);
    }

    private double parseDouble(String text, double defaultValue) {
        try {
            return Double.parseDouble(text);
        } catch (NumberFormatException e) {
            return defaultValue;
        }
    }

    private double getIncrement() {
        try {
            double value = Double.parseDouble(increment.getText());
            return Double.isNaN(value) ? 0.5 : value;
        } catch (NumberFormatException e) {
            increment.setText("0.5");
            return 0.5;
        }
    }

    private double getCurrent() {
        return Double.isNaN(directDriverValue) ? 0 : directDriverValue;
    }

    public JPanel getContent() {
        return content;
    }
}
