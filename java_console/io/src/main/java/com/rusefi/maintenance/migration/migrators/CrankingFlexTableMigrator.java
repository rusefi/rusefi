package com.rusefi.maintenance.migration.migrators;

import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.generateConstant;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

/**
 * Migrates legacy cranking flex-fuel tunes onto the new 2D coolant-vs-ethanol table.
 *
 * Old firmware blended two 1D coolant curves - {@code crankingFuelCoef} (E0) and {@code crankingFuelCoefE100}
 * (E100) - by the flex sensor reading, using {@code interpolateClamped(0, E0, 85, E100, ethanol%)}.
 * The new firmware drops that blend and instead reads {@code crankingFuelFlexTable}, a 2D table over
 * (coolant, ethanol%). {@code crankingFuelCoefE100} has been retired (renamed to {@code unused...}).
 *
 * This migrator reconstructs the new table from the user's old E0/E100 curves so their cranking flex behaviour
 * is preserved: each ethanol row reproduces the legacy 85%-anchored blend of the two curves at that ethanol level.
 */
public enum CrankingFlexTableMigrator implements TuneMigrator {
    INSTANCE;

    private static final String E0_CURVE_FIELD = "crankingFuelCoef";          // exists before and after
    private static final String E100_CURVE_FIELD = "crankingFuelCoefE100";    // retired by this change
    private static final String FLEX_TABLE_FIELD = "crankingFuelFlexTable";   // new 2D table (coolant x ethanol%)
    private static final String FLEX_ETHANOL_BINS_FIELD = "crankingFuelFlexBins"; // new ethanol % axis

    // Legacy firmware anchored the E0->E100 blend at this ethanol %, clamping above it.
    private static final double LEGACY_ETHANOL_ANCHOR = 85.0;

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        // Only migrate when the E100 curve existed before but is gone now, and the new table exists - i.e. this is
        // exactly the upgrade introducing the 2D table. Mirrors the guard used by MultiplierToTableMigrator.
        final boolean appliesToThisUpgrade =
            context.getPrevIniFile().findIniField(E100_CURVE_FIELD).isPresent()
                && !context.getUpdatedIniFile().findIniField(E100_CURVE_FIELD).isPresent()
                && context.getUpdatedIniFile().findIniField(FLEX_TABLE_FIELD).isPresent();
        if (appliesToThisUpgrade) {
            migrate(context);
        }
    }

    private static void migrate(final TuneMigrationContext context) {
        final Optional<ArrayIniField> e0Field = arrayField(context.getPrevIniFile().findIniField(E0_CURVE_FIELD));
        final Optional<ArrayIniField> e100Field = arrayField(context.getPrevIniFile().findIniField(E100_CURVE_FIELD));
        final Optional<ArrayIniField> tableField = arrayField(context.getUpdatedIniFile().findIniField(FLEX_TABLE_FIELD));
        final Optional<ArrayIniField> ethanolBinsField =
            arrayField(context.getUpdatedIniFile().findIniField(FLEX_ETHANOL_BINS_FIELD));
        if (!e0Field.isPresent() || !e100Field.isPresent() || !tableField.isPresent() || !ethanolBinsField.isPresent()) {
            return;
        }

        final Constant e0Const = context.getPrevTune().getConstantsAsMap().get(E0_CURVE_FIELD);
        final Constant e100Const = context.getPrevTune().getConstantsAsMap().get(E100_CURVE_FIELD);
        final Constant ethanolBinsConst = context.getUpdatedTune().getConstantsAsMap().get(FLEX_ETHANOL_BINS_FIELD);
        if (e0Const == null || e100Const == null || ethanolBinsConst == null) {
            return;
        }

        final double[] e0 = readArray(e0Field.get(), e0Const.getValue());
        final double[] e100 = readArray(e100Field.get(), e100Const.getValue());
        final double[] ethanol = readArray(ethanolBinsField.get(), ethanolBinsConst.getValue());

        if (e0.length != e100.length) {
            context.logWarn(String.format(
                "Cranking flex migration skipped: %s has %d values but %s has %d",
                E0_CURVE_FIELD, e0.length, E100_CURVE_FIELD, e100.length));
            return;
        }

        final ArrayIniField table = tableField.get();
        final int rows = table.getRows();
        final int cols = table.getCols();
        // One axis is coolant (length == e0.length), the other is ethanol (length == ethanol.length). Disambiguate
        // by size so the migrator is robust to the table's row/column orientation.
        final boolean colsAreCoolant = (cols == e0.length) && (rows == ethanol.length);
        final boolean rowsAreCoolant = (rows == e0.length) && (cols == ethanol.length);
        if (!colsAreCoolant && !rowsAreCoolant) {
            context.logWarn(String.format(
                "Cranking flex migration skipped: unexpected %s shape %dx%d for coolant(%d) x ethanol(%d)",
                FLEX_TABLE_FIELD, rows, cols, e0.length, ethanol.length));
            return;
        }

        final String[][] values = new String[rows][cols];
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                final int coolantIdx = colsAreCoolant ? col : row;
                final int ethanolIdx = colsAreCoolant ? row : col;
                final double fraction = legacyBlendFraction(ethanol[ethanolIdx]);
                final double value = e0[coolantIdx] + fraction * (e100[coolantIdx] - e0[coolantIdx]);
                values[row][col] = IdleCurveMigrator.formatArrayValue(value, table.getDigits());
            }
        }

        context.addMigration(FLEX_TABLE_FIELD, generateConstant(table, table.formatValue(values)));
    }

    /**
     * Reproduces the legacy {@code interpolateClamped(0, E0, 85, E100, ethanol%)} blend fraction at a given ethanol %.
     */
    private static double legacyBlendFraction(final double ethanolPercent) {
        final double clamped = Math.max(0.0, Math.min(ethanolPercent, LEGACY_ETHANOL_ANCHOR));
        return clamped / LEGACY_ETHANOL_ANCHOR;
    }

    private static Optional<ArrayIniField> arrayField(final Optional<IniField> field) {
        if (field.isPresent() && (field.get() instanceof ArrayIniField)) {
            return Optional.of((ArrayIniField) field.get());
        }
        return Optional.empty();
    }

    private static double[] readArray(final ArrayIniField field, final String value) {
        final String[][] matrix = field.getValues(value);
        final double[] result = new double[field.getRows() * field.getCols()];
        int i = 0;
        for (final String[] matrixRow : matrix) {
            for (final String element : matrixRow) {
                result[i++] = Double.parseDouble(element);
            }
        }
        return result;
    }
}
