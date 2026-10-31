package com.rusefi.maintenance.migration.migrators;

import com.rusefi.maintenance.migration.TuneMigrationContext;

/**
 * TuneMigrator is an interface for migrating user tuning data across different firmware versions.
 *
 * When the configuration structure in `rusefi_config.txt` (firmware side) changes—for example,
 * when a field is renamed, its scaling is modified, or a scalar is converted to an array—
 * a TuneMigrator implementation may be needed to ensure that user tunes are correctly preserved.
 *
 * All migrators are registered in {@link com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator}.
 *
 * <h3>Common Migrator Types:</h3>
 * <ul>
 *   <li>{@link com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator}:
 *       Handles resizing of 2D tables (e.g., VE, Ignition) when columns or rows are added.</li>
 *   <li>{@link com.rusefi.maintenance.migration.migrators.ArrayFieldScaleMigrator}:
 *       Handles precision changes (e.g., scale moving from 0.1 to 0.01).</li>
 *   <li>{@link com.rusefi.maintenance.migration.migrators.BooleanIniFieldMigrator}:
 *       Handles migration of boolean fields, including logic inversion (e.g., "true"/"false" to "yes"/"no") or renaming.</li>
 *   <li>{@link com.rusefi.maintenance.migration.migrators.STFTFieldMigrator}:
 *       Specialized migration for Short Term Fuel Trim fields, handling type changes (INT8 to FLOAT) and sign changes.</li>
 *   <li>{@link com.rusefi.maintenance.migration.migrators.DisplacementIniFieldMigrator}:
 *       Example of unit conversion migrator (e.g., Liters to Cubic Inches).</li>
 *   <li>{@link com.rusefi.maintenance.migration.migrators.DefaultTuneMigrator}:
 *       The final "catch-all" stage that migrates any remaining fields where names and units match.</li>
 * </ul>
 *
 * <h3>Testing:</h3>
 * Migration logic is tested in the {@code java_console/ui} module under:
 * {@code src/test/java/com/rusefi/maintenance/migration/}
 *
 * Key tests include:
 * <ul>
 *   <li>{@code DefaultTuneMigratorTest}: Integration-style test for the full migration pipeline, utilizing real-world
 *       scenarios with test data in {@code test_data/}.</li>
 *   <li>Specific migrator tests like {@code ArrayFieldScaleMigratorTest}, {@code BooleanIniFieldMigratorTest},
 *       or {@code TableAddColumnsMigratorTest} for isolated validation of complex rules.</li>
 * </ul>
 */
public interface TuneMigrator {
    void migrateTune(TuneMigrationContext context);
}
