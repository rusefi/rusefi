package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;

public class UploadTuneJobContext extends SerialPortJobContext {
    private final String panamaUrl;

    UploadTuneJobContext(final PortResult port, final String panamaUrl) {
        super(port);
        this.panamaUrl = panamaUrl;
    }

    public String getPanamaUrl() {
        return panamaUrl;
    }
}
