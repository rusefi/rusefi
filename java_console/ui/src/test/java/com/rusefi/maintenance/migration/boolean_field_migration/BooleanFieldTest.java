package com.rusefi.maintenance.migration.boolean_field_migration;

import org.junit.jupiter.api.Test;

import com.rusefi.maintenance.migration.migrators.BooleanField;

import java.util.Optional;

import static javax.management.ObjectName.quote;
import static org.junit.jupiter.api.Assertions.*;

public class BooleanFieldTest {

    @Test
    void testMigrateField() {
        BooleanField field = new BooleanField("testField", "enabled", "disabled");

        // Test migration of true value to "enabled"
        assertEquals(quote("enabled"), field.migrateField(quote("true")));

        // Test migration of false value to "disabled"
        assertEquals(quote("disabled"), field.migrateField(quote("false")));

    }

    @Test
    void testShouldMigrateField() {
        BooleanField field = new BooleanField("testField", "enabled", "disabled");

        // Test with legacy values that should be migrated
        Optional<Boolean> result = field.shouldMigrateField(quote("true"), "disabled");
        assertTrue(result.isPresent());
        assertTrue(result.get());

        result = field.shouldMigrateField(quote("false"), "disabled");
        assertTrue(result.isPresent());
        assertTrue(result.get());

        // Test with already migrated values that should not be migrated again
        result = field.shouldMigrateField(quote("enabled"), "disabled");
        assertTrue(result.isPresent());
        assertFalse(result.get());

        result = field.shouldMigrateField(quote("disabled"), "disabled");
        assertTrue(result.isPresent());
        assertFalse(result.get());

        // Test with invalid values that should return empty Optional
        result = field.shouldMigrateField(quote("iAmACorruptedTune"), "no");
        assertFalse(result.isPresent());
    }

    @Test
    void testGetters() {
        String fieldName = "testField";
        String trueValue = "enabled";
        String falseValue = "disabled";

        BooleanField field = new BooleanField(fieldName, trueValue, falseValue);

        assertEquals(fieldName, field.getFieldName());
        assertEquals(quote(trueValue), field.getTrueValue());
        assertEquals(quote(falseValue), field.getFalseValue());
    }

    @Test
    void testShouldMigrateWithInvalidNewTuneFalseValue() {
        BooleanField field = new BooleanField("testField", "enabled", "disabled");

        // Test with valid legacy value but invalid for this migration (ie, fw & new tune are legacy),  see #8611 for details
        Optional<Boolean> result = field.shouldMigrateField(quote("true"), "false");
        assertFalse(result.isPresent());
    }
}
