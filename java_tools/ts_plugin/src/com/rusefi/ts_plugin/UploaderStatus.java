package com.rusefi.ts_plugin;

public class UploaderStatus {
    public String projectWarning;
    public String tuneInfo;
    public String tuneWarning;

    public boolean isProjectIsOk() {
        return projectWarning == null;
    }

    public boolean isTuneOk() {
        return tuneWarning == null;
    }
}
