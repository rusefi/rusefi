package com.rusefi;

import com.rusefi.io.CommandQueue;
import com.rusefi.ui.MessagesView;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.EtbTestSequence;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class BenchTestPane {
    private final JPanel content = new JPanel(new GridLayout(2, 4));

    public BenchTestPane() {
        content.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        content.add(createFanTest());
        content.add(createFuelPumpTest());
        content.add(createSparkTest());
        content.add(createInjectorTest());
        content.add(createMILTest());
        content.add(createIdleTest());
        content.add(createDizzyTest());
        content.add(UiUtils.wrap(new EtbTestSequence().getButton()));
        content.add(new MessagesView().messagesScroll);
    }

    private Component createMILTest() {
        BenchTestPanel panel = new BenchTestPanel("MIL", "check_engine.jpg") {
            @NotNull
            protected String getCommand() {
                return "milbench";
            }
        };
        return panel.getContent();
    }

    private Component createIdleTest() {
        BenchTestPanel panel = new BenchTestPanel("Idle Valve", "idle_valve.png") {
            @NotNull
            protected String getCommand() {
                return "idlebench";
            }
        };
        return panel.getContent();
    }

    private Component createDizzyTest() {
        BenchTestPanel panel = new BenchTestPanel("Dizzy", "dizzy.jpg") {
            @NotNull
            protected String getCommand() {
                return "dizzybench";
            }
        };
        return panel.getContent();
    }

    private Component createFanTest() {
        BenchTestPanel panel = new BenchTestPanel("Radiator Fan", "radiator_fan.jpg") {
            @NotNull
            protected String getCommand() {
                return "fanbench";
            }
        };
        return panel.getContent();
    }

    private Component createFuelPumpTest() {
        BenchTestPanel panel = new BenchTestPanel("Fuel Pump", "fuel_pump.jpg") {
            @NotNull
            protected String getCommand() {
                return "fuelpumpbench";
            }
        };
        return panel.getContent();
    }

    private Component createSparkTest() {
        final JComboBox<Integer> indexes = createIndexCombo();
        BenchTestPanel panel = new BenchTestPanel("Spark #", "spark.jpg", indexes) {
            @Override
            protected String getCommand() {
                return "sparkbench2 1000 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    private Component createInjectorTest() {
        final JComboBox<Integer> indexes = createIndexCombo();
        BenchTestPanel panel = new BenchTestPanel("Injector #", "injector.png", indexes) {
            @Override
            protected String getCommand() {
                return "fuelbench2 1000 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    @NotNull
    private JComboBox<Integer> createIndexCombo() {
        JComboBox<Integer> indexes = new JComboBox<>();
        for (int i = 1; i <= 12; i++) {
            indexes.addItem(i);
        }
        return indexes;
    }

    public JPanel getContent() {
        return content;
    }

    private abstract static class BenchTestPanel {
        final JPanel panel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 0));
        final JButton test = new JButton("Test");

        public BenchTestPanel(String text, String iconFileName, JComponent... components) {
            ImageIcon icon = UiUtils.loadIcon(iconFileName);
            JPanel rightVerticalPanel = new JPanel(new VerticalFlowLayout());
            rightVerticalPanel.add(new JLabel(text));
            for (JComponent component : components)
                rightVerticalPanel.add(component);
            rightVerticalPanel.add(test);

            panel.add(new JLabel(icon));
            panel.add(rightVerticalPanel);

            int GAP = 3;

            panel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.black), BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP)));

            test.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    CommandQueue.getInstance().write(getCommand());
                }
            });
        }

        protected abstract String getCommand();

        public Component getContent() {
            return UiUtils.wrap(panel);
        }
    }
}