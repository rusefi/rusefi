package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.ui.MessagesView;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static com.rusefi.CommandControl.TEST;

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
        content.add(createDizzyTest());
        content.add(new CommandControl(uiContext, "Reboot", "", "Reboot") {
            @Override
            protected String getCommand() {
                return Fields.CMD_REBOOT;
            }
        }.getContent());
        content.add(new CommandControl(uiContext,"Reboot to DFU", "", "Reboot to DFU") {
            @Override
            protected String getCommand() {
                return Fields.CMD_REBOOT_DFU;
            }
        }.getContent());
        content.add(new MessagesView(config.getRoot()).messagesScroll);
    }

    private Component grabPerformanceTrace() {
        JButton button = new JButton("Grab PTrace");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                BinaryProtocol bp = uiContext.getLinkManager().getCurrentStreamState();
                PerformanceTraceHelper.grabPerformanceTrace(bp);
            }
        });
        return UiUtils.wrap(button);
    }

    private Component createMILTest() {
        CommandControl panel = new CommandControl(uiContext,"MIL", "check_engine.jpg", TEST) {
            @NotNull
            protected String getCommand() {
                return Fields.CMD_MIL_BENCH;
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

    private Component createDizzyTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "Dizzy", "dizzy.jpg", TEST, "dizzybench");
        return panel.getContent();
    }

    private Component createStarterTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "Starter", "", TEST, Fields.CMD_STARTER_BENCH);
        return panel.getContent();
    }

    private Component createFanTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "Radiator Fan", "radiator_fan.jpg", TEST, "fanbench");
        return panel.getContent();
    }

    private Component createAcRelayTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "A/C Compressor Relay", ".jpg", TEST, "acrelaybench");
        return panel.getContent();
    }

    private Component createFuelPumpTest() {
        CommandControl panel = new FixedCommandControl(uiContext, "Fuel Pump", "fuel_pump.jpg", TEST, "fuelpumpbench");
        return panel.getContent();
    }

    private Component createSparkTest() {
        final JComboBox<Integer> indexes = createIndexCombo(Fields.MAX_CYLINDER_COUNT);
        CommandControl panel = new CommandControl(uiContext,"Spark #", "spark.jpg", TEST, indexes) {
            @Override
            protected String getCommand() {
                return "sparkbench2 1000 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    private Component createInjectorTest() {
        final JComboBox<Integer> indexes = createIndexCombo(Fields.MAX_CYLINDER_COUNT);
        CommandControl panel = new CommandControl(uiContext,"Injector #", "injector.png", TEST, indexes) {
            @Override
            protected String getCommand() {
                return "fuelbench2 1000 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    private Component createSolenoidTest() {
        final JComboBox<Integer> indexes = createIndexCombo(Fields.TCU_SOLENOID_COUNT);
        CommandControl panel = new CommandControl(uiContext,"TCU Solenoid #", "solenoid.jpg", TEST, indexes) {
            @Override
            protected String getCommand() {
                return "tcusolbench 1000 " + indexes.getSelectedItem() + " 1000 1000 3";
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
