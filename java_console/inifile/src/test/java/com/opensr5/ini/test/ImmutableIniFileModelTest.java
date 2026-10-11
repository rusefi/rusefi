package com.opensr5.ini.test;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

/**
 * Tests for ImmutableIniFileModel
 */
public class ImmutableIniFileModelTest {

    @Test
    public void arrayElementLookupPreservesMetadataAndDeclaredFields() {
        IniFileModel model = readArrayFields();
        ArrayIniField array = (ArrayIniField) model.findIniField("voltages").get();
        for (int i = 0; i < 3; i++) {
            ScalarIniField element = (ScalarIniField) model.findIniField("VOLTAGES[" + i + "]").get();
            assertEquals("voltages[" + i + "]", element.getName());
            assertEquals(4 + 2 * i, element.getOffset());
            assertEquals(2, element.getSize());
            assertEquals(FieldType.UINT16, element.getType());
            assertEquals("V", element.getUnits());
            assertEquals("3", element.getDigits());
            assertEquals(0.001, element.getMultiplier());
            assertEquals(array.getPageIndex(), element.getPageIndex());
        }
        assertSame(array, model.findIniField("voltages").get());
        assertEquals(6, array.getSize());
        assertFalse(model.getAllIniFields().containsKey("voltages[0]"),
            "Element views must not duplicate array storage when iterating declared fields");

        assertEquals(21, model.findIniField("bytes[1]").get().getOffset());
        assertEquals(FieldType.INT8, ((ScalarIniField) model.findIniField("bytes[1]").get()).getType());
        assertEquals(28, model.findIniField("floats[1]").get().getOffset());
        assertEquals(FieldType.FLOAT, ((ScalarIniField) model.findIniField("floats[1]").get()).getType());
        assertSame(model.getAllIniFields().get("scalar"), model.findIniField("scalar").get());
    }

    @Test
    public void invalidArrayElementReferencesAreNotResolved() {
        IniFileModel model = readArrayFields();
        for (String key : new String[]{"voltages[-1]", "voltages[3]", "voltages[2147483648]",
            "voltages[]", "voltages[1.0]", "voltages[x]", "voltages[1", "voltages[0][1]",
            "voltages[0]suffix", "missing[0]", "scalar[0]", "table[0]"}) {
            assertFalse(model.findIniField(key).isPresent(), key);
        }
    }

    @Test
    public void arrayElementOnSecondaryPageRetainsWireIdentifier() throws Throwable {
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getPageIdentifier(1)).thenReturn(0x100);
        String text = "[Constants]\npage = 2\n"
            + "voltages = array, U16, 4, [3], \"V\", 0.001, 0, 0, 5, 3\n";
        IniFileModel model = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "array.ini", meta);
        IniField element = model.findIniField("voltages[2]").get();
        assertEquals(0x100, element.getPageIndex());
        assertEquals(8, element.getOffset());
    }

    private static IniFileModel readArrayFields() {
        String text = "[Constants]\npage = 1\n"
            + "voltages = array, U16, 4, [3], \"V\", 0.001, 0, 0, 5, 3\n"
            + "scalar = scalar, F32, 12, \"\", 1, 0, 0, 100, 1\n"
            + "bytes = array, S08, 20, [3], \"\", 1, 0, 0, 100, 0\n"
            + "floats = array, F32, 24, [3], \"\", 1, 0, 0, 100, 2\n"
            + "table = array, U16, 40, [2x3], \"\", 1, 0, 0, 100, 0\n";
        return IniFileReaderTest.readLines(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))));
    }

    @Test
    public void datalogPreservesDeclarationOrderLabelsAndSectionBoundaries() {
        String text = "[datalog]\n"
                + "entry = RPMValue, \"RPM\", int, \"%d\"\n"
                + "entry = ignitionAdvanceCyl1, \"Ign: Timing Cyl 1\", float, \"%.3f\"\n"
                + "entry = RPMValue, \"Engine, speed\", int, \"%d\"\n"
                + "[Other]\nentry = ignored, \"Ignored\", int, \"%d\"\n";
        IniFileModel model = IniFileReaderTest.readLines(IniFileReaderUtil.read(
                new ByteArrayInputStream(text.getBytes(java.nio.charset.StandardCharsets.US_ASCII))));
        assertEquals(3, model.getDatalogEntries().size());
        assertEquals("RPMValue", model.getDatalogEntries().get(0).getChannel());
        assertEquals("RPM", model.getDatalogEntries().get(0).getLabel());
        assertEquals("ignitionAdvanceCyl1", model.getDatalogEntries().get(1).getChannel());
        assertEquals("Ign: Timing Cyl 1", model.getDatalogEntries().get(1).getLabel());
        assertEquals("Engine, speed", model.getDatalogEntries().get(2).getLabel());
        assertThrows(UnsupportedOperationException.class, () -> model.getDatalogEntries().clear());
    }

    @Test
    public void testFindIniFieldInPrimaryFields() {
        String string =
            "[Constants]\n" +
            "page = 1\n" +
            "testField = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        Optional<IniField> field = model.findIniField("testField");
        assertTrue(field.isPresent());
        assertEquals("testField", field.get().getName());
    }

    @Test
    public void testFindIniFieldInSecondaryFields() {
        // Secondary fields are from page 2 or higher
        String string =
            "[Constants]\n" +
            "page = 2\n" +
            "secondaryField = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "page = 1\n" +
            "primaryField = scalar, F32, 4, \"unit\", 1, 0, 0, 100, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        // Verify primary field is found
        Optional<IniField> primaryField = model.findIniField("primaryField");
        assertTrue(primaryField.isPresent());
        assertEquals("primaryField", primaryField.get().getName());

        // Verify secondary field is also found via findIniField
        Optional<IniField> secondaryField = model.findIniField("secondaryField");
        assertTrue(secondaryField.isPresent());
        assertEquals("secondaryField", secondaryField.get().getName());
    }

    @Test
    public void testFindIniFieldNotFound() {
        String string =
            "[Constants]\n" +
            "page = 1\n" +
            "testField = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        Optional<IniField> field = model.findIniField("nonExistentField");
        assertFalse(field.isPresent());
    }

    @Test
    public void testFindIniFieldPrimaryTakesPrecedence() {
        // If a field with the same name exists in both primary and secondary,
        // primary should take precedence
        String string =
            "[Constants]\n" +
            "page = 1\n" +
            "duplicateField = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "page = 2\n" +
            "duplicateField = scalar, F32, 4, \"unit\", 1, 0, 0, 200, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        Optional<IniField> field = model.findIniField("duplicateField");
        assertTrue(field.isPresent());
        // Should get the one from page 1 (offset 0)
        assertEquals(0, field.get().getOffset());
    }

    @Test
    public void testFindIniFieldCaseInsensitive() {
        // ImmutableIniFileModel uses case-insensitive maps
        String string =
            "[Constants]\n" +
            "page = 1\n" +
            "TestField = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "page = 2\n" +
            "SecondaryField = scalar, F32, 4, \"unit\", 1, 0, 0, 100, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderTest.readLines(lines);

        // Test case insensitivity for primary field
        Optional<IniField> field1 = model.findIniField("testfield");
        assertTrue(field1.isPresent());
        assertEquals("TestField", field1.get().getName());

        Optional<IniField> field2 = model.findIniField("TESTFIELD");
        assertTrue(field2.isPresent());
        assertEquals("TestField", field2.get().getName());

        // Test case insensitivity for secondary field
        Optional<IniField> field3 = model.findIniField("secondaryfield");
        assertTrue(field3.isPresent());
        assertEquals("SecondaryField", field3.get().getName());

        Optional<IniField> field4 = model.findIniField("SECONDARYFIELD");
        assertTrue(field4.isPresent());
        assertEquals("SecondaryField", field4.get().getName());
    }
}
