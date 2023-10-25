package com.rusefi.maintenance;

import com.rusefi.Launcher;
import com.rusefi.SerialPortScanner;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;
import java.util.function.Consumer;

import static com.rusefi.StartupFrame.appendBundleName;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.ui.util.UiUtils.trueLayout;

public class ProgramSelector {

    private static final String AUTO_DFU = "Auto Update";
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

    public ProgramSelector(JComboBox<String> comboPorts) {
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

        updateFirmware.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                final String selectedMode = (String) mode.getSelectedItem();
                final String selectedPort = (String) comboPorts.getSelectedItem();

                getConfig().getRoot().setProperty(getClass().getSimpleName(), selectedMode);

                String jobName = null;
                Consumer<UpdateOperationCallbacks> job = null;

                Objects.requireNonNull(selectedMode);
                switch (selectedMode) {
                    case AUTO_DFU:
                        jobName = "DFU update";
                        job = (callbacks) -> DfuFlasher.doAutoDfu(comboPorts, selectedPort, callbacks);
                        break;
                    case MANUAL_DFU:
                        jobName = "DFU update";
                        job = DfuFlasher::runDfuProgramming;
                        break;
                    case DFU_SWITCH:
                        jobName = "DFU switch";
                        job = (callbacks) -> rebootToDfu(comboPorts, selectedPort, callbacks);
                        break;
                    case OPENBLT_SWITCH:
                        jobName = "OpenBLT switch";
                        job = (callbacks) -> rebootToOpenblt(comboPorts, selectedPort, callbacks);
                        break;
                    case OPENBLT_CAN:
                        jobName = "OpenBLT via CAN";
                        job = ProgramSelector.this::flashOpenBltCan;
                        break;
                    case OPENBLT_MANUAL:
                        jobName = "OpenBLT via Serial";
                        job = (callbacks) -> flashOpenbltSerial(selectedPort, callbacks);
                        break;
                    case OPENBLT_AUTO:
                        jobName = "OpenBLT via Serial";
                        job = (callbacks) -> flashOpenbltSerialAutomatic(comboPorts, selectedPort, callbacks);
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
                ExecHelper.submitAction(() -> job2.accept(callbacks), "mx");
            }
        });
    }

    private static void rebootToDfu(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Fields.CMD_REBOOT_DFU);
    }

    private static void rebootToOpenblt(JComponent parent, String selectedPort, UpdateOperationCallbacks callbacks) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, callbacks, Fields.CMD_REBOOT_OPENBLT);
    }

    private void flashOpenBltCan(UpdateOperationCallbacks callbacks) {
        OpenbltBootCommanderRunner.flashCan("../../rusefi_update.srec", callbacks);

        // it's a lengthy operation let's signal end
        Toolkit.getDefaultToolkit().beep();
    }

    private void flashOpenbltSerialAutomatic(JComponent parent, String fomePort, UpdateOperationCallbacks callbacks) {
        String[] portsBefore = LinkManager.getCommPorts();
        rebootToOpenblt(parent, fomePort, callbacks);

        // Give the bootloader a sec to enumerate
        BinaryProtocol.sleep(3000);

        String[] portsAfter = LinkManager.getCommPorts();

        // Check that the ECU disappeared from the "after" list
        if (!PortDetector.AUTO.equals(fomePort) && Arrays.stream(portsAfter).anyMatch(fomePort::equals)) {
            callbacks.log("Looks like your ECU didn't reboot to OpenBLT");
            callbacks.error();
            return;
        }

        // Check that exactly one thing appeared in the "after" list
        ArrayList<String> newItems = new ArrayList<>();
        for (String s : portsAfter) {
            if (!Arrays.stream(portsBefore).anyMatch(s::equals)) {
                // This item is in the after list but not before list
                newItems.add(s);
            }
        }

        if (newItems.size() == 0) {
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

        callbacks.log("Serial port " + openbltPort + " appeared, programming firmware...");

        flashOpenbltSerial(openbltPort, callbacks);
    }

    private void flashOpenbltSerial(String port, UpdateOperationCallbacks callbacks) {
        // We can't auto detect OpenBLT port yet
        if (port == null || PortDetector.AUTO.equals(port)) {
            callbacks.log("Invalid serial port for OpenBLT: " + port);
            callbacks.error();
            return;
        }

        OpenbltBootCommanderRunner.flashSerial(port, "../../fome_update.srec", callbacks);

        // it's a lengthy operation let's signal end
        Toolkit.getDefaultToolkit().beep();
    }

    @NotNull
    public static JComponent createHelpButton() {
        return new URLLabel("HOWTO Update Firmware", HELP);
    }

    public JPanel getControl() {
        return content;
    }

    public void apply(SerialPortScanner.AvailableHardware currentHardware) {
        noHardware.setVisible(currentHardware.isEmpty());
        controls.setVisible(!currentHardware.isEmpty());

        boolean hasSerialPorts = !currentHardware.getKnownPorts().isEmpty();
        boolean hasDfuDevice = currentHardware.dfuFound;
        boolean hasEcu = currentHardware.hasAnyEcu;
        boolean hasOpenblt = currentHardware.hasAnyOpenblt;

        mode.removeAllItems();
        if (IS_WIN) {
            if (hasSerialPorts) {
                mode.addItem(AUTO_DFU);
            }

            if (hasDfuDevice) {
                mode.addItem(MANUAL_DFU);
                mode.addItem(DFU_ERASE);
            }
        }

        if (hasEcu) {
            mode.addItem(DFU_SWITCH);
            mode.addItem(OPENBLT_AUTO);
            mode.addItem(OPENBLT_SWITCH);
        }

        if (hasOpenblt) {
            mode.addItem(OPENBLT_MANUAL);
        }

        trueLayout(mode);
        trueLayout(content);
    }
}
