package com.rusefi.maintenance.jobs;

import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.SerialPortScanner;

public class UpdateCalibrationsJobContext extends SerialPortJobContext<SerialPortScanner.PortResult> {
    private final ConfigurationImageWithMeta calibrations;

    UpdateCalibrationsJobContext(final SerialPortScanner.PortResult port, final ConfigurationImageWithMeta calibrations) {
        super(port);
        this.calibrations = calibrations;
    }

    public ConfigurationImageWithMeta getCalibrations() {
        return calibrations;
    }
}
