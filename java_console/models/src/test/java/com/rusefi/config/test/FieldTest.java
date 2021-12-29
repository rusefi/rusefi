package com.rusefi.config.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldCommandResponse;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Pair;
import org.junit.Test;

import static org.junit.Assert.*;

public class FieldTest {
    @Test
    public void testParse() {
        {
            assertNull(FieldCommandResponse.parseResponse("notint @768 is 21"));
        }
        {
            Pair<Integer, ?> p = FieldCommandResponse.parseResponse("int @768 is 21");
            assertNotNull(p);
            assertEquals(new Integer(768), p.first);
            assertEquals(21, p.second);
        }
        {
            Pair<Integer, ?> p = FieldCommandResponse.parseResponse("float @808 is 1.00");
            assertNotNull(p);
            assertEquals(new Integer(808), p.first);
            assertEquals(1.0, p.second);
        }
    }

    @Test
    public void setBooleanValue() {
        byte[] config = new byte[Fields.persistent_config_s_size];
        ConfigurationImage ci = new ConfigurationImage(config);

        assertFalse(Fields.ISFORCEDINDUCTION.getBooleanValue(ci));
        assertFalse(Fields.OVERRIDETRIGGERGAPS.getBooleanValue(ci));
        assertFalse(Fields.ENABLEFAN1WITHAC.getBooleanValue(ci));

        Fields.OVERRIDETRIGGERGAPS.setValue(config, true);

        assertFalse(Fields.ISFORCEDINDUCTION.getBooleanValue(ci));
        assertTrue(Fields.OVERRIDETRIGGERGAPS.getBooleanValue(ci));
        assertFalse(Fields.ENABLEFAN1WITHAC.getBooleanValue(ci));

        Fields.OVERRIDETRIGGERGAPS.setValue(config, false);

        assertFalse(Fields.ISFORCEDINDUCTION.getBooleanValue(ci));
        assertFalse(Fields.OVERRIDETRIGGERGAPS.getBooleanValue(ci));
        assertFalse(Fields.ENABLEFAN1WITHAC.getBooleanValue(ci));
    }

    @Test
    public void testPrecisionDependingOnScale() {
        assertEquals("0.12302", Field.niceToString(0.12302, 4));
        assertEquals("0.1232", Field.niceToString(0.12317, 3));

        assertEquals("1234567.1", Field.niceToString(1234567.123, 4));
        assertEquals("10000.0", Field.niceToString(10000.00002, 4));
        assertEquals("0.002", Field.niceToString(0.002, 4));
        assertEquals("12.302", Field.niceToString(12.302, 4));
        assertEquals("12.302", Field.niceToString(12.302, 3));
        assertEquals("12.31", Field.niceToString(12.312, 2));
        assertEquals("123.02", Field.niceToString(123.02, 4));
    }

}

