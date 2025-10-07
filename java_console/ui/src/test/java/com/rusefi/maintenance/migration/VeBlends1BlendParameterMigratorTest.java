package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import java.util.Map;

import static com.rusefi.config.FieldType.UINT8;
import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContext;
import static com.rusefi.maintenance.migration.migrators.VeBlends1BlendParameterMigrator.VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME;
import static java.util.Map.entry;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

public class VeBlends1BlendParameterMigratorTest {
    private static final IniField TEST_VE_BLENDS1_BLEND_PARAMETER_INI_FIELD = new EnumIniField(
        VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME,
        39172,
        UINT8,
        new EnumIniField.EnumKeyValueMap(Map.ofEntries(
            entry(0,"Zero"),
            entry(1, "TPS"),
            entry(2, "MAP"),
            entry(3, "CLT"),
            entry(4, "IAT"),
            entry(5, "Fuel Load"),
            entry(6, "Ignition Load"),
            entry(7, "Aux Temp 1"),
            entry(8, "Aux Temp 2"),
            entry(9, "Accel Pedal"),
            entry(10,"Battery Voltage"),
            entry(11, "VVT 1 In Actual"),
            entry(12, "VVT 1 Ex Actual"),
            entry(13,"VVT 2 In Actual"),
            entry(14,"VVT 2 Ex Actual")
        )),
        0,
        4
    );

    private static final Constant ZERO_VE_BLENDS_1_BLEND_PARAMETER_VALUE = new Constant(
        VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME,
        null,
        "\"Zero\"",
        null
    );
    private static final Constant VVT1_VE_BLENDS1_BLEND_PARAMETER_VALUE = new Constant(
        VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME,
        null,
        "\"VVT 1 In Actual\"",
        null
    );

    private static final Constant VVT2_VE_BLENDS1_BLEND_PARAMETER_VALUE = new Constant(
        VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME,
        null,
        "\"VVT 2 In Actual\"",
        null
    );

    @Test
    void testZeroVeBlends1BlendParameterValueIsNotMigrated() {
        TestTuneMigrationContext testContext = createTestMigrationContext(
            ZERO_VE_BLENDS_1_BLEND_PARAMETER_VALUE,
            TEST_VE_BLENDS1_BLEND_PARAMETER_INI_FIELD,
            VVT1_VE_BLENDS1_BLEND_PARAMETER_VALUE,
            TEST_VE_BLENDS1_BLEND_PARAMETER_INI_FIELD
        );

        com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        final Constant migratedValue = testContext.getMigratedConstants().get(VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME);
        assertNotNull(migratedValue);
        assertEquals(migratedValue, VVT1_VE_BLENDS1_BLEND_PARAMETER_VALUE);
    }

    @Test
    void testNonZeroVeBlends1BlendParameterValueIsMigrated() {
        TestTuneMigrationContext testContext = createTestMigrationContext(
            VVT2_VE_BLENDS1_BLEND_PARAMETER_VALUE,
            TEST_VE_BLENDS1_BLEND_PARAMETER_INI_FIELD,
            VVT1_VE_BLENDS1_BLEND_PARAMETER_VALUE,
            TEST_VE_BLENDS1_BLEND_PARAMETER_INI_FIELD
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        final Constant migratedValue = testContext.getMigratedConstants().get(VE_BLENDS_1_BLEND_PARAMETER_FIELD_NAME);
        assertNotNull(migratedValue);
        assertEquals(migratedValue, VVT2_VE_BLENDS1_BLEND_PARAMETER_VALUE);
    }
}
