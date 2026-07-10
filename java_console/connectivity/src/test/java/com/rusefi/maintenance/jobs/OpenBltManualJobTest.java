package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.FakePortScanner;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import javax.swing.*;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests the manual OpenBLT flash choreography around the always-on scanner: the port must be held
 * exclusively (suspend) for the whole flash, then invalidated + released (resume) no matter how the
 * flash ended, and the tune restore must only run after the scanner is back (it waits for the
 * rebooted ECU to reappear). Driven through the injected {@link OpenBltManualJob.FlashSteps} and a
 * {@link FakePortScanner}, so no hardware and no real flasher. [tag:better_ux_for_flashing]
 */
public class OpenBltManualJobTest {
    private static final String PORT = "COM7";

    private final FakePortScanner scanner = new FakePortScanner();
    private final ConnectivityContext connectivityContext = new ConnectivityContext(scanner);
    private final RecordingCallbacks callbacks = new RecordingCallbacks();
    private final AtomicInteger jobFinishedCount = new AtomicInteger();

    private static class RecordingCallbacks implements UpdateOperationCallbacks {
        int doneCount;
        int errorCount;
        final List<String> lines = new ArrayList<>();

        @Override
        public void log(final String message, final boolean breakLineOnTextArea, final boolean sendToLogger) {
            lines.add(message);
        }

        @Override
        public void done() {
            doneCount++;
        }

        @Override
        public void warning() {
        }

        @Override
        public void error() {
            errorCount++;
        }

        @Override
        public void clear() {
        }
    }

    /**
     * Scripted steps that record the scanner's suspend/resume/invalidate state at the moment each
     * step runs, so tests can assert the ordering of the choreography rather than just call counts.
     */
    private class ScriptedSteps implements OpenBltManualJob.FlashSteps {
        boolean ensureFirmwareResult = true;
        boolean flashResult = true;
        boolean flashThrows;

        int ensureFirmwareCount;
        int flashCount;
        String flashedPort;
        int suspendCountAtFlash;
        int resumeCountAtFlash;
        int invalidatedCountAtFlash;
        int restoreCount;
        int resumeCountAtRestore;

        @Override
        public boolean ensureFirmware(final UpdateOperationCallbacks callbacks, final ConnectedEcuTarget target) {
            ensureFirmwareCount++;
            return ensureFirmwareResult;
        }

        @Override
        public boolean flash(final JComponent parent, final String port, final UpdateOperationCallbacks callbacks,
                             final ConnectedEcuTarget target) {
            flashCount++;
            flashedPort = port;
            suspendCountAtFlash = scanner.suspendCount;
            resumeCountAtFlash = scanner.resumeCount;
            invalidatedCountAtFlash = scanner.invalidatedPorts.size();
            if (flashThrows) {
                throw new RuntimeException("flasher blew up");
            }
            return flashResult;
        }

        @Override
        public void restoreCalibrations(final UpdateOperationCallbacks callbacks,
                                        final ConnectivityContext connectivityContext) {
            restoreCount++;
            resumeCountAtRestore = scanner.resumeCount;
        }
    }

    private final ScriptedSteps steps = new ScriptedSteps();

    private OpenBltManualJob newJob() {
        return new OpenBltManualJob(new PortResult(PORT, SerialPortType.OpenBlt), null, connectivityContext, steps);
    }

    @Test
    public void successfulFlashSuspendsScannerForTheFlashThenInvalidatesResumesAndRestoresTune() {
        newJob().doJob(callbacks, jobFinishedCount::incrementAndGet);

        // the flash ran against the selected port while the scanner was suspended and untouched otherwise
        assertEquals(1, steps.flashCount);
        assertEquals(PORT, steps.flashedPort);
        assertEquals(1, steps.suspendCountAtFlash, "scanner must be suspended before flashing starts");
        assertEquals(0, steps.resumeCountAtFlash, "scanner must not be resumed while flashing");
        assertEquals(0, steps.invalidatedCountAtFlash, "port is invalidated only after the flash");

        // after the flash: re-inspect the rebooted port, release the scanner, then restore the tune
        assertEquals(Collections.singletonList(PORT), scanner.invalidatedPorts);
        assertEquals(1, scanner.resumeCount);
        assertEquals(1, steps.restoreCount);
        assertEquals(1, steps.resumeCountAtRestore, "restore waits for the rebooted ECU, so the scanner must already be running");

        assertEquals(1, callbacks.doneCount);
        assertEquals(0, callbacks.errorCount);
        assertEquals(1, jobFinishedCount.get());
    }

    @Test
    public void failedFlashReportsErrorAndSkipsRestoreButStillReleasesScanner() {
        steps.flashResult = false;

        newJob().doJob(callbacks, jobFinishedCount::incrementAndGet);

        assertEquals(Collections.singletonList(PORT), scanner.invalidatedPorts);
        assertEquals(1, scanner.resumeCount, "scanner must be released even when the flash fails");
        assertEquals(0, steps.restoreCount, "no tune restore after a failed flash");
        assertEquals(0, callbacks.doneCount);
        assertEquals(1, callbacks.errorCount);
        assertEquals(1, jobFinishedCount.get());
    }

    @Test
    public void crashingFlasherStillInvalidatesPortAndReleasesScanner() {
        steps.flashThrows = true;

        assertThrows(RuntimeException.class, () -> newJob().doJob(callbacks, jobFinishedCount::incrementAndGet));

        assertEquals(Collections.singletonList(PORT), scanner.invalidatedPorts);
        assertEquals(1, scanner.resumeCount, "scanner must be released even when the flasher crashes");
        assertEquals(0, steps.restoreCount);
        assertEquals(1, jobFinishedCount.get(), "JobHelper must report job completion even on a crash");
    }

    @Test
    public void missingFirmwareAbortsBeforeTouchingTheScannerOrThePort() {
        steps.ensureFirmwareResult = false;

        newJob().doJob(callbacks, jobFinishedCount::incrementAndGet);

        assertEquals(1, steps.ensureFirmwareCount);
        assertEquals(0, steps.flashCount);
        assertEquals(0, scanner.suspendCount);
        assertEquals(0, scanner.resumeCount);
        assertTrue(scanner.invalidatedPorts.isEmpty());
        assertEquals(0, steps.restoreCount);
        assertEquals(0, callbacks.doneCount);
        assertEquals(1, callbacks.errorCount);
        assertEquals(1, jobFinishedCount.get());
    }
}
