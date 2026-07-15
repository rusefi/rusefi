package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class AutoupdatePolicyTest {
    @Test
    void requiresUserAndBundleFlags() {
        assertTrue(Autoupdate.isAutoUpdateEnabled(true, true));
        assertFalse(Autoupdate.isAutoUpdateEnabled(true, false));
        assertFalse(Autoupdate.isAutoUpdateEnabled(false, true));
        assertFalse(Autoupdate.isAutoUpdateEnabled(false, false));
    }
}
