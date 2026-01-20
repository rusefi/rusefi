package com.opensr5.ini.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class ScalarIniFieldTest {
    private static final String TEST_FIELD = "TEST_FIELD";
    private static final String TEST_UNIT = "TEST_UNIT";
    private static final String TEST_DIGITS = "TEST_DIGITS";
    private static final FieldType TEST_TYPE = FieldType.INT16;
    private static final double TEST_MULTIPLIER = 0.1;
    private static final String TEST_VALUE = "1331.3";

    @Test
    public void updateShort() {
        ScalarIniField testField = new ScalarIniField(TEST_FIELD, 0, TEST_UNIT, TEST_TYPE, TEST_MULTIPLIER, TEST_DIGITS, 0);
        ConfigurationImage image = new ConfigurationImage(2);
        ConfigurationImageGetterSetter.setValue(testField, image, new Constant(TEST_FIELD, TEST_UNIT, TEST_VALUE, TEST_DIGITS));
        assertEquals(TEST_VALUE, ConfigurationImageGetterSetter.getValue(testField, image));
    }
}
