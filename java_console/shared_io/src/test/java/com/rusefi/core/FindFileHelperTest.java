package com.rusefi.core;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class FindFileHelperTest {
    /**
     * The target parsed here gates the firmware/board brick guard [tag:better_ux_for_flashing], so pin the real artifact
     * shapes: a two-token target, a one-token target, obfuscated (no numeric signature) and dev builds.
     */
    @Test
    public void extractsTargetFromArtifactName() {
        assertEquals("uaefi_pro", FindFileHelper.extractTargetFromFirmwareName(
            "/x/rusefi_development_2026-07-06_uaefi_pro_2528425206_7fb5500a13b00a3da2773de2d3749a4d8ecf03b6_update.srec"));
        assertEquals("uaefi", FindFileHelper.extractTargetFromFirmwareName(
            "rusefi_development_2026-07-01_uaefi_496885743_0d74c7f29351a14733ad1de1a72a35f99b1090c6_update.srec"));
        // No parseable target: obfuscated (no numeric signature) and plain dev builds.
        assertNull(FindFileHelper.extractTargetFromFirmwareName(
            "rusefi-development_2026-04-20_ae5ed0955f20e64271039ea6347f8d7f5561cc5f_obfuscated.srec"));
        assertNull(FindFileHelper.extractTargetFromFirmwareName("rusefi.srec"));
        assertNull(FindFileHelper.extractTargetFromFirmwareName(null));
    }
}
