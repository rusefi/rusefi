package com.rusefi;

import com.rusefi.io.CommandQueue;
import com.rusefi.ui.MessagesView;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class BenchTestPane {
    private final JPanel content = new JPanel(new GridLayout(2, 3));

    public BenchTestPane() {
        content.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        content.add(createFanTest());
        content.add(createFuelPumpTest());
        content.add(createSparkTest());
        content.add(createInjectorTest());
        content.add(new JLabel());
        content.add(new MessagesView().messagesScroll);
    }

    private Component createFanTest() {
        BenchTestPanel panel = new BenchTestPanel("radiator_fan.jpg", "Radiator Fan") {
            @NotNull
            protected String getCommand() {
                return "fanbench";
            }
        };
        return panel.getContent();
    }

    private Component createFuelPumpTest() {
        BenchTestPanel panel = new BenchTestPanel("fuel_pump.jpg", "Fuel Pump") {
            @NotNull
            protected String getCommand() {
                return "fuelpumpbench";
            }
        };
        return panel.getContent();
    }

    private Component createSparkTest() {
        final JComboBox<Integer> indexes = createIndexCombo();
        JLabel label = new JLabel("Spark #", UiUtils.loadIcon("spark.jpg"), SwingConstants.LEFT);
        BenchTestPanel panel = new BenchTestPanel(label, indexes) {
            @Override
            protected String getCommand() {
                return "sparkbench2 1000 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    private Component createInjectorTest() {
        final JComboBox<Integer> indexes = createIndexCombo();
        JLabel label = new JLabel("Injector #", UiUtils.loadIcon("injector.png"), SwingConstants.LEFT);
        BenchTestPanel panel = new BenchTestPanel(label, indexes) {
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

        public BenchTestPanel(String imageFileName, String label) {
            this(new JLabel(label, UiUtils.loadIcon(imageFileName), SwingConstants.LEFT));
        }

        public BenchTestPanel(String label) {
            this(new JLabel(label));
        }

        public BenchTestPanel(JComponent... components) {
            for (JComponent component : components)
                panel.add(component);
            panel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.black), BorderFactory.createEmptyBorder(5, 5, 5, 5)));

            panel.add(test);
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