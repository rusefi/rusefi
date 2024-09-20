package com.rusefi.maintenance;

import java.util.Optional;

public enum UpdateMode {
    DFU_AUTO("Auto DFU Update"),
    DFU_MANUAL("Manual DFU Update"),
    INSTALL_OPENBLT("Install OpenBLT"),
    ST_LINK("ST-LINK Update"),
    DFU_SWITCH("Switch to DFU Mode"),
    OPENBLT_SWITCH("Switch to OpenBLT Mode"),
    OPENBLT_CAN("OpenBLT via CAN"),
    OPENBLT_MANUAL("Manual OpenBLT Update"),
    OPENBLT_AUTO("Auto OpenBLT Update"),
    DFU_ERASE("Full DFU Erase");

    /***
     * Text to display in the corresponding combo box item
     */
    final String displayText;

    UpdateMode(final String displayText) {
        this.displayText = displayText;
    }

    static Optional<UpdateMode> parseDisplayText(final String persistedValue) {
        for (final UpdateMode value: values()) {
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
