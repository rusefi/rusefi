package com.rusefi;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import static java.util.Arrays.asList;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Unit tests for the single-session device state machine [tag:better_ux_for_flashing], driven through
 * a {@link FakePortScanner} injected via {@link ConnectivityContext} — no hardware, no scan thread.
 */
public class DeviceSessionManagerTest {
    private static final AvailableHardware NOTHING =
        new AvailableHardware(Collections.emptyList(), false, false, false);

    private FakePortScanner scanner;
    private ConnectivityContext connectivityContext;

    @BeforeEach
    public void resetGlobalConnectionStatus() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        scanner = new FakePortScanner();
        connectivityContext = new ConnectivityContext(scanner);
    }

    @AfterEach
    public void cleanUpGlobalConnectionStatus() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
    }

    private static AvailableHardware hardwareWith(PortResult... ports) {
        return new AvailableHardware(asList(ports), false, false, false);
    }

    private static AvailableHardware dfuOnly() {
        return new AvailableHardware(Collections.emptyList(), true, false, false);
    }

    @Test
    public void startsDisconnectedAndDeliversSnapshotToLateSubscriber() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        assertEquals(SessionState.DISCONNECTED, manager.getState());

        List<SessionState> observed = new ArrayList<>();
        manager.addListener((state, hardware) -> observed.add(state));
        assertEquals(1, observed.size(), "subscribing must deliver the current snapshot immediately");
        assertEquals(SessionState.DISCONNECTED, observed.get(0));
    }

    @Test
    public void initialPortIsPreCachedSoScannerNeverProbesTheLivePort() {
        PortResult live = new PortResult("COM7", SerialPortType.EcuWithOpenblt);
        new DeviceSessionManager(connectivityContext, live);
        assertEquals(1, scanner.cachedPorts.size());
        assertSame(live, scanner.cachedPorts.get(0));
    }

    @Test
    public void openBltPortAppearingMovesSessionToDeviceInBlt() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);

        List<AvailableHardware> observedHardware = new ArrayList<>();
        manager.addListener((state, hardware) -> observedHardware.add(hardware));

        AvailableHardware blt = hardwareWith(new PortResult("COM3", SerialPortType.OpenBlt));
        scanner.fireHardwareChange(blt);

        assertEquals(SessionState.DEVICE_IN_BLT, manager.getState());
        assertSame(blt, observedHardware.get(observedHardware.size() - 1),
            "listeners must receive the hardware snapshot that produced the state");
    }

    @Test
    public void dfuDeviceMovesSessionToDeviceInDfu() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        scanner.fireHardwareChange(dfuOnly());
        assertEquals(SessionState.DEVICE_IN_DFU, manager.getState());
    }

    @Test
    public void openBltWinsOverDfuWhenBothDetected() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        scanner.fireHardwareChange(new AvailableHardware(
            Collections.singletonList(new PortResult("COM3", SerialPortType.OpenBlt)), true, false, false));
        assertEquals(SessionState.DEVICE_IN_BLT, manager.getState());
    }

    @Test
    public void boardDisappearingReturnsSessionToDisconnected() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        scanner.fireHardwareChange(hardwareWith(new PortResult("COM3", SerialPortType.OpenBlt)));
        assertEquals(SessionState.DEVICE_IN_BLT, manager.getState());

        scanner.fireHardwareChange(NOTHING);
        assertEquals(SessionState.DISCONNECTED, manager.getState());
    }

    @Test
    public void unknownAndPlainEcuPortsAloneDoNotClaimBootloaderStates() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        scanner.fireHardwareChange(hardwareWith(
            new PortResult("COM1", SerialPortType.Unknown),
            new PortResult("COM2", SerialPortType.Ecu)));
        assertEquals(SessionState.DISCONNECTED, manager.getState());
    }

    @Test
    public void connectionStatusDrivesConnectingAndConnected() {
        PortResult live = new PortResult("COM7", SerialPortType.EcuWithOpenblt);
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, live);
        scanner.cachedPorts.clear();

        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.LOADING);
        assertEquals(SessionState.CONNECTING, manager.getState());

        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        assertEquals(SessionState.CONNECTED, manager.getState());
        // On (re)connect the session port must be re-cached so the always-on scanner never
        // re-probes the port the live BinaryProtocol connection is reading.
        assertTrue(scanner.cachedPorts.contains(live));
    }

    @Test
    public void runningEcuWithOpenbltSupportIsNotABootloaderDevice() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        // EcuWithOpenblt is a *running* ECU that merely supports the bootloader — only an actual
        // OpenBlt port means the board is sitting in the bootloader.
        scanner.fireHardwareChange(hardwareWith(new PortResult("COM5", SerialPortType.EcuWithOpenblt)));
        assertEquals(SessionState.DISCONNECTED, manager.getState());
    }

    @Test
    public void debugProbesAloneDoNotChangeSessionState() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        // ST-Link/PCAN are developer probes, not a board state; only DFU means "device in bootloader"
        scanner.fireHardwareChange(new AvailableHardware(Collections.emptyList(), false, true, true));
        assertEquals(SessionState.DISCONNECTED, manager.getState());
    }

    @Test
    public void liveConnectionWinsOverBootloaderDetection() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        scanner.fireHardwareChange(hardwareWith(new PortResult("COM3", SerialPortType.OpenBlt)));
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        assertEquals(SessionState.CONNECTED, manager.getState());
    }

    // ---- FLASHING state + watchdog/scanner choreography around a flash job ----

    /** Scripted {@link DeviceSessionManager.JobExecutor}: fires the same listeners production wires. */
    private static class FakeJobExecutor implements DeviceSessionManager.JobExecutor {
        boolean inProgress;
        final List<Runnable> aboutToStart = new ArrayList<>();
        final List<Runnable> finished = new ArrayList<>();

        @Override
        public boolean isNotInProgress() {
            return !inProgress;
        }

        @Override
        public void addOnJobAboutToStartListener(Runnable listener) {
            aboutToStart.add(listener);
        }

        @Override
        public void addOnJobInProgressFinishedListener(Runnable listener) {
            finished.add(listener);
        }

        void startJob() {
            inProgress = true;
            aboutToStart.forEach(Runnable::run);
        }

        void finishJob() {
            inProgress = false;
            finished.forEach(Runnable::run);
        }
    }

    private final List<String> watchdogEvents = new ArrayList<>();

    private DeviceSessionManager managerWithWatchdogHooks() {
        return new DeviceSessionManager(connectivityContext, null,
            () -> watchdogEvents.add("pause"), () -> watchdogEvents.add("resume"));
    }

    @Test
    public void flashJobDrivesSessionToFlashingAndPausesWatchdog() {
        DeviceSessionManager manager = managerWithWatchdogHooks();
        FakeJobExecutor jobExecutor = new FakeJobExecutor();
        manager.setJobExecutor(jobExecutor);

        // FLASHING must win even over a live connection: the flasher owns the port now
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        jobExecutor.startJob();

        assertEquals(SessionState.FLASHING, manager.getState());
        assertEquals(Collections.singletonList("pause"), watchdogEvents,
            "the watchdog must be paused so it does not race the flasher for the port");
    }

    @Test
    public void jobFinishResumesScannerForcesRescanAndResumesWatchdog() {
        DeviceSessionManager manager = managerWithWatchdogHooks();
        FakeJobExecutor jobExecutor = new FakeJobExecutor();
        manager.setJobExecutor(jobExecutor);
        jobExecutor.startJob();

        jobExecutor.finishJob();

        assertEquals(SessionState.DISCONNECTED, manager.getState(), "job over — state derives from reality again");
        assertEquals(1, scanner.resumeCount, "the scanner must resume immediately after the flash");
        assertEquals(1, scanner.restartTimerCount, "a fresh scan cycle must be forced to detect the post-flash board state");
        assertEquals(asList("pause", "resume"), watchdogEvents);

        // the forced rescan is what lets the UI offer retry when the board is still in the bootloader
        scanner.fireHardwareChange(hardwareWith(new PortResult("COM9", SerialPortType.OpenBlt)));
        assertEquals(SessionState.DEVICE_IN_BLT, manager.getState());
    }

    @Test
    public void executorWithJobAlreadyRunningReportsFlashingImmediately() {
        DeviceSessionManager manager = managerWithWatchdogHooks();
        FakeJobExecutor jobExecutor = new FakeJobExecutor();
        jobExecutor.inProgress = true;

        manager.setJobExecutor(jobExecutor);

        assertEquals(SessionState.FLASHING, manager.getState());
    }

    @Test
    public void nullExecutorIsAcceptedAndJustRefreshes() {
        DeviceSessionManager manager = managerWithWatchdogHooks();
        manager.setJobExecutor(null);
        assertEquals(SessionState.DISCONNECTED, manager.getState());
        assertTrue(watchdogEvents.isEmpty());
    }
}
