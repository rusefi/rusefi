package com.rusefi.maintenance.migration.migrators;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;
import static java.lang.Double.parseDouble;

public enum DisplacementIniFieldMigrator implements TuneMigrator {
    INSTANCE;

    private static final Logging log = getLogging(DisplacementIniFieldMigrator.class);
    public static final String DISPLACEMENT_FIELD_NAME = "displacement";
    public static final String L_UNITS = "L";
    public static final String CUBIC_INCHES_UNITS = "cubic inches";

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        final Optional<IniField> prevDisplacementIniField = context.getPrevIniFile().findIniField(DISPLACEMENT_FIELD_NAME);
        final Optional<IniField> updatedDisplacementIniField = context.getUpdatedIniFile().findIniField(DISPLACEMENT_FIELD_NAME);
        if (!prevDisplacementIniField.isPresent() || !updatedDisplacementIniField.isPresent()) {
            return;
        }

        final String prevDisplacementUnits = prevDisplacementIniField.get().getUnits();
        final String updatedDisplacementUnits = updatedDisplacementIniField.get().getUnits();
        if (!L_UNITS.equals(prevDisplacementUnits) || !CUBIC_INCHES_UNITS.equals(updatedDisplacementUnits)) {
            return;
        }

        final Constant prevDisplacementConst = context.getPrevTune().getConstantsAsMap().get(DISPLACEMENT_FIELD_NAME);
        final Constant updatedDisplacementConst = context.getUpdatedTune().getConstantsAsMap().get(DISPLACEMENT_FIELD_NAME);
        if ((null == prevDisplacementConst) || (null == updatedDisplacementConst)) {
            return;
        }

        final String prevDisplacementStringValue = prevDisplacementConst.getValue();
        if (null == prevDisplacementStringValue) {
            log.warn(String.format("Previous `%s` constant has null value - it's strange :(", DISPLACEMENT_FIELD_NAME));
            return;
        }

        try {
            final double prevDisplacementValue = parseDouble(prevDisplacementStringValue);
            final double migratedDisplacementValue = prevDisplacementValue * 61.0236100347;
            final Constant migratedDisplacementConst = updatedDisplacementConst.cloneWithValue(
                String.valueOf(migratedDisplacementValue)
            );
            context.addMigration(DISPLACEMENT_FIELD_NAME, migratedDisplacementConst);
        } catch (final NumberFormatException e) {
            log.warn(String.format(
                "Failed to parse previous `%s` constant value: %s",
                DISPLACEMENT_FIELD_NAME,
                prevDisplacementStringValue
            ));
        }
    }
}
