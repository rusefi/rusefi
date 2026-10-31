package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.maintenance.CalibrationsInfo;

public class UpdateCalibrationsJobContext extends SerialPortJobContext {
    private final CalibrationsInfo calibrations;

    UpdateCalibrationsJobContext(final PortResult port, final CalibrationsInfo calibrations) {
        super(port);
        this.calibrations = calibrations;
    }

    public CalibrationsInfo getCalibrations() {
        return calibrations;
    }
}
