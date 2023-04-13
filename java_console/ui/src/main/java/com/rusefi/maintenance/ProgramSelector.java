package com.rusefi.maintenance;

import com.rusefi.Launcher;
import com.rusefi.SerialPortScanner;
import com.rusefi.autodetect.PortDetector;
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

import static com.rusefi.StartupFrame.appendBundleName;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.ui.util.UiUtils.trueLayout;

public class ProgramSelector {

    private static final String AUTO_DFU = "Auto Update";
    private static final String MANUAL_DFU = "Manual DFU Update";
    private static final String DFU_SWITCH = "Switch to DFU Mode";
    private static final String DFU_ERASE = "Full Chip Erase";
    private static final String OPENBLT_CAN = "OpenBLT via CAN";

    public static final boolean IS_WIN = System.getProperty("os.name").toLowerCase().contains("win");

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
        if (Arrays.asList(AUTO_DFU, MANUAL_DFU, OPENBLT_CAN , DFU_ERASE, DFU_SWITCH).contains(persistedMode))
            mode.setSelectedItem(persistedMode);

        JButton updateFirmware = new JButton("Update Firmware",
                UiUtils.loadIcon("upload48.png"));
        controls.add(updateFirmware);

        updateFirmware.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String selectedMode = (String) mode.getSelectedItem();

                getConfig().getRoot().setProperty(getClass().getSimpleName(), selectedMode);

                Objects.requireNonNull(selectedMode);
                switch (selectedMode) {
                    case AUTO_DFU:
                        DfuFlasher.doAutoDfu(comboPorts.getSelectedItem(), comboPorts);
                        break;
                    case MANUAL_DFU:
                        DfuFlasher.runDfuProgramming();
                        break;
                    case DFU_SWITCH:
                        StatusWindow wnd = DfuFlasher.createStatusWindow();
                        Object selected = comboPorts.getSelectedItem();
                        String port = selected == null ? PortDetector.AUTO : selected.toString();
                        DfuFlasher.rebootToDfu(comboPorts, port, wnd);
                        break;
                    case OPENBLT_CAN:
                        flashOpenBltCan();
                        break;
                    case DFU_ERASE:
                        DfuFlasher.runDfuErase();
                        break;
                    default:
                        throw new IllegalArgumentException("How did you " + selectedMode);
                }
            }
        });

    }

    private void flashOpenBltCan() {
        StatusWindow wnd = new StatusWindow();
        wnd.showFrame(appendBundleName("OpenBLT via CAN " + Launcher.CONSOLE_VERSION));
        ExecHelper.submitAction(() -> {
            ExecHelper.executeCommand(OPENBLT_BINARY_LOCATION,
                    OPENBLT_BINARY_LOCATION + "/" + BOOT_COMMANDER_EXE +
                            " -s=xcp -t=xcp_can -d=peak_pcanusb -t1=1000 -t3=2000 -t4=10000 -t5=1000 -t7=2000 ../../rusefi_update.srec",
                    BOOT_COMMANDER_EXE, wnd, new StringBuffer());
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

        mode.removeAllItems();
        if (IS_WIN) {
            if (!currentHardware.getKnownPorts().isEmpty())
                mode.addItem(AUTO_DFU);
            if (currentHardware.isDfuFound()) {
                mode.addItem(MANUAL_DFU);
                mode.addItem(DFU_ERASE);
            }
        }
        if (!currentHardware.getKnownPorts().isEmpty())
            mode.addItem(DFU_SWITCH);
        trueLayout(mode);
        UiUtils.trueLayout(content);
    }
}
