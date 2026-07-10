package com.rusefi;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.ConnectionWatchdog;
import com.rusefi.io.LinkManager;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Window-independent owner of the "one console, all device states" session [tag:better_ux_for_flashing].
 * <p>
 * A single console instance must handle every board transition — hook, remove, hook-in-DFU,
 * hook-in-OpenBLT, re-connect — without a restart. To do that the {@link SerialPortScanner} is kept
 * alive for the whole console lifetime (instead of being stopped at splash → console handoff), and
 * this manager derives a single {@link SessionState} from {@link ConnectionStatusLogic} plus the most
 * recent {@link AvailableHardware} snapshot. UI (e.g. the live console's Device tab) subscribes and
 * reflects the current reality, offering connect / disconnect / DFU / OpenBLT as appropriate.
 * <p>
 * Threading: {@link Listener#onSessionChanged} may be invoked off the EDT (the scanner and the
 * connection-status pub/sub both fire from background threads). Swing consumers must marshal to the
 * EDT themselves.
 */
public class DeviceSessionManager {
    public interface Listener {
        void onSessionChanged(SessionState state, AvailableHardware hardware);
    }

    /**
     * The narrow slice of {@code com.rusefi.ui.basic.SingleAsyncJobExecutor} this session manager consumes, extracted so the
     * FLASHING state and the watchdog/scanner choreography can be unit-tested with a fake executor.
     */
    public interface JobExecutor {
        boolean isNotInProgress();

        void addOnJobAboutToStartListener(Runnable listener);

        void addOnJobInProgressFinishedListener(Runnable listener);
    }

    private final ConnectivityContext connectivityContext;
    private volatile JobExecutor jobExecutor;
    // ConnectionWatchdog is a static singleton; injected as hooks so tests can observe pause/resume.
    private final Runnable watchdogPause;
    private final Runnable watchdogResume;

    private final List<Listener> listeners = new CopyOnWriteArrayList<>();

    private volatile SessionState state = SessionState.DISCONNECTED;
    private volatile AvailableHardware currentHardware;
    // The board this session intends to be connected to; re-cached on CONNECTED so the always-on
    // scanner never re-probes the live port [tag:better_ux_for_flashing].
    private volatile PortResult sessionPort;

    public DeviceSessionManager(final ConnectivityContext connectivityContext,
                                final PortResult initialPort) {
        this(connectivityContext, initialPort, ConnectionWatchdog::pause, ConnectionWatchdog::resume);
    }

    public DeviceSessionManager(final ConnectivityContext connectivityContext,
                                final PortResult initialPort,
                                final Runnable watchdogPause,
                                final Runnable watchdogResume) {
        this.connectivityContext = connectivityContext;
        this.watchdogPause = watchdogPause;
        this.watchdogResume = watchdogResume;
        this.currentHardware = connectivityContext.getCurrentHardware();
        this.sessionPort = initialPort;
        this.state = computeState();

        // The console keeps the scanner alive for the whole session. Pre-cache the live-connected port
        // BEFORE the scanner is (re)started by addListener below, so the scanner never opens the port
        // that our BinaryProtocol connection is actively reading — that race hangs the live read on
        // Windows (mirrors the offline-tune cachePort in StartupFrame#onSplashConnected). [tag:offline_tune]
        if (initialPort != null && !LinkManager.isSpecialNotSerial(initialPort.port)) {
            connectivityContext.getPortScanner().cachePort(initialPort);
        }

        connectivityContext.getPortScanner().addListener(hardware -> {
            this.currentHardware = hardware;
            recomputeAndBroadcast();
        });
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> recomputeAndBroadcast());
    }

    /**
     * Wire the Device tab's firmware-job executor so the session can report {@link SessionState#FLASHING}
     * while a DFU/OpenBLT job owns the port. The executor is created by the Device tab (it owns the
     * status/progress sink), so it is injected after construction.
     */
    public void setJobExecutor(final JobExecutor jobExecutor) {
        this.jobExecutor = jobExecutor;
        if (jobExecutor != null) {
            // Pause the watchdog during a flash job so it does not race the flasher for the port. [tag:better_ux_for_flashing]
            jobExecutor.addOnJobAboutToStartListener(watchdogPause);
            jobExecutor.addOnJobAboutToStartListener(this::refresh);
            jobExecutor.addOnJobInProgressFinishedListener(() -> {
                // Flash finished (success or failure): immediately resume the scanner and force a fresh
                // scan so the UI can detect the board's post-flash state (OpenBLT/DFU) and offer retry
                // options. [tag:better_ux_for_flashing]
                final PortScanner scanner = connectivityContext.getPortScanner();
                scanner.resume();
                scanner.restartTimer();
                refresh();
                watchdogResume.run();
            });
        }
        refresh();
    }

    /**
     * Recompute the session state from the latest inputs and broadcast. Safe to call from any thread.
     */
    public void refresh() {
        recomputeAndBroadcast();
    }

    private SessionState computeState() {
        if (jobExecutor != null && !jobExecutor.isNotInProgress()) {
            return SessionState.FLASHING;
        }
        final ConnectionStatusValue connectionValue = ConnectionStatusLogic.INSTANCE.getValue();
        if (connectionValue == ConnectionStatusValue.CONNECTED) {
            return SessionState.CONNECTED;
        }
        if (connectionValue == ConnectionStatusValue.LOADING) {
            return SessionState.CONNECTING;
        }
        // NOT_CONNECTED: inspect the detected hardware for a board sitting in a bootloader state.
        final AvailableHardware hardware = currentHardware;
        if (hardware != null) {
            if (!hardware.getKnownPorts(SerialPortType.OpenBlt).isEmpty()) {
                return SessionState.DEVICE_IN_BLT;
            }
            if (hardware.isDfuFound()) {
                return SessionState.DEVICE_IN_DFU;
            }
        }
        return SessionState.DISCONNECTED;
    }

    private void recomputeAndBroadcast() {
        this.state = computeState();
        if (this.state == SessionState.CONNECTED) {
            // Re-cache the live port so the always-on scanner never re-probes it. The cache is cleared
            // when a port disappears (retainAll in the scanner), so a disconnect/replug reconnect must
            // re-cache here or the scanner races the live BinaryProtocol read and hangs on Windows. [tag:better_ux_for_flashing]
            final PortResult live = sessionPort;
            if (live != null && !LinkManager.isSpecialNotSerial(live.port)) {
                connectivityContext.getPortScanner().cachePort(live);
            }
        }
        // Always broadcast: even when the state label is unchanged the hardware snapshot may have
        // changed (a new port appeared/disappeared), and the Device tab drives its ports combo and
        // ProgramSelector menu off that snapshot.
        final SessionState snapshotState = this.state;
        final AvailableHardware snapshotHardware = this.currentHardware;
        for (final Listener listener : listeners) {
            listener.onSessionChanged(snapshotState, snapshotHardware);
        }
    }

    public void addListener(final Listener listener) {
        listeners.add(listener);
        // Deliver the current snapshot immediately so a late subscriber renders correctly.
        listener.onSessionChanged(state, currentHardware);
    }

    public SessionState getState() {
        return state;
    }

}
