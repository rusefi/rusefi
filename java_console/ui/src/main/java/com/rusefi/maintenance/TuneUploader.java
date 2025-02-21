package com.rusefi.maintenance;

import com.opensr5.ini.field.IniField;
import com.rusefi.SerialPortScanner;
import com.rusefi.binaryprotocol.BinaryProtocolLocalCache;
import com.rusefi.config.generated.TsOutputs;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.panama.PanamaClient;
import com.rusefi.ui.basic.InstanceNameEditor;

import java.io.File;
import java.util.Optional;

import static com.rusefi.maintenance.CalibrationsHelper.readAndBackupCurrentCalibrations;

public enum TuneUploader {
    INSTANCE;

    private static final String CALIBRATIONS_TO_UPLOAD_FILE_NAME = BinaryProtocolLocalCache.STATE_FOLDER + "calibrations_to_upload";

    public synchronized boolean uploadTune(
        final SerialPortScanner.PortResult ecuPort,
        final String panamaUrl,
        final UpdateOperationCallbacks callbacks
    ) {
        boolean result = false;

        final String instanceName = InstanceNameEditor.loadInstanceName();
        if (instanceName == null || instanceName.isEmpty()) {
            callbacks.logLine("Instance name is not defined!");
            callbacks.logLine("Please right-click on logo and use `Instance name` context menu  to specify an instance name.");
        }

        final Optional<CalibrationsInfo> calibrationsToUpload = readAndBackupCurrentCalibrations(
            ecuPort,
            callbacks,
            CALIBRATIONS_TO_UPLOAD_FILE_NAME
        );
        if (!calibrationsToUpload.isPresent()) {
            callbacks.logLine("Failed to back up current calibrations...");
            return false;
        }

        final Optional<Integer> receivedMcuSerial = OutputChannelsHelper.readMcuSerial(ecuPort, callbacks);
        if (!receivedMcuSerial.isPresent()) {
            callbacks.logLine("Failed to read `MCUSERIAL` output channel");
            return false;
        }
        final int mcuSerial = receivedMcuSerial.get();

        final String calibrationsToUploadFileName = CALIBRATIONS_TO_UPLOAD_FILE_NAME + ".msq";
        if (PanamaClient.uploadFile(panamaUrl,
            new File(calibrationsToUploadFileName),
            instanceName,
            mcuSerial
        )) {
            callbacks.logLine(String.format(
                "File `%s` is successfully uploaded to `%s` for `%s` (MCUSERIAL=%d)",
                calibrationsToUploadFileName,
                panamaUrl,
                instanceName,
                mcuSerial
            ));
            result = true;
        } else {
            callbacks.logLine(String.format(
                "Failed to upload file `%s` to `%s` for `%s` (MCUSERIAL=%d)",
                calibrationsToUploadFileName,
                panamaUrl,
                instanceName,
                mcuSerial
            ));
        }
        return result;
    }
}
