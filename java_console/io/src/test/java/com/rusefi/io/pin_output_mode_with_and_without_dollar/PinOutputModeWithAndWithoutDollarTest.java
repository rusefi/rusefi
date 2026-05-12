package com.rusefi.io.pin_output_mode_with_and_without_dollar;

import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.io.IOException;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class PinOutputModeWithAndWithoutDollarTest {
    private final static String TEST_DATA_FOLDER_PATH =
        "src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data";

    @Test
    void testNitrousRelayPinModeWithAndWithoutDollar() throws IOException {

        final String NITROUS_RELAY_PIN_MODE_FIELD_NAME = "nitrousRelayPinMode";
        final String EXPECTED_NITROUS_RELAY_PIN_MODE = "\"open collector inverted\"";

        final ConfigurationImageWithMeta testConfigurationImage = ConfigurationImageFile.readFromFile(
            String.format("%s/image.zip", TEST_DATA_FOLDER_PATH)
        );

        final CalibrationsInfo calibrationsInfoWithDollars = new CalibrationsInfo(
            IniFileReaderUtil.readIniFile(String.format("%s/rusefi_uaefi.ini", TEST_DATA_FOLDER_PATH)),
            testConfigurationImage
        );

        final Msq msqWithDollar = calibrationsInfoWithDollars.generateMsq();
        assertEquals(
            EXPECTED_NITROUS_RELAY_PIN_MODE,
            msqWithDollar.getConstantsAsMap().get(NITROUS_RELAY_PIN_MODE_FIELD_NAME).getValue()
        );

        final CalibrationsInfo calibrationsInfoWithoutDollars = new CalibrationsInfo(
            IniFileReaderUtil.readIniFile(String.format("%s/rusefi_uaefi_without_dollar.ini", TEST_DATA_FOLDER_PATH)),
            testConfigurationImage
        );
        final Msq msqWithoutDollar = calibrationsInfoWithoutDollars.generateMsq();
        assertEquals(
            EXPECTED_NITROUS_RELAY_PIN_MODE,
            msqWithoutDollar.getConstantsAsMap().get(NITROUS_RELAY_PIN_MODE_FIELD_NAME).getValue()
        );
    }
}
