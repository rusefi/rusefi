package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.FakePortScanner;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.FirmwareSelection;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

class OpenBltNoIniFlashStepsTest {
    @TempDir
    Path tempDir;

    @Test
    void factoryCarriesPreparedFirmwareSelectionIntoFlashAndNeverRunsSettingsRestore() throws Exception {
        PortResult port = new PortResult("COM_NO_INI", SerialPortType.Ecu);
        ConnectedEcuTarget sharedTarget = new ConnectedEcuTarget();
        sharedTarget.setConnectedTargetForUnitTest("board-a");
        ConnectivityContext context = new ConnectivityContext(
            new FakePortScanner(), sharedTarget);
        LinkManager linkManager = new LinkManager();
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        String signature = "rusEFI master.2026.08.30.board-b.1234567890";
        Path srec = tempDir.resolve("rusefi_master_2026-08-30_board-b_1234567890_fixture.srec");
        Files.write(srec, new byte[]{1});
        FirmwareSelection selection =
            FirmwareSelection.fromSrec(signature, srec.toString(), UpdateOperationCallbacks.DUMMY);
        assertNotNull(selection);
        AtomicInteger flashCalls = new AtomicInteger();
        AtomicReference<PortResult> flashedPort = new AtomicReference<>();
        AtomicReference<String> flashedSignature = new AtomicReference<>();
        AtomicReference<ConnectedEcuTarget> preparedTarget = new AtomicReference<>();
        AtomicReference<ConnectedEcuTarget> flashedTarget = new AtomicReference<>();
        AtomicReference<FirmwareSelection> flashedSelection = new AtomicReference<>();
        OpenBltManualJob.FlashSteps steps = OpenBltManualJobFactory.createNoIniUpdateSteps(
            port, context, linkManager, signature,
            (ignoredCallbacks, target, expectedSignature) -> {
                preparedTarget.set(target);
                assertEquals(signature, expectedSignature);
                return selection;
            }, (parent, targetPort, targetSignature, ignoredCallbacks, ignoredContext, target,
                selectedFirmware) -> {
                flashCalls.incrementAndGet();
                flashedPort.set(targetPort);
                flashedSignature.set(targetSignature);
                flashedTarget.set(target);
                flashedSelection.set(selectedFirmware);
                return true;
            });

        assertTrue(steps.ensureFirmware(callbacks, sharedTarget));
        steps.restoreCalibrations(callbacks, context);
        assertEquals(OpenBltManualJob.RestoreResult.SKIPPED,
            steps.restoreCalibrationsWithResult(callbacks, context));
        verify(callbacks).logLine("Settings backup and restoration were intentionally skipped because no matching INI was available.");

        assertTrue(steps.flash(null, port.port, callbacks, sharedTarget));
        assertEquals(1, flashCalls.get());
        assertEquals(port, flashedPort.get());
        assertEquals(signature, flashedSignature.get());
        assertEquals("board-b", preparedTarget.get().effectiveTarget());
        assertEquals("board-b", flashedTarget.get().effectiveTarget());
        assertSame(preparedTarget.get(), flashedTarget.get());
        assertSame(selection, flashedSelection.get());
        assertNotSame(sharedTarget, preparedTarget.get(), "no-INI recovery must not reuse the process-wide identity");
    }

    @Test
    void missingValidatedSelectionStopsBeforeFlash() {
        PortResult port = new PortResult("COM_NO_INI", SerialPortType.Ecu);
        ConnectivityContext context = new ConnectivityContext(new FakePortScanner(), new ConnectedEcuTarget());
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        AtomicInteger flashCalls = new AtomicInteger();
        OpenBltManualJob.FlashSteps steps = OpenBltManualJobFactory.createNoIniUpdateSteps(
            port, context, null, "rusEFI master.2026.08.30.board-b.1234567890",
            (ignoredCallbacks, target, signature) -> null,
            (parent, targetPort, signature, ignoredCallbacks, ignoredContext, target, selection) -> {
                flashCalls.incrementAndGet();
                return true;
            });

        assertFalse(steps.ensureFirmware(callbacks, context.getConnectedEcuTarget()));
        assertFalse(steps.flash(null, port.port, callbacks, context.getConnectedEcuTarget()));
        assertEquals(0, flashCalls.get());
    }
}
