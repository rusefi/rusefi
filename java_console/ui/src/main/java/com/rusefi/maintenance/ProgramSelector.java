package com.rusefi.maintenance;

import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.ui.util.URLLabel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class ProgramSelector {

    private static final String AUTO_DFU = "Auto DFU";
    private static final String MANUAL_DFU = "Manual DFU";
    private static final String ST_LINK = "ST-LINK";

    private static final String HELP = "https://github.com/rusefi/rusefi/wiki/HOWTO-Update-Firmware";

    private final JPanel controls = new JPanel(new FlowLayout());
    private final JComboBox mode = new JComboBox();

    public ProgramSelector(JComboBox<String> comboPorts) {
        /**
         * todo: add FULL AUTO mode which would fire up DFU and ST-LINK in parallel hoping that one of those would work?
         */
        mode.addItem(AUTO_DFU);
        mode.addItem(MANUAL_DFU);
        mode.addItem(ST_LINK);

        controls.add(mode);

        String persistedMode = getConfig().getRoot().getProperty(getClass().getSimpleName());
        if (Arrays.asList(AUTO_DFU, MANUAL_DFU, ST_LINK).contains(persistedMode))
            mode.setSelectedItem(persistedMode);

        JButton updateFirmware = new JButton("Update Firmware",
                AutoupdateUtil.loadIcon("upload48.jpg"));
        controls.add(updateFirmware);
        JButton updateHelp = new JButton("?");
        updateHelp.addActionListener(e -> URLLabel.open(HELP));
        controls.add(updateHelp);

        updateFirmware.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String selectedMode = (String) mode.getSelectedItem();

                getConfig().getRoot().setProperty(getClass().getSimpleName(), selectedMode);

                boolean isAutoDfu = selectedMode.equals(AUTO_DFU);
                boolean isManualDfu = selectedMode.equals(MANUAL_DFU);
                // todo: add ST-LINK no-assert mode

                if (isAutoDfu) {
                    DfuFlasher.doAutoDfu(comboPorts.getSelectedItem(), comboPorts);
                } else if (isManualDfu){
                    DfuFlasher.runDfuProgramming();
                } else {
                    FirmwareFlasher.doUpdateFirmware(FirmwareFlasher.IMAGE_FILE, updateFirmware);
                }
            }
        });

    }

    public JPanel getControl() {
        return controls;
    }
}
