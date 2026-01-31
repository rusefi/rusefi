package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.ui.MessagesView;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

import static com.rusefi.CommandControl.TEST;
import static com.rusefi.config.generated.Integration.CMD_STARTER_BENCH;
import static com.rusefi.config.generated.Integration.*;

public class BenchTestPane {
    private final JPanel content = new JPanel(new GridLayout(2, 5));
    private final UIContext uiContext;

    public BenchTestPane(UIContext uiContext, PersistentConfiguration config) {
        this.uiContext = uiContext;
        content.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        content.add(grabPerformanceTrace());
        content.add(createFanTest());
        content.add(createAcRelayTest());
        content.add(createFuelPumpTest());
        content.add(createSparkTest());
        content.add(createInjectorTest());
        content.add(createSolenoidTest());
        content.add(createMILTest());
        content.add(createIdleTest());
        content.add(createStarterTest());
        content.add(new CommandControl(uiContext, "Reboot", "", "Reboot") {
            @Override
            protected String getCommand() {
                return Integration.CMD_REBOOT;
            }
        }.getContent());
        content.add(new CommandControl(uiContext,"Reboot to DFU", "", "Reboot to DFU") {
            @Override
            protected String getCommand() {
                return Integration.CMD_REBOOT_DFU;
            }
        }.getContent());
        content.add(new MessagesView(config.getRoot()).messagesScroll);
    }

    private Component grabPerformanceTrace() {
        JButton button = new JButton("Grab PTrace");
        ActionListener actionListener = e -> uiContext.getLinkManager().COMMUNICATION_EXECUTOR.execute(() -> {
            BinaryProtocol bp = uiContext.getLinkManager().getBinaryProtocol();
            PerformanceTraceHelper.grabPerformanceTrace(button, bp);
        });
        button.addActionListener(actionListener);
        return UiUtils.wrap(button);
    }

    private Component createMILTest() {
        CommandControl panel = new CommandControl(uiContext,"MIL", "check_engine.jpg", TEST) {
            @NotNull
            protected String getCommand() {
                return Integration.CMD_MIL_BENCH;
            }
        };
        return panel.getContent();
    }

    private Component createIdleTest() {
        CommandControl panel = new CommandControl(uiContext,"Idle Valve", "idle_valve.png", TEST) {
            @NotNull
            protected String getCommand() {
                return "idlebench";
            }
        };
        return panel.getContent();
    }

    private Component createStarterTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "Starter", "", TEST, CMD_STARTER_BENCH);
        return panel.getContent();
    }

    private Component createFanTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "Radiator Fan", "radiator_fan.jpg", TEST, CMD_FAN_BENCH);
        return panel.getContent();
    }

    private Component createAcRelayTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "A/C Compressor Relay", ".jpg", TEST, CMD_AC_RELAY_BENCH);
        return panel.getContent();
    }

    private Component createFuelPumpTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "Fuel Pump", "fuel_pump.jpg", TEST, CMD_FUEL_PUMP_BENCH);
        return panel.getContent();
    }

    private Component createSparkTest() {
        final JComboBox<Integer> indexes = createIndexCombo(VariableRegistryValues.MAX_CYLINDER_COUNT);
        CommandControl panel = new CommandControl(uiContext,"Spark #", "spark.jpg", TEST, indexes) {
            @Override
            protected String getCommand() {
                return "sparkbench2 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    private Component createInjectorTest() {
        final JComboBox<Integer> indexes = createIndexCombo(VariableRegistryValues.MAX_CYLINDER_COUNT);
        CommandControl panel = new CommandControl(uiContext,"Injector #", "injector.png", TEST, indexes) {
            @Override
            protected String getCommand() {
                return "fuelbench2 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    private Component createSolenoidTest() {
        final JComboBox<Integer> indexes = createIndexCombo(VariableRegistryValues.TCU_SOLENOID_COUNT);
        CommandControl panel = new CommandControl(uiContext,"TCU Solenoid #", "solenoid.jpg", TEST, indexes) {
            @Override
            protected String getCommand() {
                return "tcusolbench " + indexes.getSelectedItem() + " 1000 1000 3";
            }
        };
        return panel.getContent();
    }

    @NotNull
    private JComboBox<Integer> createIndexCombo(Integer count) {
        JComboBox<Integer> indexes = new JComboBox<>();
        for (int i = 1; i <= count; i++) {
            indexes.addItem(i);
        }
        return indexes;
    }

    public JPanel getContent() {
        return content;
    }

}
