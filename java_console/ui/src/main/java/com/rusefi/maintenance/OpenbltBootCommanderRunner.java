package com.rusefi.maintenance;

import com.rusefi.Launcher;
import com.rusefi.io.UpdateOperationCallbacks;

import java.io.File;
import java.io.FileNotFoundException;

public final class OpenbltBootCommanderRunner {
    private static String getBootCommanderBinary() {
        String osName = System.getProperty("os.name").toLowerCase();
        if (osName.contains("win")) {
            return "BootCommander.exe";
        }

        if (osName.contains("mac") || osName.contains("darwin")) {
            return "BootCommander_macos";
        }

        return "BootCommander_linux";
    }

    private static final String BOOT_COMMANDER = getBootCommanderBinary();
    private static final String OPENBLT_BINARY_LOCATION = Launcher.TOOLS_PATH + File.separator + "openblt";

    public static void flashSerial(String port, String file, UpdateOperationCallbacks callbacks) {
        runOpenblt(file, callbacks, "-s=xcp -t=xcp_rs232 -d=" + port);
    }

    public static void flashCan(String file, UpdateOperationCallbacks callbacks) {
        runOpenblt(file, callbacks, "-s=xcp -t=xcp_can -d=peak_pcanusb -t1=1000 -t3=2000 -t4=10000 -t5=1000 -t7=2000");
    }

    private static void runOpenblt(String file, UpdateOperationCallbacks callbacks, String args) {
        File updateFile = new File(file);
        if (!updateFile.exists()) {
            callbacks.log("File not found: " + file);
            callbacks.error();
            return;
        }

        try {
            String cmd = OPENBLT_BINARY_LOCATION + "/" + BOOT_COMMANDER + " " + args + " " + updateFile.getAbsolutePath();

            callbacks.log("Running BootCommander like: " + cmd);

            ExecHelper.executeCommand(OPENBLT_BINARY_LOCATION, cmd, BOOT_COMMANDER, callbacks);

            // TODO: check result

            callbacks.done();
        } catch (FileNotFoundException e) {
            callbacks.log(e.toString());
            callbacks.error();
        }
    }
}
