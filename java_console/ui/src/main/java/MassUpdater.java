import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.ProductionConnectivity;
import com.rusefi.SerialPortType;
import com.rusefi.UiVersion;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.widgets.ToolButtons;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.SerialPortType.*;

public class MassUpdater {
    private static final Logging log = getLogging(MassUpdater.class);


    private final StatusWindow mainStatus = new StatusWindow();
    private final Set<String> knownPorts = new HashSet<>();

    private final AtomicBoolean flashInProgress = new AtomicBoolean();
    private boolean multipleWarned;
    private final ConnectivityContext connectivityContext;

    public MassUpdater(ConnectivityContext connectivityContext, SerialPortType target) {
        this.connectivityContext = connectivityContext;
        mainStatus.showFrame("Mass Updater " + UiVersion.CONSOLE_VERSION);

        final AtomicBoolean previousDfuState = new AtomicBoolean();
        AtomicBoolean isUsingDfu = new AtomicBoolean(); // it seems like DFU detection is not 100% reliable? a work-around to avoid double-DFU

        connectivityContext.getPortScanner().addListener(currentHardware -> {

            if (!isUsingDfu.get() && currentHardware.isDfuFound() != previousDfuState.get()) {
                mainStatus.getContent().logLine(currentHardware.isDfuFound() ? "I see a DFU device!" : "No DFU...");
                if (currentHardware.isDfuFound()) {
                    isUsingDfu.set(true);
                    UpdateOperationCallbacks releaseSemaphore = new UpdateOperationCallbacks() {
                        @Override
                        public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {

                        }

                        @Override
                        public void done() {
                            isUsingDfu.set(false);
                        }

                        @Override
                        public void error() {
                            isUsingDfu.set(false);
                        }

                        @Override
                        public void warning() {
                        }

                        @Override
                        public void clear() {}
                    };
                    SwingUtilities.invokeLater(() -> AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(
                        new DfuManualJob(connectivityContext.getConnectedEcuTarget()),
                        releaseSemaphore,
                        () -> {}
                    ));
                }
                previousDfuState.set(currentHardware.isDfuFound());
            }

            List<PortResult> currentFilteredList = currentHardware.getKnownPorts().stream().filter(portResult -> portResult.type == target).collect(Collectors.toList());

            if (EcuWithOpenblt == target) {
                handleAutoTarget(currentFilteredList);
                return;
            }

            Set<String> currentSet = currentFilteredList.stream().map(portResult -> portResult.port).collect(Collectors.toSet());
            for (Iterator<String> it = knownPorts.iterator(); it.hasNext(); ) {
                String port = it.next();
                if (!currentSet.contains(port)) {
                    mainStatus.getContent().logLine(port + ": No longer present");
                    it.remove();
                }
            }
            for (PortResult openPort : currentFilteredList) {
                if (!knownPorts.contains(openPort.port)) {
                    knownPorts.add(openPort.port);
                    mainStatus.getContent().logLine("New port " + openPort);

                    OpenBltManualJob job = OpenBltManualJobFactory.createProduction(openPort, mainStatus.getContent(), connectivityContext);
                    SwingUtilities.invokeLater(() -> AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(job));
                }
            }
        });
    }

    /**
     * Auto (EcuWithOpenblt) targets are flashed one board at a time. While a flash is running we
     * freeze detection entirely so the transient reboot-to-OpenBLT churn cannot trigger a re-flash.
     * When several ECUs are connected we refuse to guess which one to flash and ask the user to
     * unplug all but one.
     */
    private void handleAutoTarget(final List<PortResult> candidates) {
        if (flashInProgress.get()) {
            return;
        }

        knownPorts.retainAll(candidates.stream().map(p -> p.port).collect(Collectors.toSet()));

        if (candidates.size() > 1) {
            if (!multipleWarned) {
                mainStatus.getContent().logLine("Multiple ECUs detected (" + candidates.stream()
                    .map(p -> p.port).collect(Collectors.joining(", ")) + "). Please unplug all but one to start flashing.");
                multipleWarned = true;
            }
            return;
        }
        multipleWarned = false;

        if (candidates.isEmpty()) {
            return;
        }

        final PortResult openPort = candidates.get(0);
        if (knownPorts.contains(openPort.port)) {
            return; // already flashed this board — unplug it to flash the next one
        }
        knownPorts.add(openPort.port);
        mainStatus.getContent().logLine("New port " + openPort);
        flashInProgress.set(true);
        final Thread t = new Thread(() -> runAutoJob(openPort), "MassUpdater-flash");
        t.setDaemon(true);
        t.start();
    }

    /**
     * Connects a fresh LinkManager to the ECU, then runs the auto flash job synchronously on this
     * worker thread. Because {@link OpenBltAutoJob#doJob} does all its work on the calling thread,
     * returning from it means the board is fully flashed, its calibrations restored and its
     * connection re-established.
     */
    private void runAutoJob(final PortResult openPort) {
        try {
            final LinkManager lm = new LinkManager(connectivityContext.getConnectedEcuTarget())
                .setNeedPullText(false)
                .setNeedPullLiveData(true);
            final CountDownLatch connected = new CountDownLatch(1);
            final AtomicBoolean connectionOk = new AtomicBoolean();
            lm.startAndConnect(openPort.port, new ConnectionStatusLogic.Listener() {
                @Override
                public void onConnectionStatus(boolean isConnected) {
                }

                @Override
                public void onConnectionEstablished() {
                    connectionOk.set(true);
                    connected.countDown();
                }

                @Override
                public void onConnectionFailed(String s) {
                    mainStatus.getContent().logLine(openPort.port + ": connection failed: " + s);
                    connected.countDown();
                }
            });

            try {
                if (!connected.await(1, TimeUnit.MINUTES) || !connectionOk.get()) {
                    // Leave the port in knownPorts so we don't hot-loop retrying — unplug/replug to retry.
                    mainStatus.getContent().logLine(openPort.port + ": could not connect, skipping");
                    lm.close();
                    return;
                }
            } catch (final InterruptedException e) {
                Thread.currentThread().interrupt();
                lm.close();
                return;
            }

            final OpenBltAutoJob job = new OpenBltAutoJob(openPort, mainStatus.getContent(), connectivityContext, lm);
            final UpdateOperationCallbacks callbacks = createStatusWindow(job.getName());
            // Synchronous: blocks this worker thread until the whole flash sequence is done.
            job.doJob(callbacks, () -> {});
        } finally {
            flashInProgress.set(false);
        }
    }

    private static UpdateOperationCallbacks createStatusWindow(final String title) {
        final UpdateOperationCallbacks[] holder = new UpdateOperationCallbacks[1];
        try {
            SwingUtilities.invokeAndWait(() -> holder[0] = StatusWindow.createAndShowFrame(title + " " + UiVersion.CONSOLE_VERSION));
        } catch (final InterruptedException e) {
            Thread.currentThread().interrupt();
        } catch (final InvocationTargetException e) {
            log.error("createStatusWindow", e);
        }
        return holder[0];
    }

    public static void main(String[] args) throws InterruptedException, InvocationTargetException {
        SerialPortType target = getTargetFromArguments(args);
        log.error("Target port type: " + target);

        ToolButtons.showDeviceManager();
        SwingUtilities.invokeAndWait(() -> new MassUpdater(ProductionConnectivity.CONTEXT, target));
    }

    private static @NotNull SerialPortType getTargetFromArguments(String[] args) {
        SerialPortType target = OpenBlt;
        if (args.length > 0) {
            String arg = args[0];
            for (SerialPortType type : SerialPortType.values()) {
                if (type.name().equalsIgnoreCase(arg)) {
                    target = type;
                    break;
                }
            }
        }
        return target;
    }
}
