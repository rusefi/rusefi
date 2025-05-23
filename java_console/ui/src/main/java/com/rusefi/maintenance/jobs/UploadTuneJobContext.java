package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;

public class UploadTuneJobContext extends SerialPortJobContext<SerialPortScanner.PortResult> {
    private final String panamaUrl;

    UploadTuneJobContext(final SerialPortScanner.PortResult port, final String panamaUrl) {
        super(port);
        this.panamaUrl = panamaUrl;
    }

    public String getPanamaUrl() {
        return panamaUrl;
    }
}
