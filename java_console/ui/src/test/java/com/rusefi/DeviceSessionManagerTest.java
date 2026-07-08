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
    public void liveConnectionWinsOverBootloaderDetection() {
        DeviceSessionManager manager = new DeviceSessionManager(connectivityContext, null);
        scanner.fireHardwareChange(hardwareWith(new PortResult("COM3", SerialPortType.OpenBlt)));
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        assertEquals(SessionState.CONNECTED, manager.getState());
    }
}
