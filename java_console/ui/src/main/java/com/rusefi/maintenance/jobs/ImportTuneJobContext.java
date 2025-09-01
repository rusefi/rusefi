package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.tune.xml.Msq;

public class ImportTuneJobContext extends SerialPortJobContext {
    private final Msq tuneToImport;

    public ImportTuneJobContext(final PortResult port, final Msq tuneToImport) {
        super(port);
        this.tuneToImport = tuneToImport;
    }

    Msq getTuneToImport() {
        return tuneToImport;
    }
}
