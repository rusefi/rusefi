package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.*;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.DefaultIniFieldMigrator;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.maintenance.migration.TuneMigrator;
import com.rusefi.output.ConfigStructure;
import com.rusefi.tune.xml.Constant;

import java.util.*;

import static com.devexperts.logging.Logging.getLogging;

public enum DefaultTuneMigrator implements TuneMigrator {
    INSTANCE;

    private static final Logging log = getLogging(DefaultTuneMigrator.class);

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        final IniFileModel prevIni = context.getPrevIniFile();
        final Map<String, Constant> prevValues = context.getPrevTune().getConstantsAsMap();
        final IniFileModel newIni = context.getUpdatedIniFile();
        final Map<String, Constant> newValues = context.getUpdatedTune().getConstantsAsMap();
        final UpdateOperationCallbacks callbacks = context.getCallbacks();
        final Map<String, IniField> prevIniFields = prevIni.getAllIniFields();
        final Map<String, IniField> newIniFields = newIni.getAllIniFields();
        for (final Map.Entry<String, IniField> prevFieldEntry: prevIniFields.entrySet()) {
            final String prevFieldName = prevFieldEntry.getKey();
            if (!isUnusedField(prevFieldName)) {
                // We do not want to migrate already migrated ini-fields:
                if (!context.isFieldAlreadyMigrated(prevFieldName) && !context.getMigratedConstants().containsKey(
                    prevFieldEntry
                )) {
                    final IniField newField = newIniFields.get(prevFieldName);
                    final Constant prevValue = prevValues.get(prevFieldName);
                    if (newField != null) {
                        final Constant newValue = newValues.get(prevFieldName);
                        if (prevValue != null) {
                            if (newValue == null) { // new value is empty
                                log.info(String.format(
                                    "Field `%s` is going to be restored: <none> -> `%s`",
                                    prevFieldName,
                                    prevValue.getValue()
                                ));

                                context.addMigration(prevFieldName, prevValue);
                            } else {
                                if (!Objects.equals(prevValue.getValue(), newValue.getValue())) {
                                    if (!Objects.equals(prevValue.getName(), newValue.getName())) {
                                        callbacks.logLine(String.format(
                                            "WARNING! Field `%s` cannot be updated because its name is updated: `%s` -> `%s`",
                                            prevFieldName,
                                            prevValue.getName(),
                                            newValue.getName()
                                        ));
                                    } else if (!Objects.equals(prevValue.getUnits(), newValue.getUnits())) {
                                        callbacks.logLine(String.format(
                                            "WARNING! Field `%s` cannot be updated because its units are updated: `%s` -> `%s`",
                                            prevFieldName,
                                            prevValue.getUnits(),
                                            newValue.getUnits()
                                        ));
                                    } else if (!Objects.equals(prevValue.getDigits(), newValue.getDigits())) {
                                        callbacks.logLine(String.format(
                                            "WARNING! Field `%s` cannot be updated because its digits are updated: `%s` -> `%s`",
                                            prevFieldName,
                                            prevValue.getDigits(),
                                            newValue.getDigits()
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
                                        final Optional<String> migratedValue = DefaultIniFieldMigrator.INSTANCE.tryMigrateValue(
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
                            }
                        }
                    } else if (prevValue != null) {
                        callbacks.logLine(String.format(
                            "We aren't going to restore field `%s`: it is missed in new .ini file",
                            prevFieldName
                        ));
                    }
                }
            }
        }
    }

    private static boolean isUnusedField(final String fieldName) {
        return fieldName.startsWith(ConfigStructure.UNUSED_ANYTHING_PREFIX);
    }
}
