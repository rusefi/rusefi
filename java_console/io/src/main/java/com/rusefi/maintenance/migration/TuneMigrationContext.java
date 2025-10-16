package com.rusefi.maintenance.migration;

import com.opensr5.ini.IniFileModel;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class TuneMigrationContext {
    private final IniFileModel prevIniFile;
    private final Msq prevTune;
    private final IniFileModel updatedIniFile;
    private final Msq updatedTune;
    private final Map<String, Constant> migratedConstants = new HashMap<>();
    private final Set<String> migratedFields = new HashSet<>();
    final UpdateOperationCallbacks callbacks;
    final Set<String> additionalIniFieldsToIgnore;

    public TuneMigrationContext(
        final IniFileModel prevIniFile,
        final Msq prevTune,
        final IniFileModel updatedIniFile,
        final Msq updatedTune,
        final UpdateOperationCallbacks callbacks,
        final Set<String> additionalIniFieldsToIgnore
    ) {
        this.prevIniFile = prevIniFile;
        this.prevTune = prevTune;
        this.updatedIniFile = updatedIniFile;
        this.updatedTune = updatedTune;
        this.callbacks = callbacks;
        this.additionalIniFieldsToIgnore = additionalIniFieldsToIgnore;
    }

    public IniFileModel getPrevIniFile() {
        return prevIniFile;
    }

    public Msq getPrevTune() {
        return prevTune;
    }

    public IniFileModel getUpdatedIniFile() {
        return updatedIniFile;
    }

    public Msq getUpdatedTune() {
        return updatedTune;
    }

    public boolean isAdditionalIniFieldToIgnore(final String iniFieldName) {
        return additionalIniFieldsToIgnore.contains(iniFieldName);
    }

    public Map<String, Constant> getMigratedConstants() {
        return Collections.unmodifiableMap(migratedConstants);
    }

    public UpdateOperationCallbacks getCallbacks() {
        return callbacks;
    }

    public void addMigration(final String migratedFieldName, final Constant migratedValue) {
        addMigration(migratedFieldName, migratedFieldName, migratedValue);
    }

    public boolean isFieldAlreadyMigrated(final String fieldName) {
        return migratedFields.contains(fieldName);
    }

    public void addMigration(
        final String originalFieldName,
        final String migratedFieldName,
        final Constant migratedValue
    ) {
        migratedFields.add(originalFieldName);

        final Constant prevMigratedValue = migratedConstants.put(migratedFieldName, migratedValue);
        if (prevMigratedValue != null) {
            callbacks.logLine(String.format(
                "WARNING!!! We are overwriting already migrated constant `%s`: `%s` -> `%s`",
                migratedFieldName,
                prevMigratedValue,
                migratedValue
            ));
        }
    }

    public void logWarn(final String warning) {
        callbacks.logLine(String.format("WARNING! %s", warning));
    }
}
