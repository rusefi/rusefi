package com.opensr5.ini.test;

import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;

public class ScalarIniFieldTest {
    @Test
    public void testStorage() {
        ByteBuffer wrapped = ByteBuffer.allocate(2);
        FieldType type = FieldType.INT16;
        String value = "1331.3";
        double v = 0.1;
        ScalarIniField.setValue(wrapped, type, value, Field.NO_BIT_OFFSET, v);
        // todo
    }
}
