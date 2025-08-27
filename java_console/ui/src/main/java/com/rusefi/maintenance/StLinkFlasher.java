package com.rusefi.maintenance;

import com.rusefi.Launcher;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.JobHelper;
import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.FileNotFoundException;

import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;

/**
 * @see DfuFlasher
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 2/4/15
 */
public class StLinkFlasher {
    /**
     * SWD ST-LINK/V2 mode
     */
    private static final String OPENOCD_EXE = Launcher.TOOLS_PATH + File.separator + "openocd/openocd.exe";
    // todo: combine this with Launcher#TOOLS_PATH?
    private static final String OPENOCD_BINARY_LOCATION = ".";
    private static final String SUCCESS_MESSAGE_TAG = "shutdown command invoked";
    private static final String FAILED_MESSAGE_TAG = "failed";
    public static final String TITLE = "rusEFI ST-LINK Firmware Flasher";
    public static final String DONE = "DONE!";
    private static final String WMIC_STLINK_QUERY_COMMAND = "wmic path win32_pnpentity where \"Caption like '%STLink%'\" get Caption,ConfigManagerErrorCode /format:list";

    public static void doUpdateFirmware(String fileName, UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        ExecHelper.submitAction(
            () -> JobHelper.doJob(() -> doFlashFirmware(callbacks, fileName), onJobFinished),
            StLinkFlasher.class + " extProcessThread"
        );
    }

    public static String getOpenocdCommand() {
        String cfg = getHardwareKind().getOpenOcdName();
        return OPENOCD_EXE + " -f openocd/" + cfg;
    }

    protected static String executeOpenOCDCommand(String command, UpdateOperationCallbacks wnd) throws FileNotFoundException {
        return ExecHelper.executeCommand(OPENOCD_BINARY_LOCATION,
                OPENOCD_BINARY_LOCATION + File.separator + command,
                OPENOCD_EXE, wnd);
    }

    private static void doFlashFirmware(UpdateOperationCallbacks wnd, String fileName) {
        if (!new File(fileName).exists()) {
            wnd.logLine(fileName + " not found, cannot proceed !!!");
            wnd.error();
            return;
        }
      String error;
      try {
        error = executeOpenOCDCommand(getOpenocdCommand() + " -c \"program " +
                fileName +
                " verify reset exit 0x08000000\"", wnd);
      } catch (FileNotFoundException e) {
        wnd.logLine(e.toString());
        wnd.error();
        return;
      }
      if (error.contains(SUCCESS_MESSAGE_TAG) && !error.toLowerCase().contains(FAILED_MESSAGE_TAG)) {
            wnd.logLine("Flashing looks good!");
            wnd.done();
        } else {
            wnd.error();
            wnd.logLine("!!! FIRMWARE FLASH: DOES NOT LOOK RIGHT !!!");
        }
    }

    public static boolean detectStLink(UpdateOperationCallbacks wnd) {
        return MaintenanceUtil.detectDevice(wnd, WMIC_STLINK_QUERY_COMMAND, "STLink", false);
    }

    @NotNull
    public static HwPlatform getHardwareKind() {
        String bundle = BundleUtil.readBundleFullNameNotNull().getTarget();
        if (bundle.contains("h7"))
            return HwPlatform.H7;
        if (bundle.contains("f7"))
            return HwPlatform.F7;
        // how does this "hardware" key even work? does it work? shall we rely on bundle name like above?
        String value = getConfig().getRoot().getProperty("hardware", HwPlatform.F4.name());
        return HwPlatform.resolve(value);
    }

}
