package com.rusefi.maintenance;

import com.rusefi.ConnectivityContext;
import com.rusefi.FakePortScanner;
import com.rusefi.PortResult;
import com.rusefi.PortScanner;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CountDownLatch;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

class ProgramSelectorSocketCanTest {
    private static final String FIRMWARE = "firmware.srec";

    @Test
    void scannerIsSuspendedForTheWholeFlashAndInvalidatedBeforeResume() {
        FakePortScanner scanner = new FakePortScanner();
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);

        boolean result = ProgramSelector.flashCanWithSuspendedScanner(
            LinkManager.SOCKET_CAN,
            FIRMWARE,
            callbacks,
            scanner,
            (fileName, openbltCallbacks) -> {
                assertEquals(FIRMWARE, fileName);
                assertEquals(1, scanner.suspendCount);
                assertEquals(0, scanner.resumeCount);
                assertTrue(scanner.invalidatedPorts.isEmpty());
            });

        assertTrue(result);
        assertEquals(Collections.singletonList(LinkManager.SOCKET_CAN), scanner.invalidatedPorts);
        assertEquals(1, scanner.resumeCount);
    }

    @Test
    void scannerIsInvalidatedAndResumedWhenFlashFails() {
        FakePortScanner scanner = new FakePortScanner();

        boolean result = ProgramSelector.flashCanWithSuspendedScanner(
            LinkManager.SOCKET_CAN,
            FIRMWARE,
            mock(UpdateOperationCallbacks.class),
            scanner,
            (fileName, openbltCallbacks) -> {
                throw new IOException("flash failed");
            });

        assertFalse(result);
        assertEquals(Collections.singletonList(LinkManager.SOCKET_CAN), scanner.invalidatedPorts);
        assertEquals(1, scanner.resumeCount);
    }

    @Test
    void pcanScannerIsSuspendedAndInvalidatedAroundFlash() {
        FakePortScanner scanner = new FakePortScanner();

        boolean result = ProgramSelector.flashCanWithSuspendedScanner(
            LinkManager.PCAN,
            FIRMWARE,
            mock(UpdateOperationCallbacks.class),
            scanner,
            (fileName, openbltCallbacks) -> { });

        assertTrue(result);
        assertEquals(Collections.singletonList(LinkManager.PCAN), scanner.invalidatedPorts);
        assertEquals(1, scanner.suspendCount);
        assertEquals(1, scanner.resumeCount);
    }

    @Test
    void scannerResumesEvenWhenInvalidationFails() {
        PortScanner scanner = mock(PortScanner.class);
        when(scanner.suspend()).thenReturn(new CountDownLatch(0));
        doThrow(new IllegalStateException("invalidation failed"))
            .when(scanner).invalidatePort(LinkManager.SOCKET_CAN);

        assertThrows(IllegalStateException.class, () ->
            ProgramSelector.flashCanWithSuspendedScanner(
                LinkManager.SOCKET_CAN,
                FIRMWARE,
                mock(UpdateOperationCallbacks.class),
                scanner,
                (fileName, openbltCallbacks) -> { }));

        verify(scanner).resume();
    }

    @Test
    void automaticEntryRejectsALiveSerialConnectionForSocketCanTarget() {
        LinkManager linkManager = mock(LinkManager.class);
        when(linkManager.getLastTriedPort()).thenReturn("COM5");
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        FakePortScanner scanner = new FakePortScanner();

        boolean result = ProgramSelector.flashOpenbltCanAutomatic(
            null,
            new PortResult(LinkManager.SOCKET_CAN, SerialPortType.Ecu),
            mock(BinaryProtocol.class),
            linkManager,
            callbacks,
            new ConnectivityContext(scanner),
            null,
            CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION);

        assertFalse(result);
        verify(callbacks).logLine("CAN firmware update requires a matching live CAN ECU connection.");
        assertEquals(0, scannerOperations(scanner));
    }

    @Test
    void automaticEntryRejectsSocketCanConnectionForPcanTarget() {
        LinkManager linkManager = mock(LinkManager.class);
        when(linkManager.getLastTriedPort()).thenReturn(LinkManager.SOCKET_CAN);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        FakePortScanner scanner = new FakePortScanner();

        boolean result = ProgramSelector.flashOpenbltCanAutomatic(
            null,
            new PortResult(LinkManager.PCAN, SerialPortType.Ecu),
            mock(BinaryProtocol.class),
            linkManager,
            callbacks,
            new ConnectivityContext(scanner),
            null,
            CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION);

        assertFalse(result);
        verify(callbacks).logLine("CAN firmware update requires a matching live CAN ECU connection.");
        assertEquals(0, scannerOperations(scanner));
    }

    @Test
    void socketCanHandoffRunsOneRebootOnTheCommunicationExecutor() {
        LinkManager linkManager = mock(LinkManager.class);
        List<String> events = new ArrayList<>();
        when(linkManager.submit(any(Runnable.class))).thenAnswer(invocation -> {
            events.add("submit");
            ((Runnable) invocation.getArgument(0)).run();
            return CompletableFuture.completedFuture(null);
        });

        assertTrue(ProgramSelector.prepareCanHandoff(
            LinkManager.SOCKET_CAN, linkManager, mock(UpdateOperationCallbacks.class), () -> {
                events.add("reboot");
                return true;
            }));

        assertEquals(Arrays.asList("submit", "reboot"), events);
        verify(linkManager, times(1)).submit(any(Runnable.class));
    }

    @Test
    void failedSocketCanRebootDoesNotDisconnect() {
        LinkManager linkManager = mock(LinkManager.class);
        when(linkManager.submit(any(Runnable.class))).thenAnswer(invocation -> {
            ((Runnable) invocation.getArgument(0)).run();
            return CompletableFuture.completedFuture(null);
        });

        assertFalse(CalibrationsHelper.prepareFirmwareHandoff(
            mock(BinaryProtocol.class), linkManager,
            () -> ProgramSelector.prepareCanHandoff(
                LinkManager.SOCKET_CAN, linkManager, mock(UpdateOperationCallbacks.class), () -> false)));

        verify(linkManager, never()).disconnect();
    }

    private static int scannerOperations(FakePortScanner scanner) {
        return scanner.suspendCount + scanner.resumeCount + scanner.invalidatedPorts.size();
    }
}
