package com.rusefi.ui.console;

import com.rusefi.core.RusEfiSignature;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class MainFrameUpdateCheckTest {

    // ECU signature: rusEFI development.2026.05.09.uaefi_pro.4226383888
    private static final RusEfiSignature ECU_SIG = new RusEfiSignature(
        "development", "2026", "05", "09", "uaefi_pro", "4226383888", false
    );

    @Test
    public void nullEcuSigReturnsFalse() {
        assertFalse(MainFrame.needsFirmwareUpdate(null, "rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32_update.srec"));
    }

    @Test
    public void nullSrecNameReturnsFalse() {
        assertFalse(MainFrame.needsFirmwareUpdate(ECU_SIG, null));
    }

    @Test
    public void unparseableSrecNameReturnsFalse() {
        assertFalse(MainFrame.needsFirmwareUpdate(ECU_SIG, "garbage.srec"));
    }

    @Test
    public void newFormatSameHashReturnsFalse() {
        // SREC numeric hash matches ECU numeric hash → no update needed
        String srec = "rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertFalse(MainFrame.needsFirmwareUpdate(ECU_SIG, srec));
    }

    @Test
    public void newFormatDifferentHashReturnsTrue() {
        // SREC numeric hash differs from ECU numeric hash → update needed
        String srec = "rusefi_development_2026-05-09_uaefi_pro_9999999999_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertTrue(MainFrame.needsFirmwareUpdate(ECU_SIG, srec));
    }

    @Test
    public void newFormatSameDateDifferentHashReturnsTrue() {
        // Even if dates match, different numeric hash means different build → update needed
        String srec = "rusefi_development_2026-05-09_uaefi_pro_1111111111_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertTrue(MainFrame.needsFirmwareUpdate(ECU_SIG, srec));
    }

    @Test
    public void legacyFormatSameDateReturnsFalse() {
        // Legacy 4-part srec, same date as ECU → no update needed
        RusEfiSignature ecuSig = new RusEfiSignature(
            "development", "2026", "05", "09", "all", null, false
        );
        String srec = "rusefi_development_2026-05-09_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertFalse(MainFrame.needsFirmwareUpdate(ecuSig, srec));
    }

    @Test
    public void legacyFormatDifferentDateReturnsTrue() {
        // Legacy 4-part srec, date differs from ECU → update needed
        RusEfiSignature ecuSig = new RusEfiSignature(
            "development", "2026", "05", "09", "all", null, false
        );
        String srec = "rusefi_development_2026-05-10_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertTrue(MainFrame.needsFirmwareUpdate(ecuSig, srec));
    }

    @Test
    public void legacyFormatDifferentMonthReturnsTrue() {
        RusEfiSignature ecuSig = new RusEfiSignature(
            "development", "2026", "04", "09", "all", null, false
        );
        String srec = "rusefi_development_2026-05-09_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertTrue(MainFrame.needsFirmwareUpdate(ecuSig, srec));
    }

    @Test
    public void newFormatWithoutUpdateSuffix() {
        // SREC name without _update suffix should still parse correctly
        String srec = "rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32.srec";
        assertFalse(MainFrame.needsFirmwareUpdate(ECU_SIG, srec));
    }
}
