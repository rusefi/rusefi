package com.opensr5.ini.test;

import com.rusefi.ini.reader.EnumIniReaderHelper;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.condition.EnabledOnJre;
import org.junit.jupiter.api.condition.JRE;

import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertFalse;

@EnabledOnJre(JRE.JAVA_8)
public class Java8EnumInitializationTest {
    @Test
    public void initializesEnumReader() {
        // Initializing this class previously crashed TunerStudio's connection thread.
        assertTrue(EnumIniReaderHelper.isKeyValueSyntax(
            "engineType = bits, S32, 0, [0:6], 0=\"DEFAULT\",1=\"TEST\""));
        assertFalse(EnumIniReaderHelper.isKeyValueSyntax(
            "iat_adcChannel = bits, U08, 312, [0:7], $adc_channel_e_list"));
    }
}
