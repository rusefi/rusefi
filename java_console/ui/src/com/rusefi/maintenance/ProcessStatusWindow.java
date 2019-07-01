package com.rusefi.maintenance;

import com.rusefi.ui.StatusWindow;

/**
 * (c) Andrey Belomutskiy 2013-2018
 */
public class ProcessStatusWindow {
    protected final StatusWindow wnd = new StatusWindow();

    protected StringBuffer executeCommand(String command) {
        return ExecHelper.executeCommand(FirmwareFlasher.BINARY_LOCATION, command, FirmwareFlasher.OPENOCD_EXE, wnd);
    }
}