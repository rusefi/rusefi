package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.autoupdate.FirmwareRollbackResolver;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.DfuAutoJob;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import com.rusefi.ui.wizard.FirmwareRollbackPanel;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.Properties;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;

/** Shared LTS firmware history action used by both startup and live Device UIs. */
public final class FirmwareRollbackController {
    private static final Logging log = getLogging(FirmwareRollbackController.class);

    private final ConnectivityContext connectivityContext;
    private final UpdateOperationCallbacks callbacks;
    private final SingleAsyncJobExecutor jobExecutor;
    private final Supplier<Optional<PortResult>> portProvider;
    private final BooleanSupplier externallyIdle;
    private final Runnable stateChanged;
    private final Consumer<JComponent> showPicker;
    private final Runnable closePicker;
    private final List<Runnable> stateChangedListeners = new ArrayList<>();
    private final FirmwareRollbackResolver resolver = new FirmwareRollbackResolver(PropertiesHolder.getBaseUrl());
    private final boolean enabled = isFirmwareRollbackEnabled(PropertiesHolder.INSTANCE.getProperties());
    private final JButton rollbackButton = new JButton(
        "Rollback Firmware", AutoupdateUtil.loadIcon("download48.jpg"));

    private volatile List<FirmwareRollbackResolver.Build> builds = Collections.emptyList();
    private volatile String catalogKey;
    private volatile String discoveryKey;
    private volatile long discoveryGeneration;
    private volatile boolean discoveryInProgress;
    private volatile long discoveryRetryAfter;
    private volatile String discoveryRetryKey;
    private volatile boolean downloadInProgress;
    private volatile FirmwareRollbackResolver.Build pendingInstalledBuild;
    private volatile @Nullable LinkManager linkManager;

    public FirmwareRollbackController(
        ConnectivityContext connectivityContext,
        UpdateOperationCallbacks callbacks,
        SingleAsyncJobExecutor jobExecutor,
        Supplier<Optional<PortResult>> portProvider,
        BooleanSupplier externallyIdle,
        Runnable stateChanged,
        Consumer<JComponent> showPicker,
        Runnable closePicker
    ) {
        this.connectivityContext = connectivityContext;
        this.callbacks = callbacks;
        this.jobExecutor = jobExecutor;
        this.portProvider = portProvider;
        this.externallyIdle = externallyIdle;
        this.stateChanged = stateChanged;
        this.showPicker = showPicker;
        this.closePicker = closePicker;

        rollbackButton.setVisible(isRollbackButtonVisible(enabled, BundleUtil.readBundleFullNameNotNull()));
        rollbackButton.addActionListener(event -> showRollbackPicker());
        jobExecutor.addOnJobInProgressFinishedListener(() -> SwingUtilities.invokeLater(() -> {
            if (jobExecutor.getLastResult() == UpdateFirmwareResult.SUCCESS && pendingInstalledBuild != null) {
                PersistentConfiguration.getConfig().getRoot().setProperty(
                    lastInstalledKey(pendingInstalledBuild.getBoard(), pendingInstalledBuild.getBranch()),
                    pendingInstalledBuild.getSha());
            }
            pendingInstalledBuild = null;
            notifyStateChanged();
        }));
    }

    public JButton getRollbackButton() {
        return rollbackButton;
    }

    public void setLinkManager(@Nullable LinkManager linkManager) {
        this.linkManager = linkManager;
    }

    public void addStateChangedListener(Runnable listener) {
        stateChangedListeners.add(listener);
    }

    public boolean isBusy() {
        return discoveryInProgress || downloadInProgress;
    }

    public boolean startLatestUpdate(JComponent source) {
        if (!enabled || !isLtsBundle()) {
            return false;
        }
        final Optional<PortResult> port = portProvider.get();
        if (!hasLiveConnection() || !port.isPresent() || !port.get().isEcu()) {
            return false;
        }
        if (isBusy()) {
            return true;
        }
        if (!externallyIdle.getAsBoolean()) {
            return true;
        }
        if (builds.isEmpty()) {
            return false;
        }
        startFirmwareJob(builds, CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION, source, true);
        return true;
    }

    public void refresh(@Nullable PortResult port) {
        if (port == null) {
            reset();
            return;
        }
        discover(port);
        refreshButton();
    }

    public void reset() {
        synchronized (this) {
            if (discoveryInProgress) {
                builds = Collections.emptyList();
                catalogKey = null;
            }
            discoveryKey = null;
            discoveryGeneration++;
            discoveryInProgress = false;
            discoveryRetryAfter = 0;
            discoveryRetryKey = null;
        }
        if (SwingUtilities.isEventDispatchThread()) {
            refreshButton();
        } else {
            SwingUtilities.invokeLater(this::refreshButton);
        }
    }

    public void refreshButton() {
        final Optional<PortResult> port = portProvider.get();
        final boolean visible = isRollbackButtonVisible(enabled, BundleUtil.readBundleFullNameNotNull());
        final boolean available = visible
            && port.isPresent()
            && port.get().isEcu()
            && hasLiveConnection()
            && !resolver.rollbackCandidates(builds, currentFirmwareSha(port.get())).isEmpty();
        rollbackButton.setVisible(visible);
        rollbackButton.setEnabled(available && externallyIdle.getAsBoolean()
            && jobExecutor.isNotInProgress() && !downloadInProgress);
    }

    private void showRollbackPicker() {
        final Optional<PortResult> port = portProvider.get();
        if (!externallyIdle.getAsBoolean() || !hasLiveConnection() || !port.isPresent() || !port.get().isEcu()) {
            return;
        }
        final String currentSha = currentFirmwareSha(port.get());
        final boolean currentKnown = containsSha(builds, currentSha);
        final List<FirmwareRollbackResolver.Build> candidates = resolver.rollbackCandidates(builds, currentSha);
        if (candidates.isEmpty()) {
            return;
        }

        final SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        final String[] labels = candidates.stream()
            .map(build -> format.format(new Date(build.getLastModified())) + "  " + build.getSha().substring(0, 10))
            .toArray(String[]::new);
        showPicker.accept(new FirmwareRollbackPanel(currentKnown, labels, selectedIndex -> {
            closePicker.run();
            startFirmwareJob(
                Collections.singletonList(candidates.get(selectedIndex)),
                CalibrationsHelper.FirmwareUpdatePolicy.ROLLBACK_RESTORE_OR_RESET,
                rollbackButton,
                false);
        }, closePicker));
    }

    private void discover(final PortResult port) {
        final BundleInfo bundle = BundleUtil.readBundleFullNameNotNull();
        if (!enabled || !isLtsBundle(bundle)) {
            builds = Collections.emptyList();
            return;
        }
        if (!port.isEcu()) {
            return;
        }

        final String board = connectivityContext.getConnectedEcuTarget().effectiveTarget();
        final String branch = bundle.getBranchName();
        final String key = board + "/" + branch;
        final long generation;
        synchronized (this) {
            if (key.equals(catalogKey) || (discoveryInProgress && key.equals(discoveryKey))
                || (key.equals(discoveryRetryKey) && System.currentTimeMillis() < discoveryRetryAfter)) {
                return;
            }
            builds = Collections.emptyList();
            catalogKey = key;
            discoveryKey = key;
            generation = ++discoveryGeneration;
            discoveryInProgress = true;
        }
        notifyStateChanged();

        final Thread discovery = new Thread(() -> {
            List<FirmwareRollbackResolver.Build> discovered = Collections.emptyList();
            boolean success = false;
            try {
                discovered = resolver.discover(board, branch);
                success = true;
                callbacks.logLine("Found " + discovered.size() + " LTS firmware builds for " + board);
            } catch (Exception e) {
                log.warn("Failed to discover LTS firmware builds at " + key + ": " + e);
                callbacks.logLine("LTS firmware history is unavailable: " + e.getMessage());
            }
            synchronized (FirmwareRollbackController.this) {
                if (generation == discoveryGeneration && key.equals(catalogKey)) {
                    builds = discovered;
                    if (!success) {
                        catalogKey = null;
                        discoveryRetryKey = key;
                        discoveryRetryAfter = System.currentTimeMillis() + 10_000;
                    } else {
                        discoveryRetryKey = null;
                    }
                }
                if (generation == discoveryGeneration && key.equals(discoveryKey)) {
                    discoveryInProgress = false;
                    discoveryKey = null;
                }
            }
            notifyStateChanged();
        }, "lts-firmware-discovery");
        discovery.setDaemon(true);
        discovery.start();
    }

    private void startFirmwareJob(
        final List<FirmwareRollbackResolver.Build> candidates,
        final CalibrationsHelper.FirmwareUpdatePolicy policy,
        final JComponent source,
        final boolean tryNextBuild
    ) {
        final Optional<PortResult> port = portProvider.get();
        if (!port.isPresent()) {
            return;
        }
        synchronized (this) {
            if (downloadInProgress) {
                return;
            }
            downloadInProgress = true;
        }
        final String requestedCatalogKey = catalogKey;
        final long requestedGeneration = discoveryGeneration;
        notifyStateChanged();
        final Thread download = new Thread(() -> {
            AsyncJob selectedJob = null;
            FirmwareRollbackResolver.Build selectedBuild = null;
            for (FirmwareRollbackResolver.Build candidate : candidates) {
                final Autoupdate.FirmwareArtifacts artifacts = Autoupdate.downloadFirmware(
                    candidate, callbacks::updateProgress, callbacks::logLine);
                selectedJob = createFirmwareJob(port.get(), artifacts, policy, source);
                if (selectedJob != null || !tryNextBuild) {
                    selectedBuild = selectedJob == null ? null : candidate;
                    break;
                }
                callbacks.logLine("Skipping incomplete firmware build " + candidate.getSha().substring(0, 10));
            }
            final AsyncJob job = selectedJob;
            final FirmwareRollbackResolver.Build build = selectedBuild;
            SwingUtilities.invokeLater(() -> acceptDownloadedJob(
                port.get(), requestedCatalogKey, requestedGeneration, job, build, source));
        }, "lts-firmware-download");
        download.setDaemon(true);
        download.start();
    }

    private void acceptDownloadedJob(
        PortResult requestedPort,
        String requestedCatalogKey,
        long requestedGeneration,
        @Nullable AsyncJob job,
        @Nullable FirmwareRollbackResolver.Build build,
        JComponent source
    ) {
        downloadInProgress = false;
        final Optional<PortResult> currentPort = portProvider.get();
        if (!currentPort.isPresent()
            || !currentPort.get().port.equals(requestedPort.port)
            || currentPort.get().type != requestedPort.type
            || !currentPort.get().isEcu()
            || !externallyIdle.getAsBoolean()
            || !hasLiveConnection()
            || requestedGeneration != discoveryGeneration
            || !Objects.equals(requestedCatalogKey, catalogKey)) {
            callbacks.logLine("ECU changed while firmware was downloading; update cancelled.");
            notifyStateChanged();
            return;
        }
        if (job == null || build == null) {
            callbacks.logLine("Selected firmware build has no artifact for this bootloader.");
            callbacks.error();
            notifyStateChanged();
            return;
        }
        if (!jobExecutor.startJob(job, source, () -> pendingInstalledBuild = build)) {
            notifyStateChanged();
        }
    }

    private void notifyStateChanged() {
        final Runnable notify = () -> {
            stateChanged.run();
            for (Runnable listener : stateChangedListeners) {
                listener.run();
            }
        };
        if (SwingUtilities.isEventDispatchThread()) {
            notify.run();
        } else {
            SwingUtilities.invokeLater(notify);
        }
    }

    private @Nullable AsyncJob createFirmwareJob(
        PortResult port,
        Autoupdate.FirmwareArtifacts artifacts,
        CalibrationsHelper.FirmwareUpdatePolicy policy,
        JComponent source
    ) {
        if (port.type == SerialPortType.Ecu) {
            return artifacts.getBinPath()
                .map(path -> (AsyncJob) new DfuAutoJob(
                    port, source, connectivityContext, linkManager, path, policy))
                .orElse(null);
        }
        if (port.type == SerialPortType.EcuWithOpenblt) {
            return artifacts.getSrecPath()
                .map(path -> (AsyncJob) new OpenBltAutoJob(
                    port, source, connectivityContext, linkManager, path, policy))
                .orElse(null);
        }
        return null;
    }

    private String currentFirmwareSha(final PortResult port) {
        if (port.getFirmwareHash().isPresent()) {
            final String hash = port.getFirmwareHash().get().trim();
            if (!hash.isEmpty()) {
                return hash;
            }
        }
        final BundleInfo bundle = BundleUtil.readBundleFullNameNotNull();
        final String board = connectivityContext.getConnectedEcuTarget().effectiveTarget();
        return PersistentConfiguration.getConfig().getRoot().getProperty(
            lastInstalledKey(board, bundle.getBranchName()), null);
    }

    private boolean hasLiveConnection() {
        final LinkManager lm = linkManager;
        return lm != null && lm.getBinaryProtocol() != null;
    }

    private static boolean containsSha(List<FirmwareRollbackResolver.Build> builds, @Nullable String sha) {
        return sha != null && builds.stream().anyMatch(build -> build.getSha().equalsIgnoreCase(sha));
    }

    private static String lastInstalledKey(String board, String branch) {
        return "lts_last_flashed_sha." + board + "." + branch;
    }

    private boolean isLtsBundle() {
        return isLtsBundle(BundleUtil.readBundleFullNameNotNull());
    }

    static boolean isLtsBundle(BundleInfo bundle) {
        return !BundleInfo.isUndefined(bundle)
            && !bundle.isMaster()
            && bundle.getBranchName() != null
            && bundle.getBranchName().startsWith("lts");
    }

    static boolean isRollbackButtonVisible(boolean enabled, BundleInfo bundle) {
        return enabled && isLtsBundle(bundle);
    }

    static boolean isFirmwareRollbackEnabled(Properties properties) {
        return Boolean.parseBoolean(System.getProperty(
            "RE_FIRMWARE_ROLLBACK_ENABLED",
            properties.getProperty("firmware_rollback_enabled", "false")));
    }
}
