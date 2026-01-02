package com.rusefi.maintenance.migration.migrators;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Locale;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;

import java.text.MessageFormat;

//note: this migration is different from the one on IniFieldMigratorUtils.java,
//here we execute a lambda for every field to migrate to patch the old scale to the new

interface DigitMigrator {
   Double doMigration(Double prevValue);
}

class IntDigitField {
    private String fieldName;           // ex: ppsExpAverageAlpha
    private String expectedPreviousDigits; // ex: 3
    private String expectedNewDigits;      // ex: 1
    private DigitMigrator migrator;     // lambda used to migrate the field, unique param is the old value

    public IntDigitField(String fieldName, DigitMigrator migrator, String expectedPreviousDigits, String expectedNewDigits ) {
        this.fieldName = fieldName;
        this.migrator = migrator;
        this.expectedPreviousDigits = expectedPreviousDigits;
        this.expectedNewDigits = expectedNewDigits;
    }

    public Optional<Boolean> shouldMigrateField(String previousDigits, String newDigits) {
        // "we want to migrate only if prev/new are equal to the defined ones on this class initiation and they are different"
        boolean isValidToMigrate = previousDigits.startsWith(this.expectedPreviousDigits) && newDigits.startsWith(this.expectedNewDigits) && !previousDigits.startsWith(newDigits);
        // ie: already migrated on previous run/migration/etc
        boolean isMigratedValue = previousDigits == newDigits;

        if (!isValidToMigrate && !isMigratedValue) {
            // invalid!, error on migration code? or on updated .ini?
            return Optional.empty();
        }
        return Optional.of(isValidToMigrate);
    }

    public String migrateField(String oldTuneFieldValue) {
        // "12" => 12
        Double fieldValue =  Double.parseDouble(oldTuneFieldValue);

        // 12 => 12.456
        Double migratedField = this.migrator.doMigration(fieldValue);

        // migratedField = 12.456; expectedNewDigits = 2; => 12.45
        String migratedValue = String.format(Locale.ROOT, MessageFormat.format("%.{0}f", this.expectedNewDigits), migratedField);

        return migratedValue;
    }

    public String getFieldName() {
        return fieldName;
    }
}

public enum DigitsIniFieldMigrator implements TuneMigrator {
    INSTANCE;

    private static final Logging log = getLogging(DigitsIniFieldMigrator.class);

    private static final IntDigitField[] fieldsToMigrate = {
        // List all the fields to migrate here:
        new IntDigitField("ppsExpAverageAlpha", (v) -> v * 100, "3" , "1"),

        // converted from raw ADC to Volts:
        new IntDigitField("tpsMin", (v) -> v * 0.0048828125, "0" , "2"),
        new IntDigitField("tpsMax", (v) -> v * 0.0048828125, "0" , "2"),
        new IntDigitField("tps1SecondaryMin", (v) -> v * 0.0048828125, "0" , "2"),
        new IntDigitField("tps1SecondaryMax", (v) -> v * 0.0048828125, "0" , "2"),
        new IntDigitField("tps2Min", (v) -> v * 0.0048828125, "0" , "2"),
        new IntDigitField("tps2Max", (v) -> v * 0.0048828125, "0" , "2"),
        new IntDigitField("tps2SecondaryMin", (v) -> v * 0.0048828125, "0" , "2"),
        new IntDigitField("tps2SecondaryMax", (v) -> v * 0.0048828125, "0" , "2"),
    };

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        for (IntDigitField field : fieldsToMigrate) {

            final Optional<IniField> prevBooleanIniField = context.getPrevIniFile().findIniField(field.getFieldName());
            final Optional<IniField> updatedBooleanIniField = context.getUpdatedIniFile().findIniField(field.getFieldName());

            if (!prevBooleanIniField.isPresent() || !updatedBooleanIniField.isPresent()) {
                continue;
            }

            final Constant prevBooleanConst = context.getPrevTune().getConstantsAsMap().get(field.getFieldName());
            final Constant updatedBooleanConst = context.getUpdatedTune().getConstantsAsMap().get(field.getFieldName());
            final String prevBooleanValue = prevBooleanConst.getValue();

            if ((null == prevBooleanConst) || (null == updatedBooleanConst)) {
                continue;
            }

            Optional<Boolean> shouldMigrate = field.shouldMigrateField(prevBooleanConst.getDigits(), updatedBooleanConst.getDigits());

            // check if migration of this value is possible/valid
            if (shouldMigrate.isPresent()) {
                if(shouldMigrate.get()) {
                    final Constant migratedBooleanConst = prevBooleanConst.cloneWithValue(
                        String.valueOf(field.migrateField(prevBooleanValue))
                    );
                    context.addMigration(field.getFieldName(), migratedBooleanConst);
                }
            } else {
                log.warn(String.format("Invalid scalar migration on oldTune `%s` :(", field.getFieldName()));
            }

        }
    }
}
