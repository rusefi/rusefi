package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import com.rusefi.maintenance.migration.migrators.AfrLambdaTableMigrator;
import com.rusefi.maintenance.migration.migrators.DefaultTuneMigrator;
import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContext;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class AfrLambdaTableMigratorTest {
    private static final String LAMBDA = "lambdaTable";

    @Test
    void identicalLambdaTableIsNotMigrated() {
        TestTuneMigrationContext ctx = context(tableText("1.0"), tableText("1.0"));

        AfrLambdaTableMigrator.INSTANCE.migrateTune(ctx);
        DefaultTuneMigrator.INSTANCE.migrateTune(ctx);

        assertFalse(ctx.getMigratedConstants().containsKey(LAMBDA),
            "an identical lambda table must not be re-migrated on every load");
    }

    @Test
    void differentLambdaTableIsMigrated() {
        TestTuneMigrationContext ctx = context(tableText("1.0"), tableText("0.95"));

        AfrLambdaTableMigrator.INSTANCE.migrateTune(ctx);
        DefaultTuneMigrator.INSTANCE.migrateTune(ctx);

        assertTrue(ctx.getMigratedConstants().containsKey(LAMBDA),
            "a differing lambda table must be migrated");
    }

    private static TestTuneMigrationContext context(String prevText, String updatedText) {
        ArrayIniField field = new ArrayIniField(
            LAMBDA, 0, FieldType.UINT8, 16, 16, "lambda", 1 / 147.0, "0.3", "3", "2");
        Constant prev = new Constant(LAMBDA, "lambda", prevText, "2");
        Constant updated = new Constant(LAMBDA, "lambda", updatedText, "2");
        return createTestMigrationContext(prev, field, updated, field);
    }

    /** Matches ArrayIniField.formatValue: leading newline, 8-space indent + space, trailing newline. */
    private static String tableText(String value) {
        StringBuilder sb = new StringBuilder();
        for (int row = 0; row < 16; row++) {
            sb.append("\n        ");
            for (int col = 0; col < 16; col++) {
                sb.append(' ').append(value);
            }
        }
        sb.append('\n');
        return sb.toString();
    }
}
