package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IniFileProvider;
import com.rusefi.config.generated.Integration;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.IoStream;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Optional;
import java.util.Collections;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class CalibrationsHelperLayoutMigrationTest {
    private static final String OLD_SIGNATURE = "rusEFI master.2026.11.02.test.123";
    private static final String NEW_SIGNATURE = "rusEFI master.2026.11.06.test.456";

    @TempDir
    Path tempDir;

    @Test
    void changedOffsetsWithEqualPageSizeCanLockTheMigrationDestination() throws Exception {
        IniFileModel before = ini(OLD_SIGNATURE, 2, 0);
        IniFileModel after = ini(NEW_SIGNATURE, 0, 2);
        assertEquals(before.getMetaInfo().getPageSize(0), after.getMetaInfo().getPageSize(0));
        byte[] stored = {42, 0, 0, 0, 55, 0};
        byte[] defaults = {0, 0, 10, 0, 7, 0};
        AtomicBoolean reset = new AtomicBoolean();
        BinaryProtocol bp = protocol(after, reset, true);
        when(bp.readFullImageFromController(any(ConfigurationImageMeta.class))).thenAnswer(call -> {
            byte[] current = reset.get() ? defaults : stored;
            return current[0] != 0 ? ConfigurationImageWithMeta.VOID
                : new ConfigurationImageWithMeta(call.getArgument(0), current.clone());
        });
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        try {
            BinaryProtocol.iniFileProvider = signature -> after;
            assertTrue(CalibrationsHelper.prepareCalibrationsForMigration(bp, before, UpdateOperationCallbacks.DUMMY));
            Optional<CalibrationsInfo> destination = CalibrationsHelper.readCurrentCalibrations(bp, UpdateOperationCallbacks.DUMMY);

            // [tag:post_flash_layout_reset] #10590: old PWM bytes must not become the new lock key.
            assertTrue(reset.get());
            assertTrue(destination.isPresent());
            assertArrayEquals(defaults, destination.get().getImage().getConfigurationImage().getContent());
            assertEquals(42, stored[0]);
            CalibrationsInfo source = new CalibrationsInfo(before,
                new ConfigurationImageWithMeta(null, stored.clone()));
            CalibrationsHelper.MergeResult merged = CalibrationsHelper.mergeCalibrationsWithPartialFailure(
                before, source.generateMsq(), destination.get(), UpdateOperationCallbacks.DUMMY, Collections.emptySet());
            assertTrue(merged.failedFields.isEmpty(), merged.failedFields.toString());
            assertArrayEquals(new byte[]{0, 0, 42, 0, 7, 0},
                merged.mergedCalibrations.orElseThrow().getImage().getConfigurationImage().getContent());
            assertArrayEquals(new byte[]{42, 0, 0, 0, 55, 0}, stored);
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
        }
    }

    @Test
    void unchangedLayoutHashAcrossDatesDoesNotResetSettings() throws Exception {
        IniFileModel before = ini(OLD_SIGNATURE, 2, 0);
        IniFileModel after = ini(NEW_SIGNATURE.replace("456", "123"), 2, 0);
        AtomicBoolean reset = new AtomicBoolean();
        BinaryProtocol bp = protocol(after, reset, true);
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        try {
            BinaryProtocol.iniFileProvider = signature -> after;
            assertTrue(CalibrationsHelper.prepareCalibrationsForMigration(bp, before, UpdateOperationCallbacks.DUMMY));
            assertFalse(reset.get());
            verify(bp, never()).executeCommand(anyChar(), any(byte[].class), anyString());
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
        }
    }

    @Test
    void resetFailureStopsMigrationBeforeAnyCalibrationRead() throws Exception {
        IniFileModel before = ini(OLD_SIGNATURE, 2, 0);
        IniFileModel after = ini(NEW_SIGNATURE, 0, 2);
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        try {
            BinaryProtocol.iniFileProvider = signature -> after;
            for (byte[] response : new byte[][]{null, {(byte) Integration.TS_RESPONSE_OUT_OF_RANGE},
                {(byte) Integration.TS_RESPONSE_OK, 0}}) {
                BinaryProtocol bp = protocol(after, new AtomicBoolean(), false);
                doReturn(response).when(bp).executeCommand(eq(Integration.TS_EXECUTE), any(byte[].class), eq("rewriteconfig"));
                assertFalse(CalibrationsHelper.prepareCalibrationsForMigration(bp, before, UpdateOperationCallbacks.DUMMY));
                verify(bp, never()).readFullImageFromController(any(ConfigurationImageMeta.class));
            }
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
        }
    }

    @Test
    void declinedBackupRequiresDefaultsRatherThanKeepingUnverifiedStorage() throws Exception {
        IniFileModel after = ini(NEW_SIGNATURE, 0, 2);
        AtomicBoolean reset = new AtomicBoolean();
        BinaryProtocol bp = protocol(after, reset, true);
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        try {
            BinaryProtocol.iniFileProvider = signature -> after;
            assertTrue(CalibrationsHelper.prepareCalibrationsForMigration(bp, null, UpdateOperationCallbacks.DUMMY));
            assertTrue(reset.get());
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
        }
    }

    private BinaryProtocol protocol(IniFileModel after, AtomicBoolean reset, boolean resetSucceeds) throws Exception {
        String signature = after.getSignature();
        byte[] hello = new byte[signature.length() + 2];
        hello[0] = (byte) Integration.TS_RESPONSE_OK;
        System.arraycopy(signature.getBytes(StandardCharsets.US_ASCII), 0, hello, 1, signature.length());
        IoStream stream = mock(IoStream.class);
        IncomingDataBuffer buffer = mock(IncomingDataBuffer.class);
        when(stream.getDataBuffer()).thenReturn(buffer);
        doReturn(hello).when(buffer).getPacket(anyInt(), anyString());
        BinaryProtocol bp = mock(BinaryProtocol.class);
        when(bp.getStream()).thenReturn(stream);
        when(bp.executeCommand(eq(Integration.TS_EXECUTE), any(byte[].class), eq("rewriteconfig")))
            .thenAnswer(call -> {
                assertArrayEquals(BinaryProtocol.getTextCommandBytesOnlyText("rewriteconfig"), call.getArgument(1));
                reset.set(resetSucceeds);
                return resetSucceeds ? new byte[]{(byte) Integration.TS_RESPONSE_OK} : null;
            });
        return bp;
    }

    @Test
    void preparationRetriesTransientPostFlashFailuresWithoutRealSleeps() {
        AtomicInteger attempts = new AtomicInteger();
        AtomicInteger waits = new AtomicInteger();
        assertTrue(CalibrationsHelper.prepareCalibrationsForMigration(
            () -> attempts.incrementAndGet() == 2, waits::incrementAndGet));
        assertEquals(2, attempts.get());
        assertEquals(1, waits.get());

        attempts.set(0);
        waits.set(0);
        assertFalse(CalibrationsHelper.prepareCalibrationsForMigration(() -> {
            attempts.incrementAndGet();
            return false;
        }, waits::incrementAndGet));
        assertEquals(3, attempts.get());
        assertEquals(2, waits.get());
    }

    private IniFileModel ini(String signature, int lockOffset, int tableOffset) throws Exception {
        Path path = Files.createTempFile(tempDir, "layout", ".ini");
        Files.write(path, ("[MegaTune]\nsignature = \"" + signature + "\"\n"
            + "[Constants]\nnPages = 1\npageSize = 6\npageReadCommand = \"R\"\nburnCommand = \"B\"\npage = 1\n"
            + "tuneHidingKey = scalar, U16, " + lockOffset + ", \"\", 1, 0, 0, 65535, 0\n"
            + "pwm = scalar, U16, " + tableOffset + ", \"\", 1, 0, 0, 65535, 0\n"
            + (lockOffset == 0 ? "newSetting = scalar, U16, 4, \"\", 1, 0, 0, 65535, 0\n" : "")
            + "[OutputChannels]\nochBlockSize = 4\n").getBytes(StandardCharsets.UTF_8));
        return IniFileReaderUtil.readIniFile(path.toString());
    }
}
