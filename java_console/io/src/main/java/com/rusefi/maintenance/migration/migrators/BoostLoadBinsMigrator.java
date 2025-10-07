package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

public enum BoostLoadBinsMigrator implements TuneMigrator {
    INSTANCE;

    public static final String BOOST_LOAD_BINS_FIELD_NAME = "boostLoadBins";
    public static final String BOOST_OPEN_LOOP_LOAD_BINS_FIELD_NAME = "boostOpenLoopLoadBins";
    public static final String BOOST_CLOSED_LOOP_LOAD_BINS_FIELD_NAME = "boostClosedLoopLoadBins";

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        final IniFileModel prevIniFile = context.getPrevIniFile();
        final Optional<IniField> prevBoostLoadBinsIniField = prevIniFile.findIniField(BOOST_LOAD_BINS_FIELD_NAME);
        if (!prevBoostLoadBinsIniField.isPresent()) {
            return;
        }
        if (!(prevBoostLoadBinsIniField.get() instanceof ArrayIniField)) {
            context.logWarn(String.format(
                "`%s` .ini-field is expected to be instance of `ArrayIniField`, but actually is instance of`%s`.",
                BOOST_LOAD_BINS_FIELD_NAME,
                prevBoostLoadBinsIniField.get().getClass().getSimpleName()
            ));
            return;
        }
        final ArrayIniField prevBoostLoadBinsArrayIniField = (ArrayIniField) prevBoostLoadBinsIniField.get();

        final Constant prevBoostLoadBinsConstant = context.getPrevTune().getConstantsAsMap().get(
            BOOST_LOAD_BINS_FIELD_NAME
        );
        if (null == prevBoostLoadBinsConstant) {
            return; // nothing to migrate
        }

        final String prevBoostLoadBinsValue = prevBoostLoadBinsConstant.getValue();
        if (null == prevBoostLoadBinsValue) {
            return; // nothing to migrate
        }

        migrateBoostLoadBinsToNewBoostLoadBinsIfNeeded(
            context,
            prevBoostLoadBinsArrayIniField,
            prevBoostLoadBinsConstant,
            BOOST_OPEN_LOOP_LOAD_BINS_FIELD_NAME
        );
        migrateBoostLoadBinsToNewBoostLoadBinsIfNeeded(
            context,
            prevBoostLoadBinsArrayIniField,
            prevBoostLoadBinsConstant,
            BOOST_CLOSED_LOOP_LOAD_BINS_FIELD_NAME
        );

        final IniFileModel updatedIniFile = context.getUpdatedIniFile();
        final Optional<IniField> updatedBoostLoadBinsIniField = updatedIniFile.findIniField(BOOST_LOAD_BINS_FIELD_NAME);
        if (!updatedBoostLoadBinsIniField.isPresent()) {
            // Add null migration to prevent processing of disappeared `boostLoadBins` ini-field by
            // `DefaultTuneMigrator`:
            context.addMigration(BOOST_LOAD_BINS_FIELD_NAME, null);
        }
    }

    private void migrateBoostLoadBinsToNewBoostLoadBinsIfNeeded(
        final TuneMigrationContext context,
        final ArrayIniField prevBoostLoadBinsArrayIniField,
        final Constant prevBoostLoadBinsConstant,
        final String newBoostLoadBinsFieldName
    ) {
        final IniFileModel prevIniFile = context.getPrevIniFile();
        final Optional<IniField> prevNewBoostLoadBinsField = prevIniFile.findIniField(newBoostLoadBinsFieldName);
        if (prevNewBoostLoadBinsField.isPresent()) {
            return; // `DefaultTuneMigrator` will migrate new boost load bins .ini field properly
        }

        final IniFileModel updatedIniFile = context.getUpdatedIniFile();
        final Optional<IniField> updatedNewBoostLoadBinsIniField = updatedIniFile.findIniField(
            newBoostLoadBinsFieldName
        );
        if (!updatedNewBoostLoadBinsIniField.isPresent()) {
            return; // no target .ini-field
        }

        if (!(updatedNewBoostLoadBinsIniField.get() instanceof ArrayIniField)) {
            context.logWarn(String.format(
                "`%s` .ini-field is expected to be instance of `ArrayIniField`, but actually is instance of`%s`.",
                updatedNewBoostLoadBinsIniField,
                updatedNewBoostLoadBinsIniField.get().getClass().getSimpleName()
            ));
            return;
        }
        final ArrayIniField updatedNewBoostLoadBinsArrayIniField
            = (ArrayIniField) updatedNewBoostLoadBinsIniField.get();

        if (updatedNewBoostLoadBinsArrayIniField.getType() != prevBoostLoadBinsArrayIniField.getType()) {
            context.logWarn(String.format(
                "Type mismatch: %s and %s",
                prevBoostLoadBinsArrayIniField,
                updatedNewBoostLoadBinsArrayIniField
            ));
            return;
        }
        if (updatedNewBoostLoadBinsArrayIniField.getCols() != prevBoostLoadBinsArrayIniField.getCols()) {
            context.logWarn(String.format(
                "Cols mismatch: %s and %s",
                prevBoostLoadBinsArrayIniField,
                updatedNewBoostLoadBinsArrayIniField
            ));
            return;
        }
        if (updatedNewBoostLoadBinsArrayIniField.getRows() != prevBoostLoadBinsArrayIniField.getRows()) {
            context.logWarn(String.format(
                "Rows mismatch: %s and %s",
                prevBoostLoadBinsArrayIniField,
                updatedNewBoostLoadBinsArrayIniField
            ));
            return;
        }
        if (!updatedNewBoostLoadBinsArrayIniField.getDigits().equals(prevBoostLoadBinsArrayIniField.getDigits())) {
            context.logWarn(String.format(
                "Digits mismatch: %s and %s",
                prevBoostLoadBinsArrayIniField,
                updatedNewBoostLoadBinsArrayIniField
            ));
            return;
        }

        context.addMigration(newBoostLoadBinsFieldName, prevBoostLoadBinsConstant);
    }
}
