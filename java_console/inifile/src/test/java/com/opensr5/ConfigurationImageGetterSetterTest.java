package com.opensr5;

import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class ConfigurationImageGetterSetterTest {
    @Test
    public void testGetStringValueWithPrecision() {
        ConfigurationImage ci = new ConfigurationImage(new byte[4]);
        // value 1.23456
        float value = 1.23456f;
        java.nio.ByteBuffer.wrap(ci.getContent()).order(java.nio.ByteOrder.LITTLE_ENDIAN).putFloat(value);

        {
            ScalarIniField field = new ScalarIniField("test", 0, "", FieldType.FLOAT, 1, "1", 0);
            String result = ConfigurationImageGetterSetter.getStringValue(field, ci);
            assertEquals("1.2", result);
        }

        {
            ScalarIniField field = new ScalarIniField("test", 0, "", FieldType.FLOAT, 1, "2", 0);
            String result = ConfigurationImageGetterSetter.getStringValue(field, ci);
            assertEquals("1.23", result);
        }

        {
            ScalarIniField field = new ScalarIniField("test", 0, "", FieldType.FLOAT, 1, "4", 0);
            String result = ConfigurationImageGetterSetter.getStringValue(field, ci);
            assertEquals("1.2346", result);
        }
    }
}
