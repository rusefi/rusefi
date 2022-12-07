package com.rusefi.ui;

import com.rusefi.AverageAnglePanel;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.OutputChannel;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.InvocationConfirmationListener;
import com.rusefi.trigger.TriggerShapeHolder;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.Date;
import java.util.concurrent.atomic.AtomicReference;

/**
 * 1/17/2015
 */
public class Wizard {
    private final JPanel panel = new JPanel(new BorderLayout());
    private final JPanel content = new JPanel();
    private final JButton button = new JButton("Trigger Wizard");

    interface WizardStep {
        Component getContent();
    }

    abstract static class WizardStepImpl implements WizardStep {
        protected WizardStep nextStep;

        public WizardStepImpl() {
        }

        public WizardStepImpl setNext(WizardStepImpl nextStep) {
            this.nextStep = nextStep;
            return nextStep;
        }
    }

    class SendCommand extends WizardStepImpl {
        private final String command;

        SendCommand(String command) {
            this.command = command;
        }

        @Override
        public Component getContent() {
            CommandQueue instance = null;
            instance.write(command, CommandQueue.DEFAULT_TIMEOUT, new InvocationConfirmationListener() {
                @Override
                public void onCommandConfirmation() {
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            applyStep(nextStep);
                        }
                    });
                }
            });

            return new JLabel("Sending " + command);
        }
    }

    private final WizardStepImpl TRIGGER_WIZARD_HELLO = new WizardStepImpl() {
        @Override
        public Component getContent() {
            JButton button = new JButton("Hello, let's test trigger. Click this to process");
            button.addActionListener(new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    applyStep(nextStep);
                }
            });
            return button;
        }
    };

    {
        TRIGGER_WIZARD_HELLO.setNext(new SendCommand("disable injection"))
                .setNext(new SendCommand("disable ignition"))
                .setNext(new SendCommand("chartsize " + (10 * TriggerShapeHolder.CURRENT.getTotalToothCount() * 2)))
                .setNext(new SendCommand("subscribe " + OutputChannel.WaveChartCurrentSize.getProtocolId()))
                .setNext(new SendCommand("subscribe " + OutputChannel.RunningTriggerError.getProtocolId()))
                .setNext(new SendCommand("subscribe " + OutputChannel.RunningOrderingTriggerError.getProtocolId()))
                .setNext(new SendCommand("set_analog_chart_freq " + 1))
                .setNext(new WaitForZeroRpm())
        ;
    }

    public Component createPane() {
        panel.add(button, BorderLayout.NORTH);

        panel.add(content, BorderLayout.CENTER);

        panel.add(new AverageAnglePanel(null).getPanel(), BorderLayout.SOUTH);

        button.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                button.setEnabled(false);

                applyStep(Wizard.this.TRIGGER_WIZARD_HELLO);
            }
        });
        return panel;
    }

    private void applyStep(WizardStep step) {
        System.out.println(new Date() + " apply " + step);
        Component newContent = getContent(step);
        content.removeAll();
        content.add(newContent);
        UiUtils.trueLayout(content);
    }

    private Component getContent(WizardStep step) {
        Component newContent;
        if (step == null) {
            newContent = new JLabel("Wizard is done!");
            button.setEnabled(true);
        } else {
            newContent = step.getContent();
        }
        return newContent;
    }

    private class WaitForZeroRpm extends WizardStepImpl {
        private int counter;

        @Override
        public Component getContent() {
            double rpm = SensorCentral.getInstance().getValue(Sensor.RPMValue);
            if (rpm == 0) {
                return Wizard.this.getContent(nextStep);
            }

            scheduleRepaint(1000, this);

            return new JLabel("Current RPM: " + rpm + ", please stop the engine. Waiting " + counter++);
        }
    }

    private void scheduleRepaint(int timeoutMs, final WizardStep step) {
        final AtomicReference<Timer> tHolder = new AtomicReference<>();
        Timer t = new Timer(timeoutMs, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                tHolder.get().stop();
                applyStep(step);
            }
        });
        tHolder.set(t);
        t.start();
    }
}
