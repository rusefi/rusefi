package com.rusefi.maintenance;

import com.rusefi.Launcher;
import com.rusefi.io.UpdateOperationCallbacks;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Locale;

public final class OpenbltBootCommanderRunner {
    private static final String OS_NAME = System.getProperty("os.name").toLowerCase(Locale.ROOT);
    private static final boolean IS_MAC = OS_NAME.contains("mac") || OS_NAME.contains("darwin");

    private static String getBootCommanderBinary() {
        if (OS_NAME.contains("win")) {
            return "BootCommander.exe";
        }

        if (IS_MAC) {
            return "BootCommander_macos";
        }

        return "BootCommander_linux";
    }

    private static final String BOOT_COMMANDER = getBootCommanderBinary();
    private static final String OPENBLT_BINARY_LOCATION = Launcher.TOOLS_PATH + File.separator + "openblt";

    public static void flashSerial(String port, String file, UpdateOperationCallbacks callbacks) {
        // On macOS, prepend "/dev/" to the serial port name
        if (IS_MAC) {
            port = "/dev/" + port;
        }

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

            ExecHelper.executeCommand(".", cmd, OPENBLT_BINARY_LOCATION + "/" + BOOT_COMMANDER, callbacks);

            // TODO: check result

            callbacks.done();
        } catch (FileNotFoundException e) {
            callbacks.log(e.toString());
            callbacks.error();
        }
    }
}
