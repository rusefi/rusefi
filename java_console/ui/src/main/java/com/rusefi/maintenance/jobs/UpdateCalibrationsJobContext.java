package com.rusefi.maintenance.jobs;

import com.opensr5.ConfigurationImage;
import com.rusefi.SerialPortScanner;

public class UpdateCalibrationsJobContext extends SerialPortJobContext {
    private final ConfigurationImage calibrations;

    UpdateCalibrationsJobContext(final SerialPortScanner.PortResult port, final ConfigurationImage calibrations) {
        super(port);
        this.calibrations = calibrations;
    }

    public ConfigurationImage getCalibrations() {
        return calibrations;
    }
}
