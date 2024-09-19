package com.rusefi.maintenance;

import java.util.Optional;

public enum JobType {
    DFU_AUTO("DFU update", "Auto DFU Update"),
    DFU_MANUAL("DFU update", "Manual DFU Update"),
    INSTALL_OPENBLT("OpenBLT Initial Programming"/*, "Install OpenBLT"*/),
    ST_LINK(""/*, "ST-LINK Update"*/),
    DFU_SWITCH("DFU switch", "Switch to DFU Mode"),
    OPENBLT_SWITCH("OpenBLT switch", "Switch to OpenBLT Mode"),
    OPENBLT_CAN("OpenBLT via CAN", "OpenBLT via CAN"),
    OPENBLT_MANUAL("OpenBLT via Serial", "Manual OpenBLT Update"),
    OPENBLT_AUTO("OpenBLT via Serial", "Auto OpenBLT Update"),
    DFU_ERASE("DFU erase", "Full DFU Erase"),
    UPDATE_CALIBRATIONS("Update calibrations");

    final String jobName;
    final String persistedValue;

    JobType(final String jobName, final String persistedValue) {
        this.jobName = jobName;
        this.persistedValue = persistedValue;
    }

    JobType(final String jobName) {
        this.jobName = jobName;
        this.persistedValue = null;
    }

    static Optional<JobType> parsePersistedValue(final String persistedValue) {
        for (final JobType value: values()) {
            final String valuePersistedValue = value.persistedValue;
            if ((valuePersistedValue != null) && valuePersistedValue.equals(persistedValue)) {
                return Optional.of(value);
            }
        }
        return Optional.empty();
    }
}
