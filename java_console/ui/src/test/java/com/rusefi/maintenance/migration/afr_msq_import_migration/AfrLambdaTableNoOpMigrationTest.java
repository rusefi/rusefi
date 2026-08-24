package com.rusefi.maintenance.migration.afr_msq_import_migration;

import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.AfrLambdaTableMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContext;
import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.LAMBDA_TABLE_FIELD_NAME;
import static org.junit.jupiter.api.Assertions.assertFalse;

class AfrLambdaTableNoOpMigrationTest {
    private static final ArrayIniField LAMBDA_TABLE_FIELD = new ArrayIniField(
        LAMBDA_TABLE_FIELD_NAME,
        0,
        FieldType.UINT8,
        1,
        1,
        "lambda",
        0.01,
        "0.6",
        "1.5",
        "2"
    );

    private static final Constant LAMBDA_TABLE_VALUE = new Constant(
        LAMBDA_TABLE_FIELD_NAME,
        "lambda",
        "1.00",
        "2",
        "1",
        "1"
    );

    @Test
    void unchangedLambdaTableIsNotMigrated() {
        final TestTuneMigrationContext context = createTestMigrationContext(
            LAMBDA_TABLE_VALUE,
            LAMBDA_TABLE_FIELD,
            LAMBDA_TABLE_VALUE,
            LAMBDA_TABLE_FIELD
        );

        AfrLambdaTableMigrator.INSTANCE.migrateTune(context);

        assertFalse(context.getMigratedConstants().containsKey(LAMBDA_TABLE_FIELD_NAME));
    }
}
