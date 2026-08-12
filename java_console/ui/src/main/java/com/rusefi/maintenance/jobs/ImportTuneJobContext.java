package com.rusefi.maintenance.jobs;

import com.rusefi.tune.xml.Msq;

public class ImportTuneJobContext {
    private final Msq tuneToImport;

    public ImportTuneJobContext(final Msq tuneToImport) {
        this.tuneToImport = tuneToImport;
    }

    Msq getTuneToImport() {
        return tuneToImport;
    }
}
