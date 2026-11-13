package com.rusefi.core;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class SignatureHelperSrecTest {
    @Test
    void obfuscatedBundleFilenameCarriesFirmwareIdentity() {
        // firmware/bundle.mk names obfuscated artifacts differently from public update images.
        RusEfiSignature signature = SignatureHelper.parseSrec(
            "rusefi-lts-26lima_2026-11-11_test_board_123456_abcd_obfuscated.srec");
        assertNotNull(signature);
        assertEquals("lts-26lima", signature.getBranch());
        assertEquals("test_board", signature.getBundleTarget());
        assertEquals("123456", signature.getHash());
    }
}
