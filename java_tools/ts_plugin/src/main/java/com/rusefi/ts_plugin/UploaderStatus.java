package com.rusefi.ts_plugin;

import com.rusefi.TsTuneReader;
import com.rusefi.ts_plugin.ui.TuneUploder;
import com.rusefi.tune.xml.Constant;

import java.io.File;
import java.util.Map;

public class UploaderStatus {
    public static final String NO_PROJECT = "Please open project";
    public String projectWarning;
    public String tuneInfo;
    public String tuneWarning;

    public void updateProjectStatus(String configurationName, boolean isProjectActive) {
        if (!isProjectActive) {
            this.projectWarning = NO_PROJECT;
        } else if (!new File(TsTuneReader.getTsTuneFileName(configurationName)).exists()) {
            this.projectWarning = "Tune not found " + configurationName;
        } else {
            this.projectWarning = null;
        }
    }

    public void readTuneState(String configurationName) {
        Map<String, Constant> fileSystemValues = TuneUploder.getFileSystemValues(configurationName);
        Constant engineMake = fileSystemValues.get("enginemake");
        Constant engineCode = fileSystemValues.get("enginecode");
        Constant vehicleName = fileSystemValues.get("VEHICLENAME");
        String warning = "";
        if (TsPluginUiImpl.isEmpty(engineMake)) {
            warning += " engine make";
        }
        if (TsPluginUiImpl.isEmpty(engineCode)) {
            warning += " engine code";
        }
        if (TsPluginUiImpl.isEmpty(vehicleName)) {
            warning += " vehicle name";
        }
        if (warning.isEmpty()) {
            tuneInfo = engineMake.getValue() + " " + engineCode.getValue() + " " + vehicleName.getValue();
            tuneWarning = null;
        } else {
            tuneInfo = null;
            tuneWarning = "<html>Please set " + warning + " on Base Settings tab<br>and reopen Project";
        }
    }

    public boolean isProjectIsOk() {
        return projectWarning == null;
    }

    public boolean isTuneOk() {
        return tuneWarning == null;
    }
}
