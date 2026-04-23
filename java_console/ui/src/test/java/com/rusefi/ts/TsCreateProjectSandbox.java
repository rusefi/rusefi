package com.rusefi.ts;

import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;

import java.util.Optional;

/**
 * Stand-alone dev harness that scans for an ECU, reads its calibrations, and creates a
 * TunerStudio project on disk. The real implementation lives in
 * {@link TsProjectCreator} + {@link DashboardFactory}; this class only wires up the
 * ECU-discovery + calibrations-read that the console does via its live connection.
 *
 * For the in-console auto-create path see the {@code auto_create_project} property in
 * {@code shared_io.properties} and the hook in {@code StartupFrame.onSplashConnected}.
 */
public class TsCreateProjectSandbox {
    public static void main(String[] args) throws Exception {
        SerialAutoChecker.AutoDetectResult detect = PortDetector.autoDetectSerial(null);
        if (detect.getSerialPort() == null || detect.getSignature() == null) {
            System.err.println("No ECU found");
            System.exit(1);
        }
        String port = detect.getSerialPort();
        System.out.println("Detected ECU at " + port + " signature=" + detect.getSignature());

        Optional<CalibrationsInfo> maybeCalibrations =
            CalibrationsHelper.readCurrentCalibrationsWithoutSuspendingPortScanner(
                port, UpdateOperationCallbacks.LOGGER);
        if (!maybeCalibrations.isPresent()) {
            System.err.println("Failed to read calibrations from " + port);
            System.exit(2);
        }

        String projectName = args.length > 0 ? args[0] : "rusEFI";
        TsProjectCreator.create(projectName, port, maybeCalibrations.get());

        System.out.println("Created TS project " + projectName);
    }
}
