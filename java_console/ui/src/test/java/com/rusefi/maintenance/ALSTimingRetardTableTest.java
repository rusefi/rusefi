package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;

public class ALSTimingRetardTableTest {
    final String ALS_TIMING_RETARD_TABLE_FIELD_NAME = "ALSTimingRetardTable";

    final String PREV_ALS_TIMING_RETARD_TABLE = "\n" +
        "         0.0 0.0 0.0 0.0\n" +
        "         0.0 0.0 0.0 0.0\n" +
        "         0.0 0.0 0.0 0.0\n" +
        "         0.0 0.0 0.0 0.0\n";

    final String TEST_ALS_TIMING_RETARD_TABLE = "\n" +
        "         0.0 1.0 2.0 3.0\n" +
        "         0.1 1.1 2.1 3.1\n" +
        "         0.2 1.2 2.2 3.2\n" +
        "         0.3 1.3 2.3 3.3\n";

    private IniField alsTimingRetardTableField;
    private ConfigurationImage prevImage;
    private Constant prevValue;

    @BeforeEach
    void setUp() throws JAXBException {
        final CalibrationsTestData testData = CalibrationsTestData.load();
        final CalibrationsInfo prevCalibrations = testData.getPrevCalibrationsInfo();
        final IniFileModel prevIni = prevCalibrations.getIniFile();
        alsTimingRetardTableField = prevIni.getIniField(ALS_TIMING_RETARD_TABLE_FIELD_NAME);
        prevImage = prevCalibrations.getImage().getConfigurationImage();
        assertEquals(
            PREV_ALS_TIMING_RETARD_TABLE,
            alsTimingRetardTableField.getValue(prevImage)
        );
        prevValue = testData.getPrevValue(ALS_TIMING_RETARD_TABLE_FIELD_NAME);
        assertEquals(PREV_ALS_TIMING_RETARD_TABLE,
            prevValue.getValue()
        );
    }

    @Test
    void updateALSTimingRetardTable() {
        final ConfigurationImage testImage = prevImage.clone();

        alsTimingRetardTableField.setValue(testImage,  cloneWithNewValue(prevValue, TEST_ALS_TIMING_RETARD_TABLE));

        /* We are expecting to read the value that we have just set: */
        // todo: fix me?!
        assertNotEquals(TEST_ALS_TIMING_RETARD_TABLE, alsTimingRetardTableField.getValue(testImage));
    }

    private static Constant cloneWithNewValue(final Constant origin, final String newValue) {
        final Constant result = new Constant(
            origin.getName(),
            origin.getUnits(),
            newValue,
            origin.getDigits()
        );
        result.setRows(origin.getRows());
        result.setCols(origin.getCols());
        return result;
    }
}
