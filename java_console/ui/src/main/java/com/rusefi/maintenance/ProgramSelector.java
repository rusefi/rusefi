package com.rusefi.maintenance;

import com.rusefi.autodetect.PortDetector;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;
import java.util.Objects;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class ProgramSelector {

    private static final String AUTO_DFU = "Auto Update";
    private static final String MANUAL_DFU = "Manual DFU Update";
    private static final String DFU_SWITCH = "Switch to DFU Mode";
    private static final String DFU_ERASE = "Full Chip Erase";
    private static final String ST_LINK = "ST-LINK Update";
    public static final boolean IS_WIN = System.getProperty("os.name").toLowerCase().contains("win");

    private static final String HELP = "https://github.com/rusefi/rusefi/wiki/HOWTO-Update-Firmware";

    private final JPanel controls = new JPanel(new FlowLayout());
    private final JComboBox<String> mode = new JComboBox<>();

    public ProgramSelector(JComboBox<String> comboPorts) {
        /*
         * todo: add FULL AUTO mode which would fire up DFU and ST-LINK in parallel hoping that one of those would work?
         */
        if (IS_WIN) {
            mode.addItem(AUTO_DFU);
            mode.addItem(MANUAL_DFU);
            mode.addItem(DFU_ERASE);
            mode.addItem(ST_LINK);
        }
        mode.addItem(DFU_SWITCH);

        controls.add(mode);

        String persistedMode = getConfig().getRoot().getProperty(getClass().getSimpleName());
        if (Arrays.asList(AUTO_DFU, MANUAL_DFU, ST_LINK, DFU_ERASE, DFU_SWITCH).contains(persistedMode))
            mode.setSelectedItem(persistedMode);

        JButton updateFirmware = new JButton("Update Firmware",
                AutoupdateUtil.loadIcon("upload48.png"));
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
                    case ST_LINK:
                        // todo: add ST-LINK no-assert mode? or not?
                        FirmwareFlasher.doUpdateFirmware(FirmwareFlasher.IMAGE_FILE, updateFirmware);
                        break;
                    case DFU_SWITCH:
                        StatusWindow wnd = DfuFlasher.createStatusWindow();
                        Object selected = comboPorts.getSelectedItem();
                        String port = selected == null ? PortDetector.AUTO : selected.toString();
                        DfuFlasher.rebootToDfu(comboPorts, port, wnd);
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

    @NotNull
    public static JComponent createHelpButton() {
        return new URLLabel("HOWTO Update Firmware", HELP);
    }

    public JPanel getControl() {
        return controls;
    }
}
