package com.rusefi.maintenance;

import java.util.Optional;

public enum JobType {
    DFU_AUTO("DFU update", "Auto DFU Update"),
    DFU_MANUAL("DFU update", "Manual DFU Update"),
    INSTALL_OPENBLT("OpenBLT Initial Programming", "Install OpenBLT"),
    ST_LINK("", "ST-LINK Update"),
    DFU_SWITCH("DFU switch", "Switch to DFU Mode"),
    OPENBLT_SWITCH("OpenBLT switch", "Switch to OpenBLT Mode"),
    OPENBLT_CAN("OpenBLT via CAN", "OpenBLT via CAN"),
    OPENBLT_MANUAL("OpenBLT via Serial", "Manual OpenBLT Update"),
    OPENBLT_AUTO("OpenBLT via Serial", "Auto OpenBLT Update"),
    DFU_ERASE("DFU erase", "Full DFU Erase"),
    UPDATE_CALIBRATIONS("Update calibrations");

    public final String jobName;
    /***
     * Text to display in the corresponding combo box item
     */
    final String displayText;

    JobType(final String jobName, final String displayText) {
        this.jobName = jobName;
        this.displayText = displayText;
    }

    JobType(final String jobName) {
        this.jobName = jobName;
        this.displayText = null;
    }

    static Optional<JobType> parsePersistedValue(final String persistedValue) {
        for (final JobType value: values()) {
            final String valuePersistedValue = value.displayText;
            if ((valuePersistedValue != null) && valuePersistedValue.equals(persistedValue)) {
                return Optional.of(value);
            }
        }
        return Optional.empty();
    }

    @Override
    public String toString() {
        return displayText;
    }
}
