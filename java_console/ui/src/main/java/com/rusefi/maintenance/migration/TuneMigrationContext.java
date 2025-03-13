package com.rusefi.maintenance.migration;

import com.opensr5.ini.IniFileModel;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import java.util.HashMap;
import java.util.Map;

public class TuneMigrationContext {
    private final IniFileModel prevIniFile;
    private final Msq prevTune;
    private final IniFileModel updatedIniFile;
    private final Msq updatedTune;
    private final Map<String, Constant> migratedConstants = new HashMap<>();
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
        return migratedConstants;
    }

    public UpdateOperationCallbacks getCallbacks() {
        return callbacks;
    }
}
