package com.rusefi.mcp;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class EcuFirmwareUpdaterTest {
    @Test
    void validatesSrecDataBeforeAnyHardwareAccess(@TempDir Path directory) throws Exception {
        Path file = directory.resolve("firmware.srec");
        Files.writeString(file, "S3060801000001EF\n");
        assertEquals(file.toAbsolutePath(), EcuFirmwareUpdater.validateFirmwarePath(file.toString()));
        Files.writeString(file, "S3060801000001EE\n");
        assertThrows(IOException.class, () -> EcuFirmwareUpdater.validateFirmwarePath(file.toString()));
        Files.writeString(file, "");
        assertThrows(IOException.class, () -> EcuFirmwareUpdater.validateFirmwarePath(file.toString()));
    }

    @Test
    void handoffStopsLoggingAndReportsPartialMigrationAndReconnect() {
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.isActive()).thenReturn(true);
        when(lm.getLastTriedPort()).thenReturn("COM8");
        List<String> events = new ArrayList<>();
        EcuFirmwareUpdater updater = new EcuFirmwareUpdater((link, image, callbacks) -> {
            assertSame(lm, link);
            assertEquals("firmware.srec", image);
            assertFalse(callbacks.isInteractive());
            events.add("preflight");
            callbacks.firmwareHandoffStarted();
            events.add("flash and migrate");
            callbacks.logLine("Calibrations are backed up to files `backup.zip` and `backup.msq`");
            callbacks.logLine("WARNING: field(s) could not be migrated to the new firmware.");
            callbacks.done();
        });
        JSONObject result = updater.update(lm, Path.of("firmware.srec"), () -> events.add("stop logging"));
        assertEquals(Arrays.asList("preflight", "stop logging", "flash and migrate"), events);
        assertEquals(true, result.get("success"));
        assertEquals(true, result.get("handoffStarted"));
        assertEquals(true, result.get("connected"));
        assertEquals("COM8", result.get("port"));
        assertEquals(1, ((JSONArray) result.get("warnings")).size());
        assertTrue(result.get("messages").toString().contains("backup.msq"));
    }

    @Test
    void preflightRejectionDoesNotStopLogging() {
        EcuFirmwareUpdater updater = new EcuFirmwareUpdater((link, image, callbacks) -> {
            assertNull(image);
            callbacks.firmwareUpdateBlocked("Firmware not eligible");
            callbacks.error();
        });
        JSONObject result = updater.update(mock(LinkManager.class), null, () -> fail("Unexpected handoff"));
        assertEquals(false, result.get("success"));
        assertEquals(false, result.get("handoffStarted"));
        assertTrue(result.get("messages").toString().contains("Firmware not eligible"));
    }

    @Test
    void productionJobRejectsSimulatorBeforeHandoff() {
        LinkManager lm = mock(LinkManager.class);
        when(lm.getLastTriedPort()).thenReturn("localhost:29001");
        JSONObject result = new EcuFirmwareUpdater().update(lm, null, () -> fail("Unexpected handoff"));
        assertEquals(false, result.get("success"));
        assertEquals(false, result.get("handoffStarted"));
        assertTrue(result.get("messages").toString().contains("serial or SocketCAN"));
        verify(lm, never()).disconnect();
    }

    @Test
    void migrationFailureAndExceptionsAreNotReportedAsSuccess() {
        for (boolean throwException : new boolean[]{false, true}) {
            EcuFirmwareUpdater updater = new EcuFirmwareUpdater((link, image, callbacks) -> {
                callbacks.firmwareHandoffStarted();
                if (throwException) {
                    throw new IOException("Lost ECU after programming");
                }
                callbacks.logLine("Failed to write merged tune to ECU");
                callbacks.error();
            });
            JSONObject result = updater.update(mock(LinkManager.class), null, () -> { });
            assertEquals(false, result.get("success"));
            assertEquals(true, result.get("handoffStarted"));
            assertEquals(false, result.get("connected"));
            assertTrue(result.get("error").toString().contains("may already have been flashed"));
            assertFalse(((JSONArray) result.get("messages")).isEmpty());
        }
    }
}
