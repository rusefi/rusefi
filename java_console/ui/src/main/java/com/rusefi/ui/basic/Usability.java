package com.rusefi.ui.basic;

import com.opensr5.ini.field.IniField;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsInfo;

import java.util.Optional;

public enum Usability {
    INSTANCE;


    public void onCalibrations(UpdateOperationCallbacks updateOperationCallbacks, CalibrationsInfo calibrations) {
        if (!calibrations.getIniFile().getSignature().contains("hd81"))
            return;

        String ppsExpAverageAlphaName = "ppsExpAverageAlpha";

        Optional<IniField> iniField = calibrations.getIniFile().findIniField(ppsExpAverageAlphaName);
        if (!iniField.isPresent()) {
            updateOperationCallbacks.logLine(ppsExpAverageAlphaName + " not found: you want to update firmware!");
            updateOperationCallbacks.warning();
        }
    }
}
