package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.IniFileModel;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.util.Collections;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;

class KansasLimaTuningDetectorMigratorTest {
    @Test
    void missingVersionInfoDoesNotPreventOtherMigrations() {
        TuneMigrationContext context = new TuneMigrationContext(
            mock(IniFileModel.class), mock(Msq.class), mock(IniFileModel.class), mock(Msq.class),
            mock(UpdateOperationCallbacks.class), Collections.emptySet());

        assertDoesNotThrow(() -> KansasLimaTuningDetectorMigrator.INSTANCE.migrateTune(context));
        assertTrue(context.getMigratedConstants().isEmpty());
    }

    @Test
    void missingDestinationVersionInfoDoesNotEnableDetection() {
        Msq source = new Msq();
        source.versionInfo.setSignature("rusEFI lts-25kansas.2026.08.02.proteus_f7.123");
        TuneMigrationContext context = new TuneMigrationContext(
            mock(IniFileModel.class), source, mock(IniFileModel.class), mock(Msq.class),
            mock(UpdateOperationCallbacks.class), Collections.emptySet());

        assertDoesNotThrow(() -> KansasLimaTuningDetectorMigrator.INSTANCE.migrateTune(context));
        assertTrue(context.getMigratedConstants().isEmpty());
    }
}
