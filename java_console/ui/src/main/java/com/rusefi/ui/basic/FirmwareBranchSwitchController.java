package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.FirmwareBranch;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.FirmwareSelection;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.DfuAutoJob;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import com.rusefi.ui.wizard.FirmwareBranchSwitchPanel;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.Objects;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Consumer;
import java.util.function.Supplier;

/** Explicitly switches a connected Lima or Lima-Dev ECU to a development firmware branch. */
public final class FirmwareBranchSwitchController {
    public interface BranchSwitchJobMarker { }

    interface JobFactory {
        AsyncJob create(PortResult port, String signature, String branch, UpdateOperationCallbacks callbacks);
    }

    interface JobStarter {
        void start(AsyncJob job);
    }
    private final ConnectivityContext connectivityContext;
    private final UpdateOperationCallbacks callbacks;
    private final SingleAsyncJobExecutor executor;
    private final Supplier<Optional<PortResult>> portProvider;
    private final Supplier<Boolean> connected;
    private final Consumer<JComponent> showPanel;
    private final Runnable closePanel;
    private final JobFactory jobFactory;
    private final JobStarter jobStarter;
    private final Supplier<String> liveSignatureProvider;
    private final JButton button = new JButton("Switch firmware branch");
    private volatile @Nullable LinkManager linkManager;
    private long generation;

    public FirmwareBranchSwitchController(ConnectivityContext connectivityContext,
                                          UpdateOperationCallbacks callbacks,
                                          SingleAsyncJobExecutor executor,
                                          Supplier<Optional<PortResult>> portProvider,
                                          Supplier<Boolean> connected,
                                          Consumer<JComponent> showPanel,
                                          Runnable closePanel) {
        this(connectivityContext, callbacks, executor, portProvider, connected, showPanel, closePanel,
            null, null, null);
    }

    FirmwareBranchSwitchController(ConnectivityContext connectivityContext,
                                   UpdateOperationCallbacks callbacks,
                                   SingleAsyncJobExecutor executor,
                                   Supplier<Optional<PortResult>> portProvider,
                                   Supplier<Boolean> connected,
                                   Consumer<JComponent> showPanel,
                                   Runnable closePanel,
                                   JobFactory jobFactory,
                                   JobStarter jobStarter,
                                   Supplier<String> liveSignatureProvider) {
        this.connectivityContext = connectivityContext;
        this.callbacks = callbacks;
        this.executor = executor;
        this.portProvider = portProvider;
        this.connected = connected;
        this.showPanel = showPanel;
        this.closePanel = closePanel;
        this.jobFactory = jobFactory == null ? this::createProductionJob : jobFactory;
        this.jobStarter = jobStarter == null ? job -> executor.startJob(job, button) : jobStarter;
        this.liveSignatureProvider = liveSignatureProvider == null ? this::readLiveSignature : liveSignatureProvider;
        button.addActionListener(e -> openChooser());
        executor.addOnJobReleasedListener(this::refreshButton);
        refreshButton();
    }

    public JButton getButton() { return button; }

    public void setLinkManager(@Nullable LinkManager linkManager) {
        this.linkManager = linkManager;
        refreshButton();
    }

    public void refreshButton() {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(this::refreshButton);
            return;
        }
        boolean eligible = currentSnapshot() != null && executor.isNotInProgress();
        button.setVisible(eligible);
        button.setEnabled(eligible);
    }

    private void openChooser() {
        final Snapshot snapshot = currentSnapshot();
        if (snapshot == null || !executor.isNotInProgress()) {
            refreshButton();
            return;
        }
        final long opened = ++generation;
        showPanel.accept(new FirmwareBranchSwitchPanel(
            label(SignatureHelper.parse(snapshot.signature).getBranch()),
            branch -> confirmSwitch(opened, snapshot, branch),
            () -> cancel(opened)));
    }

    private void confirmSwitch(long opened, Snapshot expected, String targetBranch) {
        if (opened != generation) {
            return;
        }
        ++generation;
        closePanel.run();
        if (!isCurrent(expected, true)
            || !FirmwareBranch.canSwitch(SignatureHelper.parse(expected.signature).getBranch(), targetBranch)
            || !executor.isNotInProgress()) {
            callbacks.logLine("ECU changed or is busy; branch switch cancelled.");
            refreshButton();
            return;
        }
        jobStarter.start(new BranchSwitchJob(expected.port, expected.signature, targetBranch));
        refreshButton();
    }

    private void cancel(long opened) {
        if (opened == generation) {
            ++generation;
            closePanel.run();
        }
    }

    private @Nullable Snapshot currentSnapshot() {
        PortResult port = portProvider.get().orElse(null);
        if (port == null || (port.type != SerialPortType.Ecu && port.type != SerialPortType.EcuWithOpenblt)
            || LinkManager.isSlcanPort(port.port)
            || (port.type == SerialPortType.Ecu && LinkManager.isCanPort(port.port))) {
            return null;
        }
        LinkManager lm = linkManager;
        BinaryProtocol bp = lm == null ? null : lm.getBinaryProtocol();
        if (bp == null || bp.isClosed() || !port.port.equals(lm.getLastTriedPort())
            || !connected.get()) {
            return null;
        }
        String signature = liveSignatureProvider.get();
        return isSwitchSourceSignature(signature) ? new Snapshot(port, signature) : null;
    }

    private boolean isCurrent(Snapshot expected, boolean requireConnected) {
        AtomicBoolean result = new AtomicBoolean();
        Runnable check = () -> {
            PortResult current = portProvider.get().orElse(null);
            LinkManager lm = linkManager;
            BinaryProtocol bp = lm == null ? null : lm.getBinaryProtocol();
            boolean live = bp != null && !bp.isClosed()
                && expected.port.port.equals(lm.getLastTriedPort())
                && Objects.equals(expected.signature, liveSignatureProvider.get());
            result.set(current != null && expected.port.port.equals(current.port)
                && expected.port.type == current.type && live
                && (!requireConnected || connected.get()));
        };
        if (SwingUtilities.isEventDispatchThread()) {
            check.run();
        } else {
            try {
                SwingUtilities.invokeAndWait(check);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                return false;
            } catch (java.lang.reflect.InvocationTargetException e) {
                return false;
            }
        }
        return result.get();
    }

    private static boolean isSwitchSourceSignature(@Nullable String signature) {
        com.rusefi.core.RusEfiSignature parsed = SignatureHelper.parse(signature);
        return parsed != null && FirmwareBranch.canSwitch(parsed.getBranch(), "development");
    }

    private @Nullable String readLiveSignature() {
        LinkManager lm = linkManager;
        BinaryProtocol bp = lm == null ? null : lm.getBinaryProtocol();
        return bp == null || bp.isClosed() ? null : bp.signature;
    }

    private static final class Snapshot {
        final PortResult port;
        final String signature;

        Snapshot(PortResult port, String signature) {
            this.port = port;
            this.signature = signature;
        }
    }

    public static boolean isBranchSwitchJob(AsyncJob job) {
        return job instanceof BranchSwitchJobMarker;
    }

    private final class BranchSwitchJob extends AsyncJob implements BranchSwitchJobMarker {
        private final PortResult expectedPort;
        private final String expectedSignature;
        private final String targetBranch;
        private AsyncJob selectedJob;

        private BranchSwitchJob(PortResult expectedPort, String expectedSignature, String targetBranch) {
            super("Switch firmware branch to " + label(targetBranch));
            this.expectedPort = expectedPort;
            this.expectedSignature = expectedSignature;
            this.targetBranch = targetBranch;
        }

        @Override
        public boolean prepare(UpdateOperationCallbacks callbacks) {
            if (!validateCurrent(true)) {
                callbacks.logLine("ECU changed before branch switch; operation cancelled.");
                return false;
            }
            selectedJob = jobFactory.create(expectedPort, expectedSignature, targetBranch, callbacks);
            if (selectedJob == null) {
                callbacks.logLine("Firmware selection failed; operation cancelled.");
                return false;
            }
            if (!validateCurrent(true)) {
                callbacks.logLine("ECU changed or firmware selection failed; operation cancelled.");
                return false;
            }
            if (!selectedJob.prepare(callbacks) || !validateCurrent(true)) {
                callbacks.logLine("ECU changed during firmware download; operation cancelled.");
                return false;
            }
            return true;
        }

        @Override
        public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
            if (selectedJob == null || !validateCurrent(false)) {
                callbacks.logLine("ECU changed before branch switch; operation cancelled.");
                callbacks.error();
                onJobFinished.run();
                return;
            }
            selectedJob.doJob(callbacks, onJobFinished);
        }

        private boolean validateCurrent(boolean requireConnected) {
            return isCurrent(new Snapshot(expectedPort, expectedSignature), requireConnected);
        }
    }

    private AsyncJob createProductionJob(PortResult port, String signature, String branch,
                                         UpdateOperationCallbacks callbacks) {
        FirmwareSelection selection = FirmwareSelection.latest(signature, branch, true, callbacks);
        if (selection == null) {
            return null;
        }
        LinkManager lm = linkManager;
        if (port.type == SerialPortType.Ecu) {
            return new DfuAutoJob(port, button, connectivityContext, lm, null,
                CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION, selection);
        }
        return new OpenBltAutoJob(port, button, connectivityContext, lm, null,
            CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION, selection);
    }

    static String label(String branch) {
        if ("lts-26lima".equals(branch)) {
            return "Lima";
        }
        return "lts-26lima-dev".equals(branch) ? "Lima-Dev" : "Dev";
    }
}
