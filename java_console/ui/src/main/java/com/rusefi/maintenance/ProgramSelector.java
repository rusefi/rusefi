package com.rusefi.maintenance;

import com.rusefi.Launcher;
import com.rusefi.SerialPortScanner;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;
import java.util.function.Consumer;

import static com.rusefi.StartupFrame.appendBundleName;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.ui.util.UiUtils.trueLayout;

public class ProgramSelector {

    private static final String AUTO_DFU = "Auto DFU Update";
    private static final String MANUAL_DFU = "Manual DFU Update";
    private static final String DFU_SWITCH = "Switch to DFU Mode";
    private static final String OPENBLT_SWITCH = "Switch to OpenBLT Mode";
    private static final String OPENBLT_MANUAL = "Manual OpenBLT Update";
    private static final String OPENBLT_AUTO = "Auto OpenBLT Update";
    private static final String DFU_ERASE = "Full Chip Erase";
    private static final String OPENBLT_CAN = "OpenBLT via CAN";

    public static final boolean IS_WIN = System.getProperty("os.name").toLowerCase().contains("win");

    private static final String HELP = "https://github.com/rusefi/rusefi/wiki/HOWTO-Update-Firmware";

    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel noHardware = new JLabel("Nothing detected");
    private final JPanel controls = new JPanel(new FlowLayout());
    private final JComboBox<String> mode = new JComboBox<>();

    public ProgramSelector(JComboBox<SerialPortScanner.PortResult> comboPorts) {
        content.add(controls, BorderLayout.NORTH);
        content.add(noHardware, BorderLayout.SOUTH);
        controls.setVisible(false);
        controls.add(mode);

        String persistedMode = getConfig().getRoot().getProperty(getClass().getSimpleName());
        if (Arrays.asList(AUTO_DFU, MANUAL_DFU, OPENBLT_CAN, OPENBLT_SWITCH, OPENBLT_MANUAL, OPENBLT_AUTO, DFU_ERASE, DFU_SWITCH).contains(persistedMode))
            mode.setSelectedItem(persistedMode);

        JButton updateFirmware = new JButton("Update Firmware",
                UiUtils.loadIcon("upload48.png"));
        controls.add(updateFirmware);

        comboPorts.addItemListener(this::selectedPortChanged);

        updateFirmware.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                final String selectedMode = (String) mode.getSelectedItem();
                final SerialPortScanner.PortResult selectedPort = ((SerialPortScanner.PortResult) comboPorts.getSelectedItem());

                if (selectedPort != null && selectedPort.signature != null && !selectedPort.signature.matchesBundle()) {
                    String target = BundleUtil.getBundleTarget();
                    String message = String.format("Looks like you're trying to update your ECU with the wrong firmware.\n\nYour controller says it's a %s, but this bundle is for a %s.\n\nYou can attempt to proceed, but unexpected behavior may result.\nContinue at your own risk.", selectedPort.signature.getBundleTarget(), target);
                    int result = JOptionPane.showConfirmDialog(comboPorts, message, "WARNING", JOptionPane.OK_CANCEL_OPTION);

                    if (result != JOptionPane.OK_OPTION) {
                        return;
                    }
                }

                getConfig().getRoot().setProperty(getClass().getSimpleName(), selectedMode);

                String jobName;
                Consumer<UpdateOperationCallbacks> job;

                Objects.requireNonNull(selectedMode);
                switch (selectedMode) {
                    case AUTO_DFU:
                        jobName = "DFU update";
                        job = (callbacks) -> DfuFlasher.doAutoDfu(comboPorts, selectedPort.port, callbacks);
                        break;
                    case MANUAL_DFU:
                        jobName = "DFU update";
                        job = DfuFlasher::runDfuProgramming;
                        break;
                    case DFU_SWITCH:
                        jobName = "DFU switch";
                        job = (callbacks) -> rebootToDfu(selectedPort.port, callbacks);
                        break;
                    case OPENBLT_SWITCH:
                        jobName = "OpenBLT switch";
                        job = (callbacks) -> rebootToOpenblt(selectedPort.port, callbacks);
                        break;
                    case OPENBLT_CAN:
                        jobName = "OpenBLT via CAN";
                        job = ProgramSelector.this::flashOpenBltCan;
                        break;
                    case OPENBLT_MANUAL:
                        jobName = "OpenBLT via Serial";
                        job = (callbacks) -> flashOpenbltSerialJni(selectedPort.port, callbacks);
                        break;
                    case OPENBLT_AUTO:
                        jobName = "OpenBLT via Serial";
                        job = (callbacks) -> flashOpenbltSerialAutomatic(selectedPort.port, callbacks);
                        break;
                    case DFU_ERASE:
                        jobName = "DFU erase";
                        job = DfuFlasher::runDfuEraseAsync;
                        break;
                    default:
                        throw new IllegalArgumentException("How did you " + selectedMode);
                }

                final UpdateOperationCallbacks callbacks = new UpdateStatusWindow(appendBundleName(jobName + " " + Launcher.CONSOLE_VERSION));
                final Consumer<UpdateOperationCallbacks> job2 = job;
                ExecHelper.submitAction(() -> {
                    SerialPortScanner.INSTANCE.stopTimer();
                    job2.accept(callbacks);
                    SerialPortScanner.INSTANCE.startTimer();
                }, "mx");
            }
        });
    }

    private static void rebootToDfu(String selectedPort, UpdateOperationCallbacks callbacks) {
        DfuFlasher.rebootToDfu(selectedPort, callbacks, Fields.CMD_REBOOT_DFU);
    }

    private static void rebootToOpenblt(String selectedPort, UpdateOperationCallbacks callbacks) {
        DfuFlasher.rebootToDfu(selectedPort, callbacks, Fields.CMD_REBOOT_OPENBLT);
    }

    private void flashOpenBltCan(UpdateOperationCallbacks callbacks) {
        OpenbltJni.OpenbltCallbacks cb = makeOpenbltCallbacks(callbacks);

        try {
            OpenbltJni.flashCan("../fome_update.srec", cb);

            callbacks.log("Update completed successfully!");
            callbacks.done();
        } catch (Throwable e) {
            callbacks.log("Error: " + e);
            callbacks.error();
        } finally {
            OpenbltJni.stop(cb);
        }
    }

    private void flashOpenbltSerialAutomatic(String fomePort, UpdateOperationCallbacks callbacks) {
        String[] portsBefore = LinkManager.getCommPorts();
        rebootToOpenblt(fomePort, callbacks);

        // Give the bootloader a sec to enumerate
        BinaryProtocol.sleep(3000);

        String[] portsAfter = LinkManager.getCommPorts();

        // Check that the ECU disappeared from the "after" list
        if (Arrays.asList(portsAfter).contains(fomePort)) {
            callbacks.log("Looks like your ECU didn't reboot to OpenBLT");
            callbacks.error();
            return;
        }

        // Check that exactly one thing appeared in the "after" list
        ArrayList<String> newItems = new ArrayList<>();
        for (String s : portsAfter) {
            if (Arrays.stream(portsBefore).noneMatch(s::equals)) {
                // This item is in the after list but not before list
                newItems.add(s);
            }
        }

        if (newItems.isEmpty()) {
            callbacks.log("Looks like your ECU disappeared during the update process. Please try again.");
            callbacks.error();
            return;
        }

        if (newItems.size() > 1) {
            // More than one port appeared? whattt?
            callbacks.log("Unable to find ECU after reboot as multiple serial ports appeared. Before: " + portsBefore.length + " After: " + portsAfter.length);
            callbacks.error();
            return;
        }

        String openbltPort = newItems.get(0);

        // Check that the one that appeared is indeed OpenBLT
        boolean isOpenBlt = SerialPortScanner.isPortOpenblt(openbltPort);

        if (!isOpenBlt) {
            callbacks.log("A serial port appeared as it should, but OpenBLT didn't respond.");
            callbacks.error();
            return;
        }

        callbacks.log("Serial port " + openbltPort + " appeared and looks like OpenBLT, programming firmware...");

        flashOpenbltSerialJni(openbltPort, callbacks);
    }

    private OpenbltJni.OpenbltCallbacks makeOpenbltCallbacks(UpdateOperationCallbacks callbacks) {
        return new OpenbltJni.OpenbltCallbacks() {
            @Override
            public void log(String line) {
                callbacks.log(line);
            }

            @Override
            public void updateProgress(int percent) {
                callbacks.log("Progress: " + percent + "%");
            }

            @Override
            public void error(String line) {
                throw new RuntimeException(line);
            }

            @Override
            public void setPhase(String title, boolean hasProgress) {
                callbacks.log("Begin phase: " + title);
            }
        };
    }

    private void flashOpenbltSerialJni(String port, UpdateOperationCallbacks callbacks) {
        OpenbltJni.OpenbltCallbacks cb = makeOpenbltCallbacks(callbacks);

        try {
            OpenbltJni.flashSerial("../fome_update.srec", port, cb);

            callbacks.log("Update completed successfully!");
            callbacks.done();
        } catch (Throwable e) {
            callbacks.log("Error: " + e);
            callbacks.error();
        } finally {
            OpenbltJni.stop(cb);
        }
    }

    @NotNull
    public static JComponent createHelpButton() {
        return new URLLabel("HOWTO Update Firmware", HELP);
    }

    public JPanel getControl() {
        return content;
    }

    private SerialPortScanner.AvailableHardware currentHardware = new SerialPortScanner.AvailableHardware(new ArrayList<>(), false);

    private void addDfuItems() {
        if (currentHardware.dfuFound) {
            mode.addItem(MANUAL_DFU);
            mode.addItem(DFU_ERASE);
        }
    }

    private void selectedPortChanged(ItemEvent e) {
        mode.removeAllItems();

        if (e != null) {
            SerialPortScanner.PortResult pr = (SerialPortScanner.PortResult) e.getItem();

            // Prefer OpenBLT so put that option first
            if (pr.type == SerialPortScanner.SerialPortType.FomeEcuWithOpenblt) {
                mode.addItem(OPENBLT_AUTO);
                mode.addItem(OPENBLT_SWITCH);
            }

            if (IS_WIN) {
                if (pr.isEcu()) {
                    mode.addItem(AUTO_DFU);
                }

                addDfuItems();
            }

            if (pr.isEcu()) {
                mode.addItem(DFU_SWITCH);
            }

            if (pr.type == SerialPortScanner.SerialPortType.OpenBlt) {
                mode.addItem(OPENBLT_MANUAL);
            }
        }
        else
        {
            // No ports, just show DFU items (if present)
            if (IS_WIN) {
                addDfuItems();
            }
        }

        // Show update controls if there are any options
        controls.setVisible(0 != mode.getItemCount());

        trueLayout(mode);
        trueLayout(content);
    }

    public void apply(SerialPortScanner.AvailableHardware currentHardware) {
        this.currentHardware = currentHardware;

        // If no ports, force an update with nothing selected
        if (currentHardware.getKnownPorts().isEmpty()) {
            selectedPortChanged(null);
        }

        noHardware.setVisible(currentHardware.isEmpty());

        trueLayout(mode);
        trueLayout(content);
    }
}
