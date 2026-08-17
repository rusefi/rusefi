package com.rusefi.maintenance.migration;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.AfrLambdaTableMigrator;
import com.rusefi.maintenance.migration.migrators.DefaultTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContext;
import static org.junit.jupiter.api.Assertions.assertFalse;

/**
 * Reproduces the 21129 "lambdaTable restored on every load" report:
 * the file text is written into an image (as the burn does), read back the
 * way Msq generation does, and the migrators must then see no change.
 */
public class Lambda21129RoundTripTest {
    private static final String LAMBDA = "lambdaTable";
    private static final String UNITS_TEMPLATE = "{useLambdaOnInterface ? \"lambda\" : \"afr\"}";
    private static final String DIGITS_TEMPLATE = "{useLambdaOnInterface ? 2 : 1}";

    private static final String FILE_TEXT =
    "\n" +
    "         1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.952 0.952 0.884 0.837 0.837 0.837 0.837\n" +
    "         1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.952 0.952 0.884 0.837 0.837 0.837 0.837\n" +
    "         1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.952 0.952 0.884 0.837 0.837 0.837 0.837\n" +
    "         1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.925 0.912 0.871 0.837 0.837 0.837 0.837\n" +
    "         1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.878 0.864 0.85 0.837 0.837 0.837 0.837\n" +
    "         1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.837 0.837 0.837 0.837 0.837 0.837 0.837\n" +
    "         1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.837 0.837 0.837 0.837 0.837 0.837 0.837\n" +
    "         0.98 0.98 0.98 0.98 0.98 0.98 0.966 0.966 0.966 0.837 0.837 0.837 0.837 0.837 0.837 0.837\n" +
    "         0.959 0.959 0.959 0.959 0.959 0.959 0.939 0.939 0.939 0.837 0.837 0.837 0.837 0.837 0.837 0.837\n" +
    "         0.939 0.939 0.939 0.939 0.939 0.939 0.905 0.905 0.905 0.837 0.837 0.837 0.837 0.837 0.837 0.837\n" +
    "         0.918 0.918 0.918 0.918 0.918 0.918 0.871 0.871 0.871 0.837 0.837 0.837 0.837 0.837 0.837 0.837\n" +
    "         0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.83\n" +
    "         0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.823\n" +
    "         0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.823\n" +
    "         0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.83 0.83 0.823\n" +
    "         0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.823 0.823 0.823\n" +
    "";

    @Test
    void fileTextRoundTripsThroughImageWithoutMigration() {
        final ArrayIniField field = new ArrayIniField(
            LAMBDA, 0, FieldType.UINT8, 16, 16,
            UNITS_TEMPLATE, 1 / 147.0, "0", "25", DIGITS_TEMPLATE);

        // 1. Burn: write the file text into a fresh image, exactly as setValue2 does
        final ConfigurationImage image = new ConfigurationImage(field.getSize());
        ConfigurationImageGetterSetter.setValue2(field, image, LAMBDA, FILE_TEXT);

        // 2. ECU read-back: exactly what Msq generation (getStringValue) does
        final String ecuText = ConfigurationImageGetterSetter.getStringValue(field, image);

        System.out.println("=== round-trip identical: " + FILE_TEXT.equals(ecuText));
        if (!FILE_TEXT.equals(ecuText)) {
            final String[] a = FILE_TEXT.split("\\s+");
            final String[] b = ecuText.split("\\s+");
            for (int i = 0; i < a.length; i++) {
                if (!a[i].equals(b[i])) {
                    System.out.println("first diff at cell " + i + ": file='" + a[i] + "' ecu='" + b[i] + "'");
                    break;
                }
            }
        }

        final Constant prev = new Constant(LAMBDA, UNITS_TEMPLATE, FILE_TEXT, DIGITS_TEMPLATE);
        final Constant updated = new Constant(LAMBDA, UNITS_TEMPLATE, ecuText, DIGITS_TEMPLATE);
        final TestTuneMigrationContext ctx = createTestMigrationContext(prev, field, updated, field);

        AfrLambdaTableMigrator.INSTANCE.migrateTune(ctx);
        DefaultTuneMigrator.INSTANCE.migrateTune(ctx);

        System.out.println("=== migrated constants: " + ctx.getMigratedConstants().keySet());
        assertFalse(ctx.getMigratedConstants().containsKey(LAMBDA),
            "the lambda table must not be re-migrated after a successful burn round-trip");
    }
}
