package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;

public class ExportTuneJobContext extends SerialPortJobContext {
    private final String fileName;

    public ExportTuneJobContext(PortResult port, String fileName) {
        super(port);
        this.fileName = fileName;
    }

    public String getFileName() {
        return fileName;
    }
}
