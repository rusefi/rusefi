package com.rusefi.maintenance;

import com.rusefi.FileLog;
import com.rusefi.Launcher;
import com.rusefi.SerialPortScanner;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.Arrays;
import java.util.Objects;

import static com.rusefi.core.ui.FrameHelper.appendBundleName;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.ui.util.UiUtils.trueLayout;

public class ProgramSelector {

    private static final String AUTO_DFU = "Auto Update";
    private static final String MANUAL_DFU = "Manual DFU Update";
    private static final String DFU_SWITCH = "Switch to DFU Mode";
    private static final String OPENBLT_SWITCH = "Switch to OpenBLT Mode";
    private static final String DFU_ERASE = "Full Chip Erase";
    private static final String ST_LINK = "ST-LINK Update";
    private static final String OPENBLT_CAN = "OpenBLT via CAN";

    private static final String HELP = "https://github.com/rusefi/rusefi/wiki/HOWTO-Update-Firmware";
    public static final String BOOT_COMMANDER_EXE = "BootCommander.exe";
    public static final String OPENBLT_BINARY_LOCATION = Launcher.TOOLS_PATH + File.separator + "openblt";


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
        if (Arrays.asList(AUTO_DFU, MANUAL_DFU, OPENBLT_CAN, OPENBLT_SWITCH, DFU_ERASE, DFU_SWITCH).contains(persistedMode))
            mode.setSelectedItem(persistedMode);

        JButton updateFirmware = createUpdateFirmwareButton();
        controls.add(updateFirmware);

        updateFirmware.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String selectedMode = (String) mode.getSelectedItem();
                String selectedPort = (String) comboPorts.getSelectedItem();

                getConfig().getRoot().setProperty(getClass().getSimpleName(), selectedMode);

                Objects.requireNonNull(selectedMode);
                switch (selectedMode) {
                    case AUTO_DFU:
                        DfuFlasher.doAutoDfu(comboPorts, selectedPort, createStatusWindow("DFU update"));
                        break;
                    case MANUAL_DFU:
                        DfuFlasher.runDfuProgramming(createStatusWindow("DFU update"));
                        break;
                    case ST_LINK:
                        // todo: add ST-LINK no-assert mode? or not?
                        FirmwareFlasher.doUpdateFirmware(FirmwareFlasher.IMAGE_FILE, updateFirmware);
                        break;
                    case DFU_SWITCH:
                        rebootToDfu(comboPorts, selectedPort);
                        break;
                    case OPENBLT_SWITCH:
                        rebootToOpenblt(comboPorts, selectedPort);
                        break;
                    case OPENBLT_CAN:
                        flashOpenBltCan();
                        break;
                    case DFU_ERASE:
                        DfuFlasher.runDfuEraseAsync(createStatusWindow("DFU erase"));
                        break;
                    default:
                        throw new IllegalArgumentException("How did you " + selectedMode);
                }
            }
        });
    }

    @NotNull
    protected static UpdateOperationCallbacks createStatusWindow(String message) {
        return new UpdateStatusWindow(appendBundleName(message + " " + Launcher.CONSOLE_VERSION));
    }

    private static void rebootToDfu(JComponent parent, String selectedPort) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, createStatusWindow("DFU switch"), Fields.CMD_REBOOT_DFU);
    }

    private static void rebootToOpenblt(JComponent parent, String selectedPort) {
        String port = selectedPort == null ? PortDetector.AUTO : selectedPort;
        DfuFlasher.rebootToDfu(parent, port, createStatusWindow("OpenBLT switch"), Fields.CMD_REBOOT_OPENBLT);
    }

    @NotNull
    public static JButton createUpdateFirmwareButton() {
        return new JButton("Update Firmware", AutoupdateUtil.loadIcon("upload48.png"));
    }

    private void flashOpenBltCan() {
        UpdateOperationCallbacks callbacks = createStatusWindow("OpenBLT via CAN");
        ExecHelper.submitAction(() -> {
            ExecHelper.executeCommand(OPENBLT_BINARY_LOCATION,
                    OPENBLT_BINARY_LOCATION + "/" + BOOT_COMMANDER_EXE +
                            " -s=xcp -t=xcp_can -d=peak_pcanusb -t1=1000 -t3=2000 -t4=10000 -t5=1000 -t7=2000 ../../rusefi_update.srec",
                    BOOT_COMMANDER_EXE, callbacks, new StringBuffer());
            // it's a lengthy operation let's signal end
            Toolkit.getDefaultToolkit().beep();
        }, "OpenBLT via CAN");
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
        boolean hasDfuDevice = currentHardware.isDfuFound();

        mode.removeAllItems();
        if (FileLog.isWindows()) {
            if (hasSerialPorts) {
                mode.addItem(AUTO_DFU);
            }

            if (hasDfuDevice) {
                mode.addItem(MANUAL_DFU);
                mode.addItem(DFU_ERASE);
            }
            if (currentHardware.isStLinkConnected())
                mode.addItem(ST_LINK);
            if (currentHardware.isPCANConnected())
                mode.addItem(OPENBLT_CAN);
            // todo: detect PCAN mode.addItem(OPENBLT_CAN);
        }

        // If any serial port is detected, offer the option to switch to DFU
        if (hasSerialPorts) {
            // mode.addItem(AUTO_OPENBLT);
            mode.addItem(DFU_SWITCH);
            mode.addItem(OPENBLT_SWITCH);
            // mode.addItem(MANUAL_OPENBLT);
        }

        trueLayout(mode);
        trueLayout(content);
    }
}
