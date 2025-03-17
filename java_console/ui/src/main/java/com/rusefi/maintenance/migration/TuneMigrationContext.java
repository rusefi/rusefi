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

    public TuneMigrationContext(
        final IniFileModel prevIniFile,
        final Msq prevTune,
        final IniFileModel updatedIniFile,
        final Msq updatedTune,
        final UpdateOperationCallbacks callbacks
    ) {
        this.prevIniFile = prevIniFile;
        this.prevTune = prevTune;
        this.updatedIniFile = updatedIniFile;
        this.updatedTune = updatedTune;
        this.callbacks = callbacks;
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
        final Constant existingMigration = migratedConstants.get(migratedFieldName);
        if (existingMigration != null) {
            callbacks.logLine(String.format(
                "WARNING!!! We are overwriting already migrated constant `%s`: `%s` -> `%s`",
                migratedFieldName,
                existingMigration,
                migratedFieldName
            ));
        }
        migratedConstants.put(migratedFieldName, migratedValue);
    }
}
