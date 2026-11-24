package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.autoupdate.Autoupdate.DevFirmwareMetadataException;
import com.rusefi.autoupdate.Autoupdate.DevFirmwareMetadataFailure;
import com.rusefi.autoupdate.FirmwareRollbackResolver;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.FirmwareBranch;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.FirmwareSelection;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.DfuAutoJob;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import com.rusefi.ui.wizard.FirmwareRollbackPanel;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.time.DateTimeException;
import java.time.Instant;
import java.time.LocalDate;
import java.time.ZoneOffset;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Objects;
import java.util.Optional;
import java.util.Properties;
import java.util.function.BooleanSupplier;
import java.util.function.BiFunction;
import java.util.function.Consumer;
import java.util.function.LongSupplier;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;

/** Shared LTS firmware history action used by both startup and live Device UIs. */
public final class FirmwareRollbackController {
    private static final Logging log = getLogging(FirmwareRollbackController.class);
    private static final long DEV_METADATA_CACHE_TTL_MILLIS = 5 * 60 * 1000;
    private static final long DEV_METADATA_RETRY_DELAY_MILLIS = 10_000;

    interface DevRefreshTask {
        void cancel();
    }

    @FunctionalInterface
    interface DevRefreshScheduler {
        DevRefreshTask schedule(int delayMillis, Runnable task);
    }

    private final ConnectivityContext connectivityContext;
    private final UpdateOperationCallbacks callbacks;
    private final SingleAsyncJobExecutor jobExecutor;
    private final Supplier<Optional<PortResult>> portProvider;
    private final BooleanSupplier externallyIdle;
    private final Runnable stateChanged;
    private final Consumer<JComponent> showPicker;
    private final Runnable closePicker;
    private final List<Runnable> stateChangedListeners = new ArrayList<>();
    private final FirmwareRollbackResolver resolver = new FirmwareRollbackResolver(
        PropertiesHolder.getFirmwareRollbackRootUrl());
    @FunctionalInterface
    interface CatalogLoader {
        List<FirmwareRollbackResolver.Build> discover(String board, String branch) throws IOException;
    }
    private CatalogLoader catalogLoader = resolver::discover;
    @FunctionalInterface
    interface FirmwareDownloader {
        Optional<Autoupdate.FirmwareArtifacts> download(
            FirmwareRollbackResolver.Build build,
            ConnectionAndMeta.DownloadProgressListener progress,
            Consumer<String> logger
        );
    }
    private FirmwareDownloader firmwareDownloader =
        (build, progress, logger) -> Optional.of(Autoupdate.downloadFirmware(build, progress, logger));
    @FunctionalInterface
    interface DevMetadataLoader {
        long getLastModified(String board, String branch) throws IOException;
    }
    private DevMetadataLoader devMetadataLoader = Autoupdate::getDevFirmwareLastModified;
    private final boolean enabled = isFirmwareRollbackEnabled(PropertiesHolder.INSTANCE.getProperties());
    private final JButton rollbackButton = new JButton("Rollback Firmware");

    private volatile List<FirmwareRollbackResolver.Build> builds = Collections.emptyList();
    private volatile String catalogKey;
    private volatile String discoveryKey;
    private volatile long discoveryGeneration;
    private volatile boolean discoveryInProgress;
    private volatile boolean releaseStatusDiscoveryEnabled;
    private volatile DevLookupKey devLookupKey;
    private volatile long devLookupGeneration;
    private volatile boolean devLookupInProgress;
    private volatile boolean devMetadataChecked;
    private volatile long devLastModified;
    private volatile DevFirmwareMetadataFailure devMetadataFailure;
    private volatile long devMetadataCheckedAt;
    private volatile long devRetryAfter;
    private volatile DevStatusSnapshot devStatusSnapshot;
    private LongSupplier devClock = System::currentTimeMillis;
    private DevRefreshScheduler devRefreshScheduler = (delayMillis, task) -> {
        Timer timer = new Timer(delayMillis, event -> task.run());
        timer.setRepeats(false);
        timer.start();
        return timer::stop;
    };
    private DevRefreshTask devRefreshTask;
    private long devRefreshGeneration;
    private boolean devStatusViewActive;
    private volatile boolean disposed;
    private volatile long discoveryRetryAfter;
    private volatile String discoveryRetryKey;
    private volatile boolean downloadInProgress;
    private volatile FirmwareRollbackResolver.Build pendingInstalledBuild;
    private volatile @Nullable String sessionInstalledSha;
    private volatile @Nullable LinkManager linkManager;
    private volatile @Nullable String lastDevIdentityDiagnostic;
    private volatile @Nullable PortResult selectedPortSnapshot;

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

        rollbackButton.setVisible(false);
        rollbackButton.setMargin(new Insets(com.formdev.flatlaf.util.UIScale.scale(6),
            com.formdev.flatlaf.util.UIScale.scale(10), com.formdev.flatlaf.util.UIScale.scale(6),
            com.formdev.flatlaf.util.UIScale.scale(10)));
        rollbackButton.addActionListener(event -> showRollbackPicker());
        jobExecutor.addOnJobInProgressFinishedListener(() -> {
            if (jobExecutor.getLastResult() == UpdateFirmwareResult.SUCCESS && pendingInstalledBuild != null) {
                sessionInstalledSha = pendingInstalledBuild.getSha();
                PersistentConfiguration.getConfig().getRoot().setProperty(
                    lastInstalledKey(pendingInstalledBuild.getBoard(), pendingInstalledBuild.getBranch()),
                    pendingInstalledBuild.getSha());
            }
            pendingInstalledBuild = null;
        });
        jobExecutor.addOnJobReleasedListener(this::notifyStateChanged);
    }

    public JButton getRollbackButton() {
        return rollbackButton;
    }

    public void setLinkManager(@Nullable LinkManager linkManager) {
        this.linkManager = linkManager;
    }

    public @Nullable BundleInfo getConnectedFirmware() {
        return connectedFirmware();
    }

    public void addStateChangedListener(Runnable listener) {
        stateChangedListeners.add(listener);
    }

    public boolean isBusy() {
        return discoveryInProgress || downloadInProgress;
    }

    public boolean isDownloadInProgress() {
        return downloadInProgress;
    }

    public boolean isPrimaryUpdateBlockedByCatalogDiscovery() {
        return enabled && discoveryInProgress;
    }

    public void enableReleaseStatusDiscovery() {
        releaseStatusDiscoveryEnabled = true;
    }

    /** Status is deliberately unknown unless the connected build is ordered in its exact branch catalog. */
    public String getReleaseStatus() {
        BundleInfo firmware = releaseStatusFirmware();
        if (firmware == null) {
            return "Disconnected — connect to inspect ECU firmware";
        }
        String signature = releaseStatusSignature();
        PortResult selectedPort = currentSelectedPortForStatus();
        boolean missingIniIdentity = signature != null && connectedSignature() == null
            && Objects.equals(signature, missingIniSignature(selectedPort));
        String key = firmware.getTarget() + "/" + firmware.getBranchName();
        if (isDevelopmentBundle(firmware)) {
            return getDevelopmentReleaseStatus();
        }
        if (!isLtsBundle(firmware)) {
            return releaseStatus(firmware.getBranchName(), false, false, false, false, false, false);
        }
        LinkManager lm = linkManager;
        if (!missingIniIdentity && !matchesExpectedUpdateTarget(selectedPort, selectedPort, hasLiveConnection(),
            lm == null ? null : lm.getLastTriedPort())) {
            return "Couldn't check for updates. Reconnect the ECU and try again.";
        }
        if (discoveryInProgress && key.equals(discoveryKey)) {
            return releaseStatus(firmware.getBranchName(), true, true, false, false, false, false);
        }
        if (!key.equals(catalogKey) || builds.isEmpty()) {
            return releaseStatus(firmware.getBranchName(), true, false, false, false, false, false);
        }
        Optional<String> observedSha = Optional.ofNullable(selectedPort).flatMap(PortResult::getFirmwareHash);
        if (!observedSha.isPresent()) {
            return signatureMatchedReleaseStatus(firmware, signature);
        }
        FirmwareRollbackResolver.Build installed = builds.stream()
            .filter(build -> sameReleaseIdentity(firmware.getTarget(), firmware.getBranchName(), observedSha.get(),
                build.getBoard(), build.getBranch(), build.getSha()))
            .findFirst().orElse(null);
        FirmwareRollbackResolver.Build latest = resolver.latest(builds).orElse(null);
        boolean matched = installed != null && latest != null;
        boolean latestUnique = latest != null && builds.stream()
            .filter(build -> build.getLastModified() == latest.getLastModified()).count() == 1;
        boolean atLatest = matched && latestUnique && latest.getSha().equalsIgnoreCase(installed.getSha());
        boolean newer = matched && latestUnique && latest.getLastModified() > installed.getLastModified();
        return releaseStatus(firmware.getBranchName(), true, false, true, matched, atLatest, newer);
    }

    /** Display data from the same live ECU/catalog snapshot used by the release-status decision. */
    public ReleaseDisplay getReleaseDisplay() {
        BundleInfo firmware = releaseStatusFirmware();
        String status = getReleaseStatus();
        LocalDate installedDate = parseSignatureDate(SignatureHelper.parse(releaseStatusSignature()));
        LocalDate latestDate = null;
        if (firmware != null && isLtsBundle(firmware) && firmware.getTarget() != null
            && (firmware.getTarget() + "/" + firmware.getBranchName()).equals(catalogKey)) {
            List<FirmwareRollbackResolver.Build> scopedBuilds = new ArrayList<>();
            for (FirmwareRollbackResolver.Build build : builds) {
                if (sameReleaseIdentity(firmware.getTarget(), firmware.getBranchName(),
                    build.getBoard(), build.getBranch())) {
                    scopedBuilds.add(build);
                }
            }
            FirmwareRollbackResolver.Build latest = resolver.latest(scopedBuilds).orElse(null);
            if (latest != null && scopedBuilds.stream()
                .filter(build -> build.getLastModified() == latest.getLastModified()).count() == 1) {
                RusEfiSignature signature = SignatureHelper.parse(latest.getFirmwareSignature());
                LocalDate date = parseSignatureDate(signature);
                if (signature != null && sameReleaseIdentity(firmware.getTarget(), firmware.getBranchName(),
                    signature.getBundleTarget(), signature.getBranch()) && date != null
                    && !date.isAfter(LocalDate.now(ZoneOffset.UTC))) {
                    latestDate = date;
                }
            }
        } else if (firmware != null && isDevelopmentBundle(firmware)) {
            DevStatusSnapshot snapshot = currentDevStatusSnapshot();
            if (snapshot != null && snapshot.key != null && snapshot.key.equals(devLookupKey)
                && devMetadataChecked && devLastModified > 0) {
                try {
                    LocalDate date = Instant.ofEpochMilli(devLastModified).atZone(ZoneOffset.UTC).toLocalDate();
                    if (!date.isAfter(LocalDate.now(ZoneOffset.UTC))) {
                        latestDate = date;
                    }
                } catch (DateTimeException ignored) {
                    latestDate = null;
                }
            }
        }
        return new ReleaseDisplay(status, installedDate, latestDate);
    }

    public @Nullable String getConnectedPortLabel() {
        PortResult selected = currentSelectedPortForStatus();
        LinkManager lm = linkManager;
        if (!matchesExpectedUpdateTarget(selected, selected, hasLiveConnection(),
            lm == null ? null : lm.getLastTriedPort())) {
            return null;
        }
        String port = selected.port;
        return port.startsWith("/dev/") ? port.substring("/dev/".length()) : port;
    }

    public static final class ReleaseDisplay {
        public final String status;
        public final @Nullable LocalDate installedDate;
        public final @Nullable LocalDate latestDate;

        private ReleaseDisplay(String status, @Nullable LocalDate installedDate, @Nullable LocalDate latestDate) {
            this.status = status;
            this.installedDate = installedDate;
            this.latestDate = latestDate;
        }
    }

    private String signatureMatchedReleaseStatus(BundleInfo firmware, @Nullable String connectedSignature) {
        if (connectedSignature == null) {
            return "Couldn't check for updates. Try again later.";
        }
        List<FirmwareRollbackResolver.Build> scopedBuilds = new ArrayList<>();
        for (FirmwareRollbackResolver.Build build : builds) {
            if (sameReleaseIdentity(firmware.getTarget(), firmware.getBranchName(), build.getBoard(),
                build.getBranch())) {
                scopedBuilds.add(build);
            }
        }
        List<FirmwareRollbackResolver.Build> matches = new ArrayList<>();
        for (FirmwareRollbackResolver.Build build : scopedBuilds) {
            if (connectedSignature.equals(build.getFirmwareSignature())) {
                matches.add(build);
            }
        }
        if (matches.size() != 1) {
            return "Couldn't check for updates. Try again later.";
        }
        FirmwareRollbackResolver.Build installed = matches.get(0);
        FirmwareRollbackResolver.Build latest = resolver.latest(scopedBuilds).orElse(null);
        if (latest == null || scopedBuilds.stream()
            .filter(build -> build.getLastModified() == latest.getLastModified()).count() != 1) {
            return "Couldn't compare firmware versions.";
        }
        if (latest.getSha().equalsIgnoreCase(installed.getSha())) {
            return "This ECU is up to date.";
        }
        if (latest.getLastModified() > installed.getLastModified()) {
            return "Update available";
        }
        return "Couldn't compare firmware versions.";
    }

    static String releaseStatus(String branch, boolean enabled, boolean checking, boolean catalogAvailable,
                                boolean buildMatched, boolean atLatest, boolean newerBuildOrdered) {
        if (!enabled) {
            return "Couldn't check for updates. Try again later.";
        }
        if (checking) {
            return "Checking for updates…";
        }
        if (!catalogAvailable) {
            return "Firmware update information is unavailable.";
        }
        if (!buildMatched) {
            return "Couldn't check for updates. Try again later.";
        }
        if (atLatest) {
            return "This ECU is up to date.";
        }
        if (newerBuildOrdered) {
            return "Update available";
        }
        return "Couldn't compare firmware versions.";
    }

    static boolean sameReleaseIdentity(String ecuBoard, String ecuBranch, String ecuSha,
                                       String buildBoard, String buildBranch, String buildSha) {
        return ecuBoard != null && ecuBranch != null && ecuSha != null
            && ecuBoard.equalsIgnoreCase(buildBoard)
            && ecuBranch.equalsIgnoreCase(buildBranch)
            && ecuSha.equalsIgnoreCase(buildSha);
    }

    private static boolean sameReleaseIdentity(String ecuBoard, String ecuBranch, String buildBoard,
                                               String buildBranch) {
        return ecuBoard != null && ecuBranch != null
            && ecuBoard.equalsIgnoreCase(buildBoard)
            && ecuBranch.equalsIgnoreCase(buildBranch);
    }

    public boolean startLatestUpdate(JComponent source) {
        final Optional<PortResult> port = portProvider.get();
        return port.isPresent() && startLatestUpdate(source, port.get());
    }

    /** A confirmed UI selection must not be replaced by the startup controller's independent provider. */
    public boolean startLatestUpdate(JComponent source, PortResult expectedPort) {
        if (!enabled || !isLtsBundle(connectedFirmware()) || !isSerialFirmwareUpdateSupported(expectedPort)) {
            return false;
        }
        final Optional<PortResult> port = portProvider.get();
        final LinkManager lm = linkManager;
        if (!matchesExpectedUpdateTarget(expectedPort, port.orElse(null),
            hasLiveConnection(), lm == null ? null : lm.getLastTriedPort())) {
            callbacks.logLine("ECU selection changed; update cancelled.");
            return true; // Consumed, not a request to fall back to another update path.
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
        startFirmwareJob(builds, CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION, source, true, expectedPort);
        return true;
    }

    public void refresh(@Nullable PortResult port) {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(() -> refresh(port));
            return;
        }
        if (disposed) {
            return;
        }
        selectedPortSnapshot = port;
        if (!isSerialFirmwareUpdateSupported(port)) {
            reset();
            return;
        }
        discover(port);
        refreshButton();
    }

    public void reset() {
        cancelDevRefreshTask();
        synchronized (this) {
            builds = Collections.emptyList();
            catalogKey = null;
            sessionInstalledSha = null;
            discoveryKey = null;
            discoveryGeneration++;
            discoveryInProgress = false;
            discoveryRetryAfter = 0;
            discoveryRetryKey = null;
            devLookupGeneration++;
            devLookupKey = null;
            devLookupInProgress = false;
            devMetadataChecked = false;
            devLastModified = 0;
            devMetadataFailure = null;
            devMetadataCheckedAt = 0;
            devRetryAfter = 0;
            devStatusSnapshot = null;
            selectedPortSnapshot = null;
        }
        if (SwingUtilities.isEventDispatchThread()) {
            refreshButton();
        } else {
            SwingUtilities.invokeLater(this::refreshButton);
        }
    }

    public void refreshButton() {
        final Optional<PortResult> port = portProvider.get();
        final boolean visible = isRollbackButtonVisible(enabled, connectedFirmware())
            && (!port.isPresent() || !LinkManager.SOCKET_CAN.equals(port.get().port));
        final boolean available = visible
            && port.isPresent()
            && isSerialFirmwareUpdateSupported(port.get())
            && hasLiveConnection()
            && !resolver.rollbackCandidates(builds, currentFirmwareSha(port.get())).isEmpty();
        if (rollbackButton.isVisible() != visible) {
            rollbackButton.setVisible(visible);
            if (rollbackButton.getParent() != null) {
                rollbackButton.getParent().revalidate();
            }
        }
        rollbackButton.setEnabled(available && externallyIdle.getAsBoolean()
            && jobExecutor.isNotInProgress() && !downloadInProgress);
    }

    private void showRollbackPicker() {
        final Optional<PortResult> port = portProvider.get();
        if (!externallyIdle.getAsBoolean() || !hasLiveConnection() || !port.isPresent()
            || !isSerialFirmwareUpdateSupported(port.get())) {
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
        final BundleInfo bundle = releaseStatusFirmware();
        if ((!enabled && !releaseStatusDiscoveryEnabled)) {
            reset();
            return;
        }
        if (isDevelopmentBundle(bundle)) {
            discoverDevelopmentMetadata(port);
            return;
        }
        if (!isLtsBundle(bundle)) {
            reset();
            return;
        }
        if (!isSerialFirmwareUpdateSupported(port)) {
            return;
        }

        final String board = bundle.getTarget();
        final String branch = bundle.getBranchName();
        final String key = board + "/" + branch;
        final long generation;
        synchronized (this) {
            if (key.equals(catalogKey) || (discoveryInProgress && key.equals(discoveryKey))
                || (key.equals(discoveryRetryKey) && System.currentTimeMillis() < discoveryRetryAfter)) {
                return;
            }
            builds = Collections.emptyList();
            sessionInstalledSha = null;
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
                discovered = catalogLoader.discover(board, branch);
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

    private void discoverDevelopmentMetadata(PortResult port) {
        DevStatusSnapshot snapshot = captureDevStatusSnapshot(port);
        devStatusSnapshot = snapshot;
        if (snapshot.key == null) {
            clearDevelopmentLookup();
            notifyStateChanged();
            return;
        }
        DevLookupKey key = snapshot.key;
        final long generation;
        synchronized (this) {
            long now = devClock.getAsLong();
            boolean cached = devMetadataChecked && now >= devMetadataCheckedAt
                && now - devMetadataCheckedAt < DEV_METADATA_CACHE_TTL_MILLIS;
            boolean sameKey = key.equals(devLookupKey);
            if (sameKey && devLookupInProgress) {
                return;
            }
            if (sameKey && cached) {
                scheduleDevRefresh((int) (DEV_METADATA_CACHE_TTL_MILLIS - (now - devMetadataCheckedAt)));
                return;
            }
            if (sameKey && now < devRetryAfter) {
                scheduleDevRefresh((int) (devRetryAfter - now));
                return;
            }
            cancelDevRefreshTask();
            devLookupKey = key;
            devLookupGeneration++;
            generation = devLookupGeneration;
            devLookupInProgress = true;
            devMetadataChecked = false;
            devLastModified = 0;
            devMetadataFailure = null;
            devMetadataCheckedAt = 0;
            devRetryAfter = 0;
        }
        notifyStateChanged();
        Thread lookup = new Thread(() -> {
            long published = 0;
            boolean success = false;
            DevFirmwareMetadataFailure failure = DevFirmwareMetadataFailure.NO_PUBLICATION_DATE;
            try {
                published = devMetadataLoader.getLastModified(key.board, BundleInfo.DEVELOPMENT);
                success = published > 0;
            } catch (Exception e) {
                log.warn("Failed to inspect Dev firmware metadata for " + key.board + ": " + e);
                failure = e instanceof DevFirmwareMetadataException
                    ? ((DevFirmwareMetadataException) e).getFailure()
                    : DevFirmwareMetadataFailure.CONNECTION_FAILED;
            }
            final long completedLastModified = published;
            final boolean completedSuccessfully = success;
            final DevFirmwareMetadataFailure completedFailure = failure;
            SwingUtilities.invokeLater(() -> finishDevelopmentLookup(
                key, generation, completedLastModified, completedSuccessfully, completedFailure));
        }, "dev-firmware-metadata");
        lookup.setDaemon(true);
        lookup.start();
    }

    private void clearDevelopmentLookup() {
        cancelDevRefreshTask();
        synchronized (this) {
            devLookupGeneration++;
            devLookupKey = null;
            devLookupInProgress = false;
            devMetadataChecked = false;
            devLastModified = 0;
            devMetadataFailure = null;
            devMetadataCheckedAt = 0;
        }
    }

    private void finishDevelopmentLookup(DevLookupKey requestKey, long generation, long published, boolean success,
                                         DevFirmwareMetadataFailure failure) {
        if (disposed) {
            return;
        }
        DevStatusSnapshot current = captureDevStatusSnapshot(null);
        devStatusSnapshot = current;
        boolean accepted = false;
        synchronized (this) {
            if (generation == devLookupGeneration && requestKey.equals(devLookupKey)) {
                accepted = requestKey.equals(current.key);
                if (requestKey.equals(current.key)) {
                    devLastModified = published;
                    devMetadataChecked = success;
                    devMetadataFailure = success ? null : failure;
                    devMetadataCheckedAt = success ? devClock.getAsLong() : 0;
                    devRetryAfter = success ? 0 : devClock.getAsLong() + DEV_METADATA_RETRY_DELAY_MILLIS;
                } else {
                    devLookupKey = null;
                    devMetadataChecked = false;
                    devLastModified = 0;
                    devMetadataCheckedAt = 0;
                    devMetadataFailure = null;
                }
                devLookupInProgress = false;
            }
        }
        if (accepted && devStatusViewActive && !disposed) {
            scheduleDevRefresh((int) (success ? DEV_METADATA_CACHE_TTL_MILLIS : DEV_METADATA_RETRY_DELAY_MILLIS));
        }
        notifyStateChanged();
    }

    private void scheduleDevRefresh(int delayMillis) {
        cancelDevRefreshTask();
        if (!devStatusViewActive || disposed || delayMillis <= 0) {
            return;
        }
        long generation = devRefreshGeneration;
        devRefreshTask = devRefreshScheduler.schedule(delayMillis, () -> {
            if (generation != devRefreshGeneration) {
                return;
            }
            devRefreshTask = null;
            if (devStatusViewActive && !disposed) {
                refresh(portProvider.get().orElse(null));
            }
        });
    }

    private void cancelDevRefreshTask() {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(this::cancelDevRefreshTask);
            return;
        }
        devRefreshGeneration++;
        if (devRefreshTask != null) {
            devRefreshTask.cancel();
            devRefreshTask = null;
        }
    }

    private DevStatusSnapshot captureDevStatusSnapshot(@Nullable PortResult expectedPort) {
        LinkManager lm = linkManager;
        String signature = releaseStatusSignature();
        BundleInfo firmware = releaseStatusFirmware();
        Optional<PortResult> selected = portProvider.get();
        String livePort = lm == null ? null : lm.getLastTriedPort();
        PortResult selectedPort = selected.orElse(null);
        boolean matches = selectedPort != null
            && (expectedPort == null || expectedPort.equals(selectedPort))
            && matchesReleaseStatusTarget(selectedPort, signature, livePort)
            && isDevelopmentBundle(firmware);
        DevLookupKey key = matches
            ? new DevLookupKey(firmware.getTarget(), BundleInfo.DEVELOPMENT,
                selectedPort, signature, livePort, lm)
            : null;
        return new DevStatusSnapshot(selectedPort, signature, livePort, lm, matches, key);
    }

    private static final class DevStatusSnapshot {
        private final PortResult selectedPort;
        private final String signature;
        private final String livePort;
        private final LinkManager linkManager;
        private final boolean matchesLiveEcu;
        private final @Nullable DevLookupKey key;

        private DevStatusSnapshot(PortResult selectedPort, String signature, String livePort,
                                  LinkManager linkManager, boolean matchesLiveEcu, @Nullable DevLookupKey key) {
            this.selectedPort = selectedPort;
            this.signature = signature;
            this.livePort = livePort;
            this.linkManager = linkManager;
            this.matchesLiveEcu = matchesLiveEcu;
            this.key = key;
        }
    }

    private String getDevelopmentReleaseStatus() {
        RusEfiSignature signature = SignatureHelper.parse(releaseStatusSignature());
        DevStatusSnapshot snapshot = currentDevStatusSnapshot();
        if (snapshot == null) {
            return "Couldn't check for updates. Reconnect the ECU and try again.";
        }
        lastDevIdentityDiagnostic = null;
        DevLookupKey currentKey = snapshot.key;
        if (currentKey == null || !currentKey.equals(devLookupKey)) {
            return "Firmware update information is unavailable.";
        }
        if (devLookupInProgress) {
            return "Checking for updates…";
        }
        if (!devMetadataChecked) {
            return developmentMetadataFailureStatus(devMetadataFailure);
        }
        LocalDate ecuDate = parseSignatureDate(signature);
        LocalDate publishedDate;
        try {
            publishedDate = Instant.ofEpochMilli(devLastModified).atZone(ZoneOffset.UTC).toLocalDate();
        } catch (DateTimeException e) {
            return "Couldn't compare firmware versions.";
        }
        LocalDate today = LocalDate.now(ZoneOffset.UTC);
        if (ecuDate == null || ecuDate.isAfter(today) || publishedDate.isAfter(today)) {
            return "Couldn't compare firmware versions.";
        }
        if (publishedDate.isAfter(ecuDate)) {
            return "Update available";
        }
        if (publishedDate.equals(ecuDate)) {
            return "No newer build date found.";
        }
        return "No newer update found.";
    }

    /** The Dev status and displayed publication date must both belong to the same live identity snapshot. */
    private @Nullable DevStatusSnapshot currentDevStatusSnapshot() {
        DevStatusSnapshot snapshot = devStatusSnapshot;
        String connectedSignature = releaseStatusSignature();
        String snapshotLivePort = snapshot == null || snapshot.linkManager == null
            ? null : snapshot.linkManager.getLastTriedPort();
        boolean sessionConnected = externallyIdle.getAsBoolean();
        boolean liveTargetMatches = snapshot != null
            && matchesReleaseStatusTarget(snapshot.selectedPort, snapshot.signature, snapshotLivePort);
        if (snapshot == null || !snapshot.matchesLiveEcu
            || !Objects.equals(snapshot.signature, connectedSignature)
            || !Objects.equals(snapshot.livePort, snapshotLivePort)
            || !liveTargetMatches) {
            logDevIdentityMismatch(snapshot, connectedSignature, snapshotLivePort, sessionConnected, liveTargetMatches);
            return null;
        }
        return snapshot;
    }

    static String developmentMetadataFailureStatus(DevFirmwareMetadataFailure failure) {
        if (failure == DevFirmwareMetadataFailure.DOWNLOAD_NOT_FOUND) {
            return "Couldn't check for updates. No firmware download was found for this ECU.";
        }
        if (failure == DevFirmwareMetadataFailure.NO_PUBLICATION_DATE) {
            return "Couldn't compare versions because the latest firmware has no publication date.";
        }
        if (failure == DevFirmwareMetadataFailure.INVALID_SERVER_RESPONSE) {
            return "Couldn't check for updates. Try again later.";
        }
        return "Couldn't reach the update server. Check your network and try again.";
    }

    private void logDevIdentityMismatch(@Nullable DevStatusSnapshot snapshot, @Nullable String signature,
                                       @Nullable String snapshotLivePort, boolean sessionConnected,
                                       boolean liveTargetMatches) {
        LinkManager currentLinkManager = linkManager;
        BinaryProtocol protocol = currentLinkManager == null ? null : currentLinkManager.getBinaryProtocol();
        boolean protocolClosed = protocol == null || protocol.isClosed();
        boolean selectionReadable = SwingUtilities.isEventDispatchThread();
        PortResult currentSelected = selectionReadable ? portProvider.get().orElse(null) : null;
        String currentSelectedPort = currentSelected == null ? null : currentSelected.port;
        SerialPortType currentSelectedType = currentSelected == null ? null : currentSelected.type;
        String currentLivePort = currentLinkManager == null ? null : currentLinkManager.getLastTriedPort();
        String snapshotSelectedPort = snapshot == null || snapshot.selectedPort == null
            ? null : snapshot.selectedPort.port;
        SerialPortType snapshotSelectedType = snapshot == null || snapshot.selectedPort == null
            ? null : snapshot.selectedPort.type;
        boolean snapshotSignatureMatches = snapshot != null && Objects.equals(snapshot.signature, signature);
        boolean snapshotManagerIsCurrent = snapshot != null && snapshot.linkManager == currentLinkManager;
        boolean snapshotLivePortMatches = snapshot != null && Objects.equals(snapshot.livePort, snapshotLivePort);
        String diagnostic = "currentSelectedPort=" + diagnosticValue(currentSelectedPort)
            + ", currentSelectedType=" + currentSelectedType
            + ", currentLivePort=" + diagnosticValue(currentLivePort)
            + ", snapshotPresent=" + (snapshot != null)
            + ", snapshotSelectedPort=" + diagnosticValue(snapshotSelectedPort)
            + ", snapshotSelectedType=" + snapshotSelectedType
            + ", snapshotLivePort=" + diagnosticValue(snapshotLivePort)
            + ", selectionReadable=" + selectionReadable
            + ", protocolClosed=" + protocolClosed
            + ", sessionConnected=" + sessionConnected
            + ", disposed=" + disposed + ", statusViewActive=" + devStatusViewActive
            + ", signaturePresent=" + (signature != null)
            + ", snapshotMatched=" + (snapshot != null && snapshot.matchesLiveEcu)
            + ", snapshotSignatureMatches=" + snapshotSignatureMatches
            + ", snapshotLivePortMatches=" + snapshotLivePortMatches
            + ", snapshotManagerIsCurrent=" + snapshotManagerIsCurrent
            + ", liveTargetMatches=" + liveTargetMatches;
        if (!diagnostic.equals(lastDevIdentityDiagnostic)) {
            lastDevIdentityDiagnostic = diagnostic;
            log.warn("Dev release status identity guard rejected: " + diagnostic);
        }
    }

    private static String diagnosticValue(@Nullable String value) {
        if (value == null) {
            return "null";
        }
        String sanitized = value.replaceAll("[\\r\\n\\p{Cntrl}]", "?");
        return sanitized.length() <= 80 ? sanitized : sanitized.substring(0, 80);
    }

    static @Nullable LocalDate parseSignatureDate(@Nullable RusEfiSignature signature) {
        if (signature == null || signature.getYear() == null || signature.getMonth() == null || signature.getDay() == null
            || !signature.getYear().matches("\\d{4}")
            || !signature.getMonth().matches("\\d{1,2}") || !signature.getDay().matches("\\d{1,2}")) {
            return null;
        }
        try {
            return LocalDate.of(Integer.parseInt(signature.getYear()),
                Integer.parseInt(signature.getMonth()), Integer.parseInt(signature.getDay()));
        } catch (DateTimeException | NumberFormatException e) {
            return null;
        }
    }

    static boolean isDevelopmentBundle(@Nullable BundleInfo bundle) {
        return bundle != null && !BundleInfo.isUndefined(bundle)
            && FirmwareBranch.isSameBranch(bundle.getBranchName(), BundleInfo.DEVELOPMENT);
    }

    private static final class DevLookupKey {
        private final String board;
        private final String branch;
        private final String port;
        private final SerialPortType portType;
        private final String signature;
        private final String livePort;
        private final LinkManager linkManager;

        private DevLookupKey(String board, String branch, PortResult selectedPort, String signature,
                             String livePort, LinkManager linkManager) {
            this.board = board;
            this.branch = branch;
            this.port = selectedPort.port;
            this.portType = selectedPort.type;
            this.signature = signature;
            this.livePort = livePort;
            this.linkManager = linkManager;
        }

        @Override
        public boolean equals(Object other) {
            if (!(other instanceof DevLookupKey)) {
                return false;
            }
            DevLookupKey key = (DevLookupKey) other;
            return board.equalsIgnoreCase(key.board) && branch.equals(key.branch) && port.equals(key.port)
                && portType == key.portType && Objects.equals(signature, key.signature)
                && Objects.equals(livePort, key.livePort) && linkManager == key.linkManager;
        }

        @Override
        public int hashCode() {
            return Objects.hash(board.toLowerCase(java.util.Locale.ROOT), branch, port, portType, signature,
                livePort, System.identityHashCode(linkManager));
        }
    }

    private void startFirmwareJob(
        final List<FirmwareRollbackResolver.Build> candidates,
        final CalibrationsHelper.FirmwareUpdatePolicy policy,
        final JComponent source,
        final boolean tryNextBuild
    ) {
        startFirmwareJob(candidates, policy, source, tryNextBuild, null);
    }

    private void startFirmwareJob(
        final List<FirmwareRollbackResolver.Build> candidates,
        final CalibrationsHelper.FirmwareUpdatePolicy policy,
        final JComponent source,
        final boolean tryNextBuild,
        @Nullable PortResult expectedPort
    ) {
        final Optional<PortResult> port = portProvider.get();
        final LinkManager lm = linkManager;
        if (!port.isPresent() || (expectedPort != null && !matchesExpectedUpdateTarget(expectedPort, port.get(),
            hasLiveConnection(), lm == null ? null : lm.getLastTriedPort()))) {
            return;
        }
        if (!jobExecutor.isNotInProgress() || !externallyIdle.getAsBoolean()) {
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
        final String requestedSignature = connectedSignature();
        notifyStateChanged();
        callbacks.clear();
        callbacks.updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.DOWNLOAD, 0);
        final Thread download = new Thread(() -> {
            AsyncJob selectedJob = null;
            FirmwareRollbackResolver.Build selectedBuild = null;
            for (int i = 0; i < candidates.size(); i++) {
                FirmwareRollbackResolver.Build candidate = candidates.get(i);
                final int candidateStart = 100 * i / candidates.size();
                final int candidateWidth = 100 * (i + 1) / candidates.size() - candidateStart;
                final Optional<Autoupdate.FirmwareArtifacts> artifacts = firmwareDownloader.download(
                    candidate,
                    percent -> callbacks.updateFirmwareProgress(
                        UpdateOperationCallbacks.FirmwareUpdateStage.DOWNLOAD,
                        candidateStart + candidateWidth * percent / 100),
                    callbacks::logLine);
                callbacks.updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.DOWNLOAD,
                    candidateStart + candidateWidth);
                selectedJob = artifacts.map(value ->
                    createFirmwareJob(port.get(), requestedSignature, value, policy, source)).orElse(null);
                if (selectedJob != null || !tryNextBuild) {
                    selectedBuild = selectedJob == null ? null : candidate;
                    break;
                }
                callbacks.logLine("Skipping incomplete firmware build " + candidate.getSha().substring(0, 10));
            }
            final AsyncJob job = selectedJob;
            final FirmwareRollbackResolver.Build build = selectedBuild;
            SwingUtilities.invokeLater(() -> acceptDownloadedJob(
                port.get(), requestedSignature, requestedCatalogKey, requestedGeneration, job, build, source));
        }, "lts-firmware-download");
        download.setDaemon(true);
        download.start();
    }

    private void acceptDownloadedJob(
        PortResult requestedPort,
        String requestedSignature,
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
            || !isSerialFirmwareUpdateSupported(currentPort.get())
            || !externallyIdle.getAsBoolean()
            || !hasLiveConnection()
            || linkManager == null || !currentPort.get().port.equals(linkManager.getLastTriedPort())
            || !Objects.equals(requestedSignature, connectedSignature())
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
        if (!jobExecutor.startJobContinuingProgress(job, source, () -> pendingInstalledBuild = build)) {
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
        @Nullable String requestedSignature,
        Autoupdate.FirmwareArtifacts artifacts,
        CalibrationsHelper.FirmwareUpdatePolicy policy,
        JComponent source
    ) {
        if (!isSerialFirmwareUpdateSupported(port)) {
            return null;
        }
        FirmwareSelection selection = FirmwareSelection.fromArtifacts(
            requestedSignature, artifacts.getBundleInfo(), artifacts, false);
        if (selection == null) {
            return null;
        }
        if (port.type == SerialPortType.Ecu) {
            return artifacts.getBinPath()
                .map(path -> (AsyncJob) new DfuAutoJob(
                    port, source, connectivityContext, linkManager, path, policy, selection))
                .orElse(null);
        }
        if (port.type == SerialPortType.EcuWithOpenblt) {
            return artifacts.getSrecPath()
                .map(path -> (AsyncJob) new OpenBltAutoJob(
                    port, source, connectivityContext, linkManager, path, policy, selection))
                .orElse(null);
        }
        return null;
    }

    static boolean isSerialFirmwareUpdateSupported(@Nullable PortResult port) {
        return port != null && port.isEcu() && !LinkManager.isCanPort(port.port);
    }

    static boolean matchesExpectedUpdateTarget(PortResult expected, @Nullable PortResult current,
                                             boolean liveConnection, @Nullable String connectedPort) {
        return isSerialFirmwareUpdateSupported(expected) && isSerialFirmwareUpdateSupported(current)
            && expected.type == current.type && expected.port.equals(current.port)
            && liveConnection && expected.port.equals(connectedPort);
    }

    private String currentFirmwareSha(final PortResult port) {
        final BundleInfo bundle = connectedFirmware();
        if (bundle == null) {
            return null;
        }
        final String board = bundle.getTarget();
        return selectCurrentFirmwareSha(
            sessionInstalledSha,
            port.getFirmwareHash(),
            PersistentConfiguration.getConfig().getRoot().getProperty(
                lastInstalledKey(board, bundle.getBranchName()), null));
    }

    static @Nullable String selectCurrentFirmwareSha(
        @Nullable String sessionInstalledSha,
        Optional<String> portFirmwareSha,
        @Nullable String persistedFirmwareSha
    ) {
        if (sessionInstalledSha != null && !sessionInstalledSha.trim().isEmpty()) {
            return sessionInstalledSha.trim();
        }
        if (portFirmwareSha.isPresent() && !portFirmwareSha.get().trim().isEmpty()) {
            return portFirmwareSha.get().trim();
        }
        return persistedFirmwareSha;
    }

    private boolean hasLiveConnection() {
        return connectedSignature() != null;
    }

    private static boolean containsSha(List<FirmwareRollbackResolver.Build> builds, @Nullable String sha) {
        return sha != null && builds.stream().anyMatch(build -> build.getSha().equalsIgnoreCase(sha));
    }

    private static String lastInstalledKey(String board, String branch) {
        return "lts_last_flashed_sha." + board + "." + branch;
    }

    private @Nullable BundleInfo connectedFirmware() {
        return connectedFirmware(connectedSignature());
    }

    private @Nullable String connectedSignature() {
        LinkManager lm = linkManager;
        BinaryProtocol bp = lm == null ? null : lm.getBinaryProtocol();
        return bp == null || bp.isClosed() ? null : bp.signature;
    }

    private @Nullable String releaseStatusSignature() {
        String liveSignature = connectedSignature();
        if (liveSignature != null) {
            return liveSignature;
        }
        return missingIniSignature(currentSelectedPortForStatus());
    }

    private @Nullable PortResult currentSelectedPortForStatus() {
        return SwingUtilities.isEventDispatchThread()
            ? portProvider.get().orElse(null) : selectedPortSnapshot;
    }

    private @Nullable BundleInfo releaseStatusFirmware() {
        BundleInfo liveFirmware = connectedFirmware();
        return liveFirmware != null ? liveFirmware : connectedFirmware(releaseStatusSignature());
    }

    private @Nullable String missingIniSignature(@Nullable PortResult selectedPort) {
        com.rusefi.AvailableHardware hardware = connectivityContext.getCurrentHardware();
        if (selectedPort == null
            || (selectedPort.type != SerialPortType.Ecu && selectedPort.type != SerialPortType.EcuWithOpenblt)
            || LinkManager.isCanPort(selectedPort.port)
            || hardware == null || hardware.getKnownPorts().stream().noneMatch(port ->
                port.type == selectedPort.type && port.port.equals(selectedPort.port))) {
            return null;
        }
        com.rusefi.MissingIniRecoveryState.Failure failure =
            connectivityContext.getMissingIniRecoveryState().get();
        return failure != null && selectedPort.port.equals(failure.port)
            && SignatureHelper.parse(failure.signature) != null ? failure.signature : null;
    }

    private boolean matchesReleaseStatusTarget(@Nullable PortResult selectedPort, @Nullable String signature,
                                               @Nullable String livePort) {
        if (selectedPort == null || signature == null) {
            return false;
        }
        return hasLiveConnection()
            ? matchesExpectedUpdateTarget(selectedPort, selectedPort, true, livePort)
            : Objects.equals(signature, missingIniSignature(selectedPort));
    }

    static @Nullable BundleInfo connectedFirmware(@Nullable String signature) {
        RusEfiSignature parsed = SignatureHelper.parse(signature);
        return parsed == null ? null : parsed.asBundleInfo();
    }

    void setCatalogLoaderForTests(CatalogLoader loader) {
        catalogLoader = loader;
    }

    void setFirmwareDownloaderForTests(FirmwareDownloader downloader) {
        firmwareDownloader = downloader;
    }

    /** Cross-package seam for exercising a live Device card with deterministic LTS history. */
    public void setCatalogProviderForTests(BiFunction<String, String, List<FirmwareRollbackResolver.Build>> provider) {
        catalogLoader = provider::apply;
    }

    void setDevMetadataLoaderForTests(DevMetadataLoader loader) {
        devMetadataLoader = loader;
    }

    /** Cross-package test seam for exercising the Device summary with deterministic release metadata. */
    public void setDevMetadataProviderForTests(BiFunction<String, String, Long> provider) {
        devMetadataLoader = (board, branch) -> provider.apply(board, branch);
    }

    void setDevClockForTests(LongSupplier clock) {
        devClock = clock;
    }

    void setDevRefreshSchedulerForTests(DevRefreshScheduler scheduler) {
        devRefreshScheduler = scheduler;
    }

    void setDevStatusViewActive(boolean active) {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(() -> setDevStatusViewActive(active));
            return;
        }
        if (disposed || devStatusViewActive == active) {
            return;
        }
        devStatusViewActive = active;
        if (!active) {
            cancelDevRefreshTask();
        } else if (releaseStatusDiscoveryEnabled) {
            refresh(portProvider.get().orElse(null));
        }
    }

    void dispose() {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(this::dispose);
            return;
        }
        if (disposed) {
            return;
        }
        disposed = true;
        devStatusViewActive = false;
        cancelDevRefreshTask();
        synchronized (this) {
            devLookupGeneration++;
            devLookupInProgress = false;
            devLookupKey = null;
        }
    }

    static boolean isLtsBundle(BundleInfo bundle) {
        return bundle != null && !BundleInfo.isUndefined(bundle)
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
