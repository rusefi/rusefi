package com.rusefi.ui.basic;

import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.maintenance.CalibrationsInfo;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class UpdateCalibrationsSignatureTest {
    @Test
    void acceptsLiveAndLegacyIniBackupsButRejectsOtherFirmware() {
        String live = "rusEFI lts-25kansas.2026.06.27.proteus_f7.12345";
        String iniSignature = "rusEFI lts-25kansas.2026.06.24.proteus_f7.12345";
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.getSignature()).thenReturn(iniSignature);
        CalibrationsInfo current = new CalibrationsInfo(ini,
            new ConfigurationImageWithMeta(new ConfigurationImageMetaVersion0_0(16, live + "\u0000"), new byte[16]));

        assertTrue(UpdateCalibrations.matchesCalibrationSignature(live, current));
        assertTrue(UpdateCalibrations.matchesCalibrationSignature(live + "\u0000", current));
        assertTrue(UpdateCalibrations.matchesCalibrationSignature(iniSignature, current));
        assertFalse(UpdateCalibrations.matchesCalibrationSignature(live.replace("12345", "98765"), current));
        assertFalse(UpdateCalibrations.matchesCalibrationSignature(live.replace("proteus_f7", "proteus_f4"), current));
        assertFalse(UpdateCalibrations.matchesCalibrationSignature(null, current));
        assertFalse(UpdateCalibrations.matchesCalibrationSignature("", current));
    }
}
