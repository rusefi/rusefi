package com.rusefi.maintenance;

import org.junit.jupiter.api.Test;

import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;

class CalibrationsHelperEmergencyWipeTest {
    @Test
    void discardsSessionCachedTune() {
        CalibrationsHelper.setLastEcuCalibrationsForUnitTest(Optional.of(mock(CalibrationsInfo.class)));
        assertTrue(CalibrationsHelper.hasLastEcuCalibrationsForUnitTest());

        CalibrationsHelper.discardLastEcuCalibrations();

        assertFalse(CalibrationsHelper.hasLastEcuCalibrationsForUnitTest());
    }
}
