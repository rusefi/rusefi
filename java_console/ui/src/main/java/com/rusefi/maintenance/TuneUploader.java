package com.rusefi.maintenance;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocolLocalCache;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.panama.PanamaClient;
import com.rusefi.panama.PanamaHelper;
import com.rusefi.ui.basic.InstanceNameEditor;

import java.io.File;
import java.util.Optional;

import static com.rusefi.maintenance.CalibrationsHelper.readAndBackupCurrentCalibrationsWithSuspendedPortScanner;

public enum TuneUploader {
    INSTANCE;

    private static final String CALIBRATIONS_TO_UPLOAD_FILE_NAME = BinaryProtocolLocalCache.STATE_FOLDER + "calibrations_to_upload";

    /**
     * @return true in case of success, false otherwise
     */
    public synchronized boolean uploadTune(
        final PortResult ecuPort,
        final String panamaUrl,
        final UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext
    ) {
        boolean result = false;

        final String instanceName = InstanceNameEditor.loadInstanceName();
        if (instanceName == null || instanceName.isEmpty()) {
            callbacks.logLine("Instance name is not defined!");
            callbacks.logLine("Please right-click on logo and use `Instance name` context menu  to specify an instance name.");
            return false;
        }

        final Optional<CalibrationsInfo> calibrationsToUpload = readAndBackupCurrentCalibrationsWithSuspendedPortScanner(
            ecuPort.port,
            callbacks,
            CALIBRATIONS_TO_UPLOAD_FILE_NAME, connectivityContext
        );
        if (!calibrationsToUpload.isPresent()) {
            callbacks.logLine("Failed to back up current calibrations...");
            return false;
        }

        final Optional<Integer> receivedMcuSerial = OutputChannelsHelper.readMcuSerial(ecuPort, callbacks, connectivityContext);
        if (!receivedMcuSerial.isPresent()) {
            callbacks.logLine("Failed to read " + PanamaHelper.MCUSERIAL + " output channel - please update firmware first!");
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
                "File `%s` is successfully uploaded to `%s` for `%s` (" + PanamaHelper.MCUSERIAL + "=%d)",
                calibrationsToUploadFileName,
                panamaUrl,
                instanceName,
                mcuSerial
            ));
            result = true;
        } else {
            callbacks.logLine(String.format(
                "Failed to upload file `%s` to `%s` for `%s` (" + PanamaHelper.MCUSERIAL + "=%d)",
                calibrationsToUploadFileName,
                panamaUrl,
                instanceName,
                mcuSerial
            ));
        }
        return result;
    }
}
