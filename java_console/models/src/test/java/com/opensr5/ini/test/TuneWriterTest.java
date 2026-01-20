package com.opensr5.ini.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;


public class TuneWriterTest {
    @Test
    public void testFloatRounding() {
        ConfigurationImage image = new ConfigurationImage(200);
        IniField floatField = new ScalarIniField("test", 0, "test", FieldType.FLOAT, 1, "2", 0);
        double value = 0.9;
        ConfigurationImageGetterSetter.setValue(floatField, image, new Constant("x", "y", Double.toString(value), "2"));

        assertEquals("0.9", ConfigurationImageGetterSetter.getValue(floatField, image));
    }
}
