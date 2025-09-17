package com.rusefi.maintenance.migration;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.IniField;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;
import static javax.management.ObjectName.quote;


class BooleanField {
    private String fieldName;   // ex: launchControlEnabled
    private String trueValue;   // ex: "enabled"
    private String falseValue;  // ex: "disabled"

    public BooleanField(String fieldName, String trueValue, String falseValue) {
        this.fieldName = fieldName;
        this.falseValue = quote(falseValue);
        this.trueValue = quote(trueValue);
    }

    public String migrateField(String oldTuneFieldValue) {
        if (oldTuneFieldValue.startsWith(quote("true"))) {
            return trueValue;
        }
        return falseValue;
    }

    public Optional<Boolean> shouldMigrateField(String oldTuneFieldValue) {
        boolean isLegacyValue = oldTuneFieldValue.startsWith(quote("true")) || oldTuneFieldValue.startsWith(quote("false"));
        // ie: already migrated on previous run/migration/etc
        boolean isMigratedValue = oldTuneFieldValue.startsWith(trueValue) || oldTuneFieldValue.startsWith(falseValue);

        if (!isLegacyValue && !isMigratedValue) {
            // invalid!, error on migration code? or on updated .ini?
            return Optional.empty();
        }
        return Optional.of(isLegacyValue);
    }

    public String getFieldName() {
        return fieldName;
    }

    public String getTrueValue() {
        return trueValue;
    }

    public String getFalseValue() {
        return falseValue;
    }

}

public enum BooleanIniFieldMigrator implements TuneMigrator {
    INSTANCE;

    private static final Logging log = getLogging(BooleanIniFieldMigrator.class);

    public static final String LAUNCH_CONTROL_FIELD_NAME = "launchControlEnabled";

    private static final BooleanField[] fieldsToMigrate = {
        // List all the boolean fields to migrate here, you should also update the tests!!
        new BooleanField(LAUNCH_CONTROL_FIELD_NAME, "enabled", "disabled"),
        new BooleanField("isInjectionEnabled", "enabled", "disabled"),
        new BooleanField("isIgnitionEnabled", "enabled", "disabled"),
        new BooleanField("isCylinderCleanupEnabled", "enabled", "disabled"),
        new BooleanField("isMapAveragingEnabled", "enabled", "disabled"),
        new BooleanField("isFasterEngineSpinUpEnabled", "enabled", "disabled"),
        new BooleanField("enableSoftwareKnock", "enabled", "disabled"),
        new BooleanField("fuelClosedLoopCorrectionEnabled", "enabled", "disabled"),
        new BooleanField("isSdCardEnabled", "enabled", "disabled"),
        new BooleanField("isBoostControlEnabled", "enabled", "disabled"),
        new BooleanField("useSeparateAdvanceForIdle", "enabled", "disabled"),
        new BooleanField("useSeparateVeForIdle", "enabled", "disabled"),
        new BooleanField("useSeparateIdleTablesForCrankingTaper", "enabled", "disabled"),
        new BooleanField("useIacTableForCoasting", "enabled", "disabled"),
        new BooleanField("nitrousControlEnabled", "enabled", "disabled"),
        new BooleanField("stftIgnoreErrorMagnitude", "yes", "no"),
        new BooleanField("enableLaunchRetard", "yes", "no"),
        new BooleanField("useIdleAdvanceWhileCoasting", "yes", "no"),
        new BooleanField("launchFuelCutEnable", "yes", "no"),
        new BooleanField("launchSparkCutEnable", "yes", "no"),
        new BooleanField("useAdvanceCorrectionsForCranking", "yes", "no"),
    };

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        for (BooleanField field : fieldsToMigrate) {

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

            Optional<Boolean> shouldMigrate = field.shouldMigrateField(prevBooleanValue);

            // check if migration of this value is possible/valid
            if (shouldMigrate.isPresent()) {
                if(shouldMigrate.get()) {
                    final Constant migratedBooleanConst = prevBooleanConst.cloneWithValue(
                        String.valueOf(field.migrateField(prevBooleanValue))
                    );
                    context.addMigration(field.getFieldName(), migratedBooleanConst);
                }
            } else {
                log.warn(String.format("Invalid boolean field value on oldTune `%s` neither true/false or `%s`/`%s` :(", field.getFieldName(), field.getTrueValue(), field.getFalseValue()));
            }

        }
    }
}
