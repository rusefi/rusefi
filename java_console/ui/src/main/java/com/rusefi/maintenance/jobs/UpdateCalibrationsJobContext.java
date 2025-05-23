package com.rusefi.maintenance.jobs;

import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.PortResult;

public class UpdateCalibrationsJobContext extends SerialPortJobContext {
    private final ConfigurationImageWithMeta calibrations;

    UpdateCalibrationsJobContext(final PortResult port, final ConfigurationImageWithMeta calibrations) {
        super(port);
        this.calibrations = calibrations;
    }

    public ConfigurationImageWithMeta getCalibrations() {
        return calibrations;
    }
}
