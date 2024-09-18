package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.io.UpdateOperationCallbacks;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FileNotFoundException;

import static com.devexperts.logging.Logging.getLogging;

/**
 * This code automates drivers unpacking and installation
 * new, DFU Virtual Comport and ST-Link drivers are installed
 * <p>
 * See https://github.com/rusefi/rusefi/tree/master/misc/install_st
 */
public class DriverInstall {
    private static final Logging log = getLogging(DriverInstall.class);
    private static final String FOLDER = "../drivers";
    private static final String SELF_UNCOMPRESSING_ARCHIVE = "silent_st_drivers2.exe";
    private static final String YES = " -y";
    private static final String UNPACKED_FOLDER = FOLDER + File.separator + "silent_st_drivers";
    private static final String WINDOWS7_BATCH = "silent_install_windows7.bat";
    private static final String WINDOWS8_BATCH = "silent_install_windows8.bat";

    public static Component createButton() {
        JButton button = new JButton("Install Drivers");
        button.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int dialogResult = JOptionPane.showConfirmDialog(button, "Do you really want to install ST drivers?",
                        "Please disconnect from vehicle", JOptionPane.YES_NO_OPTION);
                if (dialogResult != JOptionPane.YES_OPTION)
                    return;

                final UpdateOperationCallbacks wnd = new UpdateStatusWindow("Windows rusEFI ST Drivers");

                ExecHelper.submitAction(() -> installDrivers(wnd), getClass() + " thread");

            }
        });

        return button;
    }

    private static void installDrivers(UpdateOperationCallbacks wnd) {
        log.info("IsWindows=" + FileLog.isWindows());
        if (!new File(FOLDER).exists()) {
            String message = FOLDER + " not found";
            wnd.appendLine(message);
            log.info(message);
            return;
        }
      try {
        ExecHelper.executeCommand(FOLDER,
                FOLDER + File.separator + SELF_UNCOMPRESSING_ARCHIVE + YES,
                SELF_UNCOMPRESSING_ARCHIVE,
                wnd);

      String batch = isWindows7orBelow() ? WINDOWS7_BATCH : WINDOWS8_BATCH;
        ExecHelper.executeCommand(UNPACKED_FOLDER, ExecHelper.getBatchCommand(batch), batch, wnd);
      } catch (FileNotFoundException e) {
        wnd.appendLine(e.toString());
        wnd.error();
      }
    }

    private static boolean isWindows7orBelow() {
        String version = System.getProperty(FileLog.OS_VERSION);
        // https://en.wikipedia.org/wiki/List_of_Microsoft_Windows_versions
        return version.startsWith("5.") || version.startsWith("6.0") || version.startsWith("6.1");
    }

    public static boolean isFolderExist() {
        return new File(FOLDER).exists();
    }
}
