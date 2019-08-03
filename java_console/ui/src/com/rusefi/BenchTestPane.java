package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.ui.MessagesView;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.CommandControl.TEST;

public class BenchTestPane {
    private final JPanel content = new JPanel(new GridLayout(2, 5));

    public BenchTestPane() {
        content.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        content.add(createFanTest());
        content.add(createFuelPumpTest());
        content.add(createSparkTest());
        content.add(createInjectorTest());
        content.add(createMILTest());
        content.add(createIdleTest());
        content.add(createDizzyTest());
        content.add(new CommandControl("Reboot", "", "Reboot") {
            @Override
            protected String getCommand() {
                return Fields.CMD_REBOOT;
            }
        }.getContent());
        content.add(new CommandControl("Reboot to DFU", "", "Reboot to DFU") {
            @Override
            protected String getCommand() {
                return Fields.CMD_REBOOT_DFU;
            }
        }.getContent());
        content.add(new MessagesView().messagesScroll);
    }

    private Component createMILTest() {
        CommandControl panel = new CommandControl("MIL", "check_engine.jpg", TEST) {
            @NotNull
            protected String getCommand() {
                return "milbench";
            }
        };
        return panel.getContent();
    }

    private Component createIdleTest() {
        CommandControl panel = new CommandControl("Idle Valve", "idle_valve.png", TEST) {
            @NotNull
            protected String getCommand() {
                return "idlebench";
            }
        };
        return panel.getContent();
    }

    private Component createDizzyTest() {
        CommandControl panel = new FixedCommandControl("Dizzy", "dizzy.jpg", TEST, "dizzybench");
        return panel.getContent();
    }

    private Component createFanTest() {
        CommandControl panel = new FixedCommandControl("Radiator Fan", "radiator_fan.jpg", TEST, "fanbench");
        return panel.getContent();
    }

    private Component createFuelPumpTest() {
        CommandControl panel = new FixedCommandControl("Fuel Pump", "fuel_pump.jpg", TEST, "fuelpumpbench");
        return panel.getContent();
    }

    private Component createSparkTest() {
        final JComboBox<Integer> indexes = createIndexCombo();
        CommandControl panel = new CommandControl("Spark #", "spark.jpg", TEST, indexes) {
            @Override
            protected String getCommand() {
                return "sparkbench2 1000 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    private Component createInjectorTest() {
        final JComboBox<Integer> indexes = createIndexCombo();
        CommandControl panel = new CommandControl("Injector #", "injector.png", TEST, indexes) {
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

}