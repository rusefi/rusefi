package com.rusefi.maintenance;

import com.rusefi.Launcher;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.StatusWindow;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
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

    private final JButton button;

    public StLinkFlasher(String fileName, String buttonTest, String tooltip) {
        button = new JButton(buttonTest);
        button.setToolTipText(tooltip);
        button.addActionListener(event -> doUpdateFirmware(fileName, button));
    }

    public static void doUpdateFirmware(String fileName, JComponent parent) {
        StatusWindow wnd = new StatusWindow();
        int dialogResult = JOptionPane.showConfirmDialog(parent, "Do you really want to update firmware? Please disconnect vehicle battery before erasing.",
                "Please disconnect from vehicle", JOptionPane.YES_NO_OPTION);
        if (dialogResult != JOptionPane.YES_OPTION)
            return;

        wnd.getFrame().setLocationRelativeTo(parent);
        wnd.showFrame(TITLE);

        ExecHelper.submitAction(() -> doFlashFirmware(wnd, fileName), StLinkFlasher.class + " extProcessThread");
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

    private static void doFlashFirmware(StatusWindow wnd, String fileName) {
        if (!new File(fileName).exists()) {
            wnd.appendLine(fileName + " not found, cannot proceed !!!");
            wnd.setStatus("ERROR");
            return;
        }
        StatusWindowAnimation sa = new StatusWindowAnimation(wnd);
      String error = null;
      try {
        error = executeOpenOCDCommand(getOpenocdCommand() + " -c \"program " +
                fileName +
                " verify reset exit 0x08000000\"", wnd);
      } catch (FileNotFoundException e) {
        wnd.appendLine(e.toString());
        wnd.error();
        return;
      }
      if (error.contains(SUCCESS_MESSAGE_TAG) && !error.toLowerCase().contains(FAILED_MESSAGE_TAG)) {
            wnd.appendLine("Flashing looks good!");
            sa.stop();
            wnd.setStatus(DONE);
            wnd.setSuccessState();
        } else {
            wnd.setErrorState();
            wnd.appendLine("!!! FIRMWARE FLASH: DOES NOT LOOK RIGHT !!!");
        }
    }

    public static boolean detectStLink(UpdateOperationCallbacks wnd) {
        return MaintenanceUtil.detectDevice(wnd, WMIC_STLINK_QUERY_COMMAND, "STLink");
    }

    public JButton getButton() {
        return button;
    }

    @NotNull
    public static HwPlatform getHardwareKind() {
        String bundle = BundleUtil.readBundleFullNameNotNull();
        if (bundle.contains("h7"))
            return HwPlatform.H7;
        if (bundle.contains("f7"))
            return HwPlatform.F7;
        // how does this "hardware" key even work? does it work? shall we rely on bundle name like above?
        String value = getConfig().getRoot().getProperty("hardware", HwPlatform.F4.name());
        return HwPlatform.resolve(value);
    }

}
