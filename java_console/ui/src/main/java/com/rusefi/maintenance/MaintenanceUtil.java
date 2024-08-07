package com.rusefi.maintenance;

import com.rusefi.core.FindFileHelper;
import com.rusefi.io.UpdateOperationCallbacks;

import java.io.File;

public class MaintenanceUtil {

    private static final String WMIC_PCAN_QUERY_COMMAND = "wmic path win32_pnpentity where \"Caption like '%PCAN-USB%'\" get Caption,ConfigManagerErrorCode /format:list";

    static boolean detectDevice(UpdateOperationCallbacks callbacks, String queryCommand, String pattern) {
        //        long now = System.currentTimeMillis();
        StringBuffer output = new StringBuffer();
        StringBuffer error = new StringBuffer();
        try {
            ExecHelper.executeCommand(queryCommand, callbacks, output, error, null);
        } catch (ErrorExecutingCommand e) {
            callbacks.logLine("IOError: " + e);
            // let's assume DFU is present just to give user more options
            return true;
        }
        callbacks.logLine(output.toString());
        callbacks.logLine(error.toString());
//        long cost = System.currentTimeMillis() - now;
//        System.out.println("DFU lookup cost " + cost + "ms");
        return output.toString().contains(pattern);
    }

    public static boolean detectPcan(UpdateOperationCallbacks wnd) {
        return detectDevice(wnd, WMIC_PCAN_QUERY_COMMAND, "PCAN");
    }

    public static long getBinaryModificationTimestamp() {
        String fileName = FindFileHelper.isObfuscated() ? FindFileHelper.findSrecFile() : FindFileHelper.FIRMWARE_BIN_FILE;
        return new File(fileName).lastModified();
    }
}
