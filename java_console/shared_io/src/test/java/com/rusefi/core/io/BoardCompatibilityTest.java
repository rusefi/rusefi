package com.rusefi.core.io;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class BoardCompatibilityTest {

    @Test
    public void exactMatchAlwaysCompatible() {
        // even with no board_compatibility, exact target matches (legacy behavior)
        assertTrue(BoardCompatibility.isEcuCompatible("uaefi", "uaefi", null));
        assertTrue(BoardCompatibility.isEcuCompatible("UAEFI", "uaefi", null));
    }

    @Test
    public void mismatchWithoutCompatibilityIsRejected() {
        assertFalse(BoardCompatibility.isEcuCompatible("uaefi", "uaefi_pro", null));
        assertFalse(BoardCompatibility.isEcuCompatible("uaefi", "uaefi_pro", ""));
    }

    @Test
    public void qcFirmwareBypass() {
        // QC firmware self-identifies as normal firmware [tag:QC_firmware]
        assertTrue(BoardCompatibility.isEcuCompatible("uaefi_QC_special", "uaefi_pro", null));
    }

    @Test
    public void wildcardMatchesAnything() {
        assertTrue(BoardCompatibility.isEcuCompatible("universal", "uaefi", "*"));
        assertTrue(BoardCompatibility.isEcuCompatible("universal", "super-uaefi", "*"));
        assertTrue(BoardCompatibility.isEcuCompatible(null, "anything", "*"));
    }

    @Test
    public void allowlistMatching() {
        String list = "uaefi, uaefi_pro , super-uaefi";
        assertTrue(BoardCompatibility.isEcuCompatible("universal", "uaefi_pro", list));
        assertTrue(BoardCompatibility.isEcuCompatible("universal", "SUPER-UAEFI", list));
        assertFalse(BoardCompatibility.isEcuCompatible("universal", "hellen121nissan", list));
    }

    @Test
    public void nullEcuTargetIsTolerant() {
        assertTrue(BoardCompatibility.isEcuCompatible("uaefi", null, null));
    }
}
