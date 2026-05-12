package com.rusefi.ts;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;
import java.util.Collections;

public class TsProjectUpdaterSandbox {
    public static void main(String[] args) throws JAXBException {
        // todo: fix me, why does test context not load?!
        TestTuneMigrationContext result = DefaultTestTuneMigrationContext.load();

        TuneMigrationContext context = new TuneMigrationContext(result.getPrevIniFile(), null,
            null, null, UpdateOperationCallbacks.DUMMY, Collections.emptySet());
        TsProjectUpdater.INSTANCE.afterTuneMigration(context);
    }
}

