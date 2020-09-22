package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.tracing.Entry;
import com.rusefi.tracing.JsonOutput;
import com.rusefi.ui.MessagesView;
import com.rusefi.ui.RpmModel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

import static com.rusefi.CommandControl.TEST;
import static com.rusefi.binaryprotocol.BinaryProtocolCommands.RESPONSE_OK;
import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class BenchTestPane {
    private final JPanel content = new JPanel(new GridLayout(2, 5));
    private final UIContext uiContext;

    public BenchTestPane(UIContext uiContext) {
        this.uiContext = uiContext;
        content.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        content.add(grabPerformanceTrace());
        content.add(createFanTest());
        content.add(createAcRelayTest());
        content.add(createFuelPumpTest());
        content.add(createSparkTest());
        content.add(createInjectorTest());
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
        content.add(new MessagesView().messagesScroll);
    }

    private Component grabPerformanceTrace() {
        JButton button = new JButton("Grab PTrace");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                BinaryProtocol bp = uiContext.getLinkManager().getCurrentStreamState();
                bp.executeCommand(new byte[]{Fields.TS_PERF_TRACE_BEGIN}, "begin trace");

                try {
                    Thread.sleep(500);

                    byte[] packet = bp.executeCommand(new byte[]{Fields.TS_PERF_TRACE_GET_BUFFER}, "get trace", true);
                    if (!checkResponseCode(packet, RESPONSE_OK) || ((packet.length - 1) % 8) != 0)
                        throw new IllegalStateException("Unexpected packet");

                    List<Entry> data = Entry.parseBuffer(packet);

                    int rpm = RpmModel.getInstance().getValue();
                    String fileName = Logger.getDate() + "_rpm_" + rpm + "_rusEfi_trace" + ".json";


                    JsonOutput.writeToStream(data, new FileOutputStream(fileName));
                } catch (IOException | InterruptedException e1) {
                    throw new IllegalStateException(e1);
                }
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
        final JComboBox<Integer> indexes = createIndexCombo();
        CommandControl panel = new CommandControl(uiContext,"Spark #", "spark.jpg", TEST, indexes) {
            @Override
            protected String getCommand() {
                return "sparkbench2 1000 " + indexes.getSelectedItem() + " 5 333 3";
            }
        };
        return panel.getContent();
    }

    private Component createInjectorTest() {
        final JComboBox<Integer> indexes = createIndexCombo();
        CommandControl panel = new CommandControl(uiContext,"Injector #", "injector.png", TEST, indexes) {
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