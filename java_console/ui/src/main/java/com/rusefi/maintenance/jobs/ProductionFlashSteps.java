package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.MaintenanceUtil;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class ProductionFlashSteps {
    public static final OpenBltManualJob.FlashSteps PRODUCTION_STEPS = new OpenBltManualJob.FlashSteps() {
        @Override
        public boolean ensureFirmware(final UpdateOperationCallbacks callbacks, final ConnectedEcuTarget target) {
            return MaintenanceUtil.ensureFirmwareForConnectedTarget(callbacks, target);
        }

        @Override
        public boolean flash(final JComponent parent, final String port, final UpdateOperationCallbacks callbacks,
                             final ConnectedEcuTarget target) {
            return ProgramSelector.flashOpenbltSerial(parent, port, callbacks, target);
        }

        @Override
        public void restoreCalibrations(final UpdateOperationCallbacks callbacks,
                                        final ConnectivityContext connectivityContext) {
            CalibrationsHelper.restorePreviousCalibrationsAfterManualFlash(callbacks, connectivityContext);
        }
    };
}
