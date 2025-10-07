package com.rusefi.maintenance.migration.clt_idle_corr_migration;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import java.util.Map;

import static com.rusefi.maintenance.migration.migrators.CltIdleCorrMigrator.OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME;
import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContextWithDisappearedConst;
import static org.junit.jupiter.api.Assertions.*;

public class CheckIfOverrideCrankingIacSettingHasDisappearedTest {
    private static final IniField TEST_OVERRIDE_CRANKING_IAC_SETTING_INI_FIELD = new EnumIniField(
        OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME,
        1328,
        FieldType.INT,
        new EnumIniField.EnumKeyValueMap(Map.of(0, "\"false\"", 1, "\"true\"")),
        6,
        0
    );

    @Test
    void testMigrationWithDisappearedTrueOverrideCrankingIacSetting() {
        final TestTuneMigrationContext testContext = createTestMigrationContextWithDisappearedConst(
            new Constant(
                OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME,
                null,
                "\"true\"",
                null
            ),
            TEST_OVERRIDE_CRANKING_IAC_SETTING_INI_FIELD
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        assertNull(testContext.getMigratedConstants().get(OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME));
        assertEquals("", testContext.getTestCallbacks().getContent());
    }

    @Test
    void testMigrationWithDisappearedFalseOverrideCrankingIacSetting() {
        final TestTuneMigrationContext testContext = createTestMigrationContextWithDisappearedConst(
            new Constant(
                OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME,
                null,
                "\"false\"",
                null
            ),
            TEST_OVERRIDE_CRANKING_IAC_SETTING_INI_FIELD
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        assertNull(testContext.getMigratedConstants().get(OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME));
        assertEquals(
            "WARNING! `overrideCrankingIacSetting` ini-field with value `\"false\"` cannot be migrated.\r\n",
            testContext.getTestCallbacks().getContent()
        );
    }
}
