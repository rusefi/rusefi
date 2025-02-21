package com.rusefi.maintenance;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.panama.PanamaClient;
import com.rusefi.ui.basic.InstanceNameEditor;

public enum TuneUploader {
    INSTANCE;

    public synchronized boolean uploadTune(
        final SerialPortScanner.PortResult port,
        final String panamaUrl,
        final UpdateOperationCallbacks callbacks
    ) {
        boolean result = false;
        // todo!
        // IniField mcuSerialField = PanamaHelper.getIniField(linkManager);
//        if (mcuSerialField == null) {
//           addMessage("Please update firmware to use this feature");
        // return;
//        }
// todo: grab current calibrations and save fresh MSQ
        int mcuSerial = 1231234; // todo

        if (PanamaClient.uploadFile(panamaUrl,
            /* todo MSQ file */ null,
            InstanceNameEditor.loadInstanceName(),
            mcuSerial
        )) {
            result = true;
        } else {
            callbacks.logLine(String.format("Failed to upload file to %s", panamaUrl));
        }
        return result;
    }
}
