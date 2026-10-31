package com.rusefi.maintenance;

import com.opensr5.ini.IniFileModel;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class CalibrationsHelperRollbackPolicyTest {
    @Test
    void sameLayoutHashCanRestoreAcrossBuildDates() {
        assertTrue(CalibrationsHelper.rollbackLayoutsMatch(
            calibrations("rusEFI lts-26.2026.07.24.uaefi.123"),
            calibrations("rusEFI lts-26.2026.07.23.uaefi.123")));
    }

    @Test
    void changedLayoutHashRequiresDefaults() {
        assertFalse(CalibrationsHelper.rollbackLayoutsMatch(
            calibrations("rusEFI lts-26.2026.07.24.uaefi.123"),
            calibrations("rusEFI lts-26.2026.07.23.uaefi.456")));
    }

    @Test
    void unparseableSignaturesMustMatchExactly() {
        assertTrue(CalibrationsHelper.rollbackLayoutsMatch(calibrations("test"), calibrations("test")));
        assertFalse(CalibrationsHelper.rollbackLayoutsMatch(calibrations("test"), calibrations("other")));
    }

    private static CalibrationsInfo calibrations(String signature) {
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.getSignature()).thenReturn(signature);
        CalibrationsInfo calibrations = mock(CalibrationsInfo.class);
        when(calibrations.getIniFile()).thenReturn(ini);
        return calibrations;
    }
}
