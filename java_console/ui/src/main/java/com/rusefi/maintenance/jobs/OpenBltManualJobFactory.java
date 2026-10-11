package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.maintenance.OpenBltWipeArtifact;

import javax.swing.*;

public class OpenBltManualJobFactory {
    public static OpenBltManualJob createProduction(PortResult port, final JComponent parent, final ConnectivityContext connectivityContext) {
        return new OpenBltManualJob(port, parent, connectivityContext, ProductionFlashSteps.PRODUCTION_STEPS);
    }

    public static OpenBltManualJob createEmergencyWipe(PortResult port, final JComponent parent,
                                                        final ConnectivityContext connectivityContext,
                                                        final OpenBltWipeArtifact artifact) {
        return new OpenBltManualJob("Emergency OpenBLT wipe", port, parent, connectivityContext,
            ProductionFlashSteps.emergencyWipe(artifact));
    }
}
