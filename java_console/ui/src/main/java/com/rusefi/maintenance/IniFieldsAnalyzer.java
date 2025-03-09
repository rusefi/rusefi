package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.*;
import com.rusefi.core.Pair;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.ComposedIniFieldMigrator;
import com.rusefi.tune.xml.Constant;

import java.util.*;

import static com.devexperts.logging.Logging.getLogging;

public class IniFieldsAnalyzer {
    private static final Logging log = getLogging(IniFieldsAnalyzer.class);

    public static List<Pair<IniField, Constant>> findValuesToUpdate(
        final IniFileModel prevIni,
        final Map<String, Constant> prevValues,
        final IniFileModel newIni,
        final Map<String, Constant> newValues,
        final UpdateOperationCallbacks callbacks
    ) {
        final List<Pair<IniField, Constant>> result = new ArrayList<>();
        final Map<String, IniField> prevIniFields = prevIni.getAllIniFields();
        final Map<String, IniField> newIniFields = newIni.getAllIniFields();
        for (final Map.Entry<String, IniField> prevFieldEntry: prevIniFields.entrySet()) {
            final String prevFieldName = prevFieldEntry.getKey();
            final IniField newField = newIniFields.get(prevFieldName);
            if (newField != null) {
                final Constant prevValue = prevValues.get(prevFieldName);
                final Constant newValue = newValues.get(prevFieldName);
                if (prevValue != null && newValue != null) {
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
                            final Optional<String> migratedValue = ComposedIniFieldMigrator.INSTANCE.tryMigrateValue(
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
                                        prevValue.getValue(),
                                        newValue.getValue()
                                    ));

                                    result.add(new Pair<>(newField, newValue.cloneWithValue(valueToRestore)));
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
                } else {
                    log.info(String.format("Field `%s` is missed in new .ini file", prevFieldName));
                }
            }
        }
        return result;
    }
}
