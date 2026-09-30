package com.rusefi.io.can.slcan;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** Shared 'V' reply rule for Lawicel, CANable 2 and WeAct USB2CANFDV1 adapters. */
class SlcanVersionTest {
    static final String WEACT = "WeAct Studio V1.0.0.3_bb264e71";
    static final String CANABLE = "16e7497-dirty github.com/normaldotcom/canable2.git";

    @Test
    void acceptsEveryKnownAdapterFamily() {
        assertTrue(SlcanVersion.isVersionReply("V1220"));
        assertTrue(SlcanVersion.isVersionReply("v1220".toUpperCase()));
        assertTrue(SlcanVersion.isVersionReply(CANABLE));
        assertTrue(SlcanVersion.isVersionReply("0123456789abcdef0123456789abcdef01234567 github.com/normaldotcom/canable2-fw"));
        assertTrue(SlcanVersion.isVersionReply(WEACT));
        assertTrue(SlcanVersion.isVersionReply("WeAct Studio V2.0"));
        assertTrue(SlcanVersion.isVersionReply("WeAct Studio V1.0.0.3_bb264e71-dirty"));
    }

    @Test
    void weActAndCanableAreTheUnacknowledgedFamily() {
        assertTrue(SlcanVersion.isCanableFamily(CANABLE));
        assertTrue(SlcanVersion.isCanableFamily(WEACT));
        assertFalse(SlcanVersion.isCanableFamily("V1220"));
    }

    @Test
    void rejectsPartialAndUnrelatedBanners() {
        assertFalse(SlcanVersion.isVersionReply(null));
        assertFalse(SlcanVersion.isVersionReply(""));
        assertFalse(SlcanVersion.isVersionReply("V"));
        assertFalse(SlcanVersion.isVersionReply("V122"));
        assertFalse(SlcanVersion.isVersionReply("V12200"));
        assertFalse(SlcanVersion.isVersionReply("Version of an unrelated serial device"));
        assertFalse(SlcanVersion.isVersionReply("rusEFI master"));
        assertFalse(SlcanVersion.isVersionReply("WeAct Studio"));
        assertFalse(SlcanVersion.isVersionReply("WeAct Studio V"));
        assertFalse(SlcanVersion.isVersionReply("WeAct Studio V1.0.0.3_xyz"));
        assertFalse(SlcanVersion.isVersionReply("xWeAct Studio V1.0.0.3_bb264e71"));
        assertFalse(SlcanVersion.isVersionReply("16e7497 github.com/someone/other.git"));
        assertFalse(SlcanVersion.isVersionReply("t7E840362F186"));
    }
}
