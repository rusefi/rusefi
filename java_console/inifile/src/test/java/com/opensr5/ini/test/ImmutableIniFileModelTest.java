package com.opensr5.ini.test;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.IniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Tests for ImmutableIniFileModel
 */
public class ImmutableIniFileModelTest {

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
