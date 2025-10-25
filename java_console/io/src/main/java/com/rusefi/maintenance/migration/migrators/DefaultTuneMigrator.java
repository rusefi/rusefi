package com.rusefi.maintenance.migration.migrators;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.*;
import com.rusefi.CompatibilitySet;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.output.UnusedPrefix;
import com.rusefi.tune.xml.Constant;

import java.util.*;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.checkIfUnitsCanBeMigrated;

public enum DefaultTuneMigrator implements TuneMigrator {
    INSTANCE;

    private static final Logging log = getLogging(DefaultTuneMigrator.class);

    // ini fields to ignore on all boards
    private static final Set<String> INI_FIELDS_TO_IGNORE = CompatibilitySet.of("byFirmwareVersion");

    private static final Set<String> boardSpecificIniFieldsToIgnore = ConnectionAndMeta.getNonMigratableIniFields();


    @Override
    public void migrateTune(final TuneMigrationContext context) {
        final IniFileModel prevIni = context.getPrevIniFile();
        final Map<String, IniField> prevIniFields = prevIni.getAllIniFields();
        for (final Map.Entry<String, IniField> prevFieldEntry : prevIniFields.entrySet()) {
            migrateIniField(context, prevFieldEntry);
        }
    }

    private static void migrateIniField(TuneMigrationContext context, Map.Entry<String, IniField> prevFieldEntry) {
        final String prevFieldName = prevFieldEntry.getKey();
        if (INI_FIELDS_TO_IGNORE.contains(prevFieldName) ||
            boardSpecificIniFieldsToIgnore.contains(prevFieldName) ||
            context.isAdditionalIniFieldToIgnore(prevFieldName) ||
            isUnusedField(prevFieldName)) {
            return;
        }
        if (context.isFieldAlreadyMigrated(prevFieldName) || context.getMigratedConstants().containsKey(prevFieldEntry)) {
            // We do not want to migrate already migrated ini-fields:
            return;
        }
        final UpdateOperationCallbacks callbacks = context.getCallbacks();
        final IniFileModel newIni = context.getUpdatedIniFile();
        final Map<String, Constant> newValues = context.getUpdatedTune().getConstantsAsMap();
        final Map<String, Constant> prevValues = context.getPrevTune().getConstantsAsMap();
        final Map<String, IniField> newIniFields = newIni.getAllIniFields();
        final IniField newField = newIniFields.get(prevFieldName);
        final Constant prevValue = prevValues.get(prevFieldName);

        if (newField == null) {
            if (prevValue != null) {
                callbacks.logLine(String.format(
                    "We aren't going to restore field `%s`: it is missed in new .ini file",
                    prevFieldName
                ));
            }
            return;
        }

        if (prevValue == null) {
            // nothing to migrate - there was no value
            return;
        }

        final Constant newValue = newValues.get(prevFieldName);
        if (newValue == null) { // new value is empty
            log.info(String.format(
                "Field `%s` is going to be restored: <none> -> `%s`",
                prevFieldName,
                prevValue.getValue()
            ));

            context.addMigration(prevFieldName, prevValue);
            return;
        }

        if (Objects.equals(prevValue.getValue(), newValue.getValue())) {
            // values are already the same
            return;
        }

        if (!Objects.equals(prevValue.getName(), newValue.getName())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be updated because its name is updated: `%s` -> `%s`",
                prevFieldName,
                prevValue.getName(),
                newValue.getName()
            ));
        } else if (!checkIfUnitsCanBeMigrated(prevValue.getUnits(), newValue.getUnits())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be updated because its units are updated: `%s` -> `%s`",
                prevFieldName,
                prevValue.getUnits(),
                newValue.getUnits()
            ));
        } else if (!Objects.equals(prevValue.getCols(), newValue.getCols())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be updated because its column count is updated: `%s` -> `%s`",
                prevFieldName,
                prevValue.getCols(),
                newValue.getCols()
            ));
        } else if (!Objects.equals(prevValue.getRows(), newValue.getRows())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be updated because its row count is updated: `%s` -> `%s`",
                prevFieldName,
                prevValue.getRows(),
                newValue.getRows()
            ));
        } else {
            final Optional<String> migratedValue = DefaultIniFieldMigrationStrategy.INSTANCE.tryMigrateValue(
                prevFieldEntry.getValue(),
                newField,
                prevValue.getValue(),
                callbacks
            );
            if (migratedValue.isPresent()) {
                final String valueToRestore = migratedValue.get();
                if (!valueToRestore.equals(newValue.getValue())) {
                    log.info(String.format(
                        "Field `%s` is going to be restored: `%s` -> `%s`",
                        prevFieldName,
                        newValue.getValue(),
                        prevValue.getValue()
                    ));

                    context.addMigration(prevFieldName, newValue.cloneWithValue(valueToRestore));
                } else {
                    callbacks.logLine(String.format(
                        "We aren't going to restore field `%s`: it looks like its value is just renamed: `%s` -> `%s`",
                        prevFieldName,
                        prevValue.getValue(),
                        newValue.getValue()
                    ));
                }
            } else {
                log.warn(String.format(
                    "Field `%s` cannot be updated: `%s` -> `%s`",
                    prevFieldName,
                    prevFieldEntry.getValue(),
                    newField
                ));
            }
        }
    }

    private static boolean isUnusedField(final String fieldName) {
        return fieldName.startsWith(UnusedPrefix.UNUSED_ANYTHING_PREFIX);
    }
}
