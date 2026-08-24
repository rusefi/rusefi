package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class GenericFieldsPanelTest {

    private static final List<String> VIN_FIELDS = Collections.singletonList("vinNumber");
    private static final int VIN_OFFSET = 0;
    private static final int VIN_SIZE = 17;

    @Test
    public void emptyStringFieldNeedsAttention() {
        ConfigurationImage image = new ConfigurationImage(new byte[32]);
        // All zero bytes → getStringValue returns ""
        assertTrue(GenericFieldsPanel.anyFieldEmpty(iniWithVin(), image, VIN_FIELDS));
    }

    @Test
    public void populatedStringFieldDoesNotNeedAttention() {
        ConfigurationImage image = new ConfigurationImage(new byte[32]);
        writeString(image, "ABCDEFGHJKLMN1234", VIN_OFFSET);
        assertFalse(GenericFieldsPanel.anyFieldEmpty(iniWithVin(), image, VIN_FIELDS));
    }

    @Test
    public void whitespaceOnlyStringNeedsAttention() {
        ConfigurationImage image = new ConfigurationImage(new byte[32]);
        writeString(image, "                 ", VIN_OFFSET);
        assertTrue(GenericFieldsPanel.anyFieldEmpty(iniWithVin(), image, VIN_FIELDS));
    }

    @Test
    public void invalidSentinelEnumNeedsAttention() {
        // Enum with INVALID at ordinal 0 — image bytes all zero → stored ordinal 0 → "INVALID"
        IniFileModel ini = iniWithEnum("mapSensorType",
            "INVALID", "MPX4250", "MPX4100", "honda3bar");
        ConfigurationImage image = new ConfigurationImage(new byte[4]);
        assertTrue(GenericFieldsPanel.anyFieldEmpty(ini, image,
            Collections.singletonList("mapSensorType")));
    }

    @Test
    public void validEnumValueDoesNotNeedAttention() {
        IniFileModel ini = iniWithEnum("mapSensorType",
            "INVALID", "MPX4250", "MPX4100", "honda3bar");
        ConfigurationImage image = new ConfigurationImage(new byte[4]);
        // Write ordinal 2 (MPX4100) as a little-endian byte
        image.getContent()[0] = 2;
        assertFalse(GenericFieldsPanel.anyFieldEmpty(ini, image,
            Collections.singletonList("mapSensorType")));
    }

    @Test
    public void missingFieldIsSkippedAndDoesNotCrash() {
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField("nonexistent")).thenReturn(Optional.empty());
        ConfigurationImage image = new ConfigurationImage(new byte[32]);
        // No fields are present → no field reports "empty" → returns false (no NPE)
        assertFalse(GenericFieldsPanel.anyFieldEmpty(ini, image,
            Collections.singletonList("nonexistent")));
    }

    @Test
    public void multipleFieldsReturnsTrueIfAnyIsEmpty() {
        IniFileModel ini = mock(IniFileModel.class);
        StringIniField a = new StringIniField("a", 0, 4);
        StringIniField b = new StringIniField("b", 4, 4);
        when(ini.findIniField("a")).thenReturn(Optional.of(a));
        when(ini.findIniField("b")).thenReturn(Optional.of(b));

        ConfigurationImage image = new ConfigurationImage(new byte[8]);
        writeString(image, "OKAY", 0);
        // "b" region stays zeroed
        assertTrue(GenericFieldsPanel.anyFieldEmpty(ini, image, Arrays.asList("a", "b")));
    }

    // ---- helpers ----

    private IniFileModel iniWithVin() {
        IniFileModel ini = mock(IniFileModel.class);
        StringIniField vin = new StringIniField("vinNumber", VIN_OFFSET, VIN_SIZE);
        when(ini.findIniField("vinNumber")).thenReturn(Optional.of(vin));
        return ini;
    }

    private IniFileModel iniWithEnum(String name, String... values) {
        Map<Integer, String> map = new HashMap<>();
        for (int i = 0; i < values.length; i++) map.put(i, values[i]);
        EnumIniField.EnumKeyValueMap enumMap = new EnumIniField.EnumKeyValueMap(map);
        EnumIniField field = new EnumIniField(name, 0, FieldType.INT8, enumMap, 0, 1);

        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField(name)).thenReturn(Optional.of(field));
        return ini;
    }

    private void writeString(ConfigurationImage image, String value, int offset) {
        byte[] bytes = value.getBytes();
        System.arraycopy(bytes, 0, image.getContent(), offset, bytes.length);
    }
}
