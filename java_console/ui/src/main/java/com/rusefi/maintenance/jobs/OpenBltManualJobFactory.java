package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;

import javax.swing.*;

public class OpenBltManualJobFactory {
    public static OpenBltManualJob createProduction(PortResult port, final JComponent parent, final ConnectivityContext connectivityContext) {
        return new OpenBltManualJob(port, parent, connectivityContext, ProductionFlashSteps.PRODUCTION_STEPS);
    }
}
