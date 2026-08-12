package com.rusefi.maintenance.jobs;

public class ExportTuneJobContext {
    private final String fileName;

    public ExportTuneJobContext(String fileName) {
        this.fileName = fileName;
    }

    public String getFileName() {
        return fileName;
    }
}
