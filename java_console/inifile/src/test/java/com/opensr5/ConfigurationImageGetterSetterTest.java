package com.opensr5;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.OrdinalOutOfRangeException;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import java.util.TreeMap;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class ConfigurationImageGetterSetterTest {
    /**
     * Reproduces the issue where a 1-byte enum field at the very end of the configuration image
     * causes an ArrayOutOfBoundsException because getByteBuffer() always reads 4 bytes.
     */
    @Test
    public void testEnumIniFieldGetValueLastByte() {
        // Create a small image where a U08 enum sits at the last byte
        byte[] data = new byte[]{0x00, 0x02};
        ConfigurationImage ci = new ConfigurationImage(data);

        TreeMap<Integer, String> map = new TreeMap<>();
        map.put(0, "Off");
        map.put(1, "On");
        map.put(2, "Auto");
        EnumIniField.EnumKeyValueMap enumMap = new EnumIniField.EnumKeyValueMap(map);
        // Enum field at offset 1 (last byte), type U08, bit range [0:7]
        EnumIniField field = new EnumIniField("communityCommsLedPin", 1, FieldType.UINT8, enumMap, 0, 7);

        // This should NOT throw ArrayOutOfBoundsException
        String value = ConfigurationImageGetterSetter.getStringValue(field, ci);
        assertEquals("\"Auto\"", value);
    }
    /**
     * Sparse key-value enum lists (pin dropdowns) omit INVALID placeholder entries,
     * see PinoutLogic.enumToOptionsList()
     */
    @Test
    public void testEnumIniFieldSparseKeyValueMap() {
        TreeMap<Integer, String> map = new TreeMap<>();
        map.put(0, "NONE");
        map.put(5, "Injector 1");
        map.put(47, "Coil 1");
        EnumIniField.EnumKeyValueMap enumMap = new EnumIniField.EnumKeyValueMap(map);

        // ordinal present in the map
        EnumIniField field = new EnumIniField("injectionPins1", 0, FieldType.UINT8, enumMap, 0, 7);
        assertEquals("\"Coil 1\"", ConfigurationImageGetterSetter.getStringValue(field, new ConfigurationImage(new byte[]{47, 0})));

        // ordinal in a gap: pin exists in the tune but is not exposed by this board's ini
        assertEquals("\"INVALID\"", ConfigurationImageGetterSetter.getStringValue(field, new ConfigurationImage(new byte[]{33, 0})));

        // ordinal beyond the highest known entry
        assertThrows(OrdinalOutOfRangeException.class,
            () -> ConfigurationImageGetterSetter.getStringValue(field, new ConfigurationImage(new byte[]{48, 0})));
    }

    @Test
    public void testLegacyEnumSuffixCompatibilityRejectsUnsafeMatches() {
        TreeMap<Integer, String> map = new TreeMap<>();
        map.put(1, "Output (variant A)");
        map.put(2, "Output (variant B)");
        EnumIniField field = new EnumIniField("output", 0, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(map), 0, 7, true);
        ConfigurationImage image = new ConfigurationImage(new byte[4]);

        assertThrows(IllegalArgumentException.class,
            () -> ConfigurationImageGetterSetter.setValue2(field, image, "output", "\"Output\""));
        assertThrows(IllegalArgumentException.class,
            () -> ConfigurationImageGetterSetter.setValue2(field, image, "output", "\"Different output\""));

        TreeMap<Integer, String> unrecognizedSuffixMap = new TreeMap<>();
        unrecognizedSuffixMap.put(1, "OutputExtended");
        EnumIniField unrecognizedSuffixField = new EnumIniField("output", 0, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(unrecognizedSuffixMap), 0, 7, true);
        assertThrows(IllegalArgumentException.class,
            () -> ConfigurationImageGetterSetter.setValue2(
                unrecognizedSuffixField, image, "output", "\"Output\""));
    }

    @Test
    public void testLegacyPinEnumAcceptsKnownAdditiveSuffixes() {
        String[] currentLabels = {
            "Output (documentation)",
            "Output or alternate function",
            "Output for low count wheel",
            "Output best for normal wheels",
            "Output, auxiliary use",
            "Output legacy connector A",
        };
        for (String currentLabel : currentLabels) {
            TreeMap<Integer, String> map = new TreeMap<>();
            map.put(3, currentLabel);
            EnumIniField field = new EnumIniField("output", 0, FieldType.UINT8,
                new EnumIniField.EnumKeyValueMap(map), 0, 7, true);
            ConfigurationImage image = new ConfigurationImage(new byte[4]);

            ConfigurationImageGetterSetter.setValue2(field, image, "output", "\"Output\"");
            assertEquals("\"" + currentLabel + "\"",
                ConfigurationImageGetterSetter.getStringValue(field, image));
        }

        TreeMap<Integer, String> ordinaryEnumMap = new TreeMap<>();
        ordinaryEnumMap.put(3, "Output or alternate function");
        EnumIniField ordinaryEnum = new EnumIniField("mode", 0, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(ordinaryEnumMap), 0, 7);
        assertThrows(IllegalArgumentException.class,
            () -> ConfigurationImageGetterSetter.setValue2(
                ordinaryEnum, new ConfigurationImage(new byte[4]), "mode", "\"Output\""));
    }

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
