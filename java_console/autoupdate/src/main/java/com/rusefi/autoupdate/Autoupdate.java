package com.rusefi.autoupdate;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.rusefi.AutoupdateProperty;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleInfoStrategy;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.FileUtil;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.core.ui.ErrorMessageHelper;
import org.jetbrains.annotations.NotNull;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.io.*;
import java.net.URI;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Date;
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.Predicate;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.FindFileHelper.findSrecFile;
import static com.rusefi.core.FindFileHelper.findFirmwareFile;

/**
 * Entry point of {@code rusefi_autoupdate.exe} and the coordinator of the console/firmware
 * auto-update user experience.
 * <p>
 * We've given up on complex classloader logic for auto-update and ended up with the
 * {@link #startConsoleAsANewProcess} approach: the updater JVM downloads and unpacks a fresh
 * bundle, then launches the real console as a brand-new process so classloading is never asked
 * to swap classes underneath a running JVM.
 *
 * <h2>User experience overview</h2>
 *
 * <h3>1. Settings that gate auto-update</h3>
 * Auto-update is controlled by <b>two</b> flags — {@link #isAutoUpdateEnabled} requires
 * BOTH to be true:
 * <ul>
 *   <li><b>{@link com.rusefi.AutoupdateProperty} (user preference)</b> — per-installation
 *       toggle persisted in the console's preferences. Users flip this to opt out of
 *       silent updates.</li>
 *   <li><b>{@code autoupdate_bundle} (bundle-level property in {@code shared_io.properties})</b>
 *       — set by whoever ships the bundle. A white-label bundle can hard-disable auto-update
 *       for its users regardless of the per-user preference.</li>
 * </ul>
 *
 * <h3>2. Caps Lock override at launch</h3>
 * When {@code rusefi_autoupdate.exe} starts, {@link #isSkipUpdater} checks the CAPS LOCK
 * key state. If Caps Lock is ON, the updater short-circuits and launches the existing
 * console without any network check — an emergency escape hatch when the user needs to
 * boot the console even if the update servers are unreachable or misbehaving. This is
 * independent of {@link AutoupdateProperty} / {@code autoupdate_bundle} (those only gate
 * the download; Caps Lock skips even the availability check).
 *
 * <h3>3. Launch-time silent update path ({@link #main} / {@link #autoupdate})</h3>
 * When the user double-clicks the bundle's autoupdate exe:
 * <ol>
 *   <li>Read {@link BundleInfo} from the local bundle name.</li>
 *   <li>If auto-update is enabled, {@link #downloadFreshZipFile} downloads the latest
 *       bundle zip (only when server timestamp/size indicate it's newer than the local
 *       copy — see {@link #isNewerAvailable}); otherwise nothing is downloaded.</li>
 *   <li>{@link #unzipAutoUpdate} extracts the bundle over the installation.</li>
 *   <li>{@link #prefetchLastConnectedBoardFirmware} (issue #9714) refreshes firmware for
 *       the board the user last connected to, so a later in-console "Update ECU" is
 *       instant.</li>
 *   <li>{@link #startConsoleAsANewProcess} launches the freshly unpacked console.</li>
 * </ol>
 *
 * <h3>4. In-console UX: the two "Actions" menu items</h3>
 * Wiring lives in {@code MainFrame}:
 * <ul>
 *   <li><b>"Update Software" ({@code updateSoftwareItem})</b> — updates the bundle
 *       (console + firmware files) from within a running console, without needing to
 *       restart via the autoupdate exe.
 *       <ul>
 *         <li>Disabled by default. Only when {@link #isAutoUpdateEnabled} is false (the user
 *             preference is OFF or the bundle hard-disables auto-update — i.e. the launch-time
 *             silent update did not run, see #9775) does {@code MainFrame} spawn a background
 *             thread that polls {@link #isUpdateAvailable} and enables the menu item if a newer
 *             bundle exists on the server. When auto-update is fully enabled the launch-time
 *             flow already handled it, so the menu stays disabled.</li>
 *         <li>Clicking runs {@link #runManualUpdate} which — bypassing the
 *             {@link #isAutoUpdateEnabled} gate — calls {@link #performUpdate} to
 *             download+unpack the bundle, stage {@code rusefi_console.jar} as
 *             {@link #PENDING_CONSOLE_JAR} (the running JAR cannot overwrite itself),
 *             then invokes {@link #relaunchConsole} which spawns a new JVM off the
 *             pending JAR; {@link #finalizePendingUpdate} promotes it to the real name
 *             during the next startup.</li>
 *       </ul></li>
 *   <li><b>"Update ECU" ({@code updateEcuItem})</b> — flashes the connected ECU when
 *       its firmware signature does not match the locally cached srec. Enabled by
 *       {@code MainFrame#checkFirmwareUpdate} after connection, based on
 *       {@code MainFrame#needsFirmwareUpdate} comparing the ECU's reported signature
 *       against the srec on disk. This action is about firmware-on-the-ECU, not about
 *       the console software itself; it delegates to the firmware update flow rather
 *       than to this class.</li>
 * </ul>
 *
 * <h3>5. "Check" vs. "Apply" are separate</h3>
 * The class deliberately splits availability check from the actual download+install:
 * <ul>
 *   <li>{@link #isUpdateAvailable} / {@link #isNewerAvailable} — read-only, network-only,
 *       safe to call from a background thread; used by {@code MainFrame} to decide
 *       whether to enable "Update Software".</li>
 *   <li>{@link #runSilentUpdate} — respects the {@link #isAutoUpdateEnabled} gate; used
 *       by callers that want the settings honored.</li>
 *   <li>{@link #runManualUpdate} — ignores the gate; used by the "Update Software"
 *       menu item because reaching that item already implies explicit user intent.</li>
 *   <li>{@link #performUpdate} — the shared apply path used by both silent and manual.</li>
 * </ul>
 *
 * @see com.rusefi.core.ui.ProgressView
 * @see com.rusefi.AutoupdateProperty
 */
public class Autoupdate {
    private static final Logging log = getLogging(Autoupdate.class);
    private static final int AUTOUPDATE_VERSION = 20260429; // separate from rusEFIVersion#CONSOLE_VERSION
    private static final String userHomeSubDirectory = FileUtil.RUSEFI_SETTINGS_FOLDER + "updates" + File.separator;

    /**
     * Filename used to stage a downloaded rusefi_console.jar before it can replace the running one.
     * The new process launched by {@link #relaunchConsole()} finalizes the swap via
     * {@link #finalizePendingUpdate()}.
     */
    public static final String PENDING_CONSOLE_JAR = "rusefi_console_pending.jar";

    public enum UpdateOutcome {
        UPDATED,
        UP_TO_DATE,
        FAILED,
        SKIPPED
    }

    static class DownloadResult {
        final UpdateOutcome outcome;
        final Optional<DownloadedAutoupdateFileInfo> file;

        private DownloadResult(UpdateOutcome outcome, Optional<DownloadedAutoupdateFileInfo> file) {
            this.outcome = outcome;
            this.file = file;
        }

        static DownloadResult updated(DownloadedAutoupdateFileInfo file) {
            return new DownloadResult(UpdateOutcome.UPDATED, Optional.of(file));
        }

        static DownloadResult of(UpdateOutcome outcome) {
            return new DownloadResult(outcome, Optional.empty());
        }
    }

    @FunctionalInterface
    interface UpdateDownloader {
        DownloadResult download(BundleInfo bundleInfo) throws Exception;
    }

    @FunctionalInterface
    interface UpdateApplier {
        void apply(DownloadedAutoupdateFileInfo file) throws Exception;
    }

    private static final String TITLE = getTitle();

    private static String getTitle() {
        try {
            return ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties()) + " Bundle Updater " + AUTOUPDATE_VERSION;
        } catch (Throwable e) {
            log.error("Error", e);
            return "Title error: " + e;
        }
    }

    static {
        log.info("java=" + System.getProperty("java.version"));
    }

    public static void main(String[] args) {
        try {
            FileLogger.init();
            log.info("Version " + AUTOUPDATE_VERSION);
            log.info("Compiled " + new Date(rusEFIVersion.classBuildTimeMillis(Autoupdate.class)));
            log.info("Current folder " + new File(".").getCanonicalPath());
            log.info("Source " + new File(Autoupdate.class.getProtectionDomain()
                .getCodeSource()
                .getLocation()
                .getPath())
                .getCanonicalPath());
            autoupdate(args);
        } catch (Throwable e) {
            log.error("Autoupdate Error", e);
            String stackTrace = extracted(e);
            ErrorMessageHelper.showErrorDialog(stackTrace, "Autoupdate Error " + TITLE);
            System.exit(-1);
        }
    }

    private static String extracted(Throwable e) {
        StringBuilder sb = new StringBuilder(e.toString());
        for (StackTraceElement ste : e.getStackTrace()) {
            sb.append("\n\tat ");
            sb.append(ste);
        }
        return sb.toString();
    }

    /**
     * Effective auto-update gate: requires BOTH the per-user {@link AutoupdateProperty} preference
     * and the bundle-level {@code autoupdate_bundle} property. When this returns false the
     * launch-time silent update is skipped, so {@code MainFrame} offers the manual
     * "Update Software" path instead (#9775).
     */
    public static boolean isAutoUpdateEnabled() {
        boolean property = AutoupdateProperty.get();
        boolean autoupdate_bundle = ConnectionAndMeta.getBoolean("autoupdate_bundle");
        boolean result = isAutoUpdateEnabled(property, autoupdate_bundle);
        if (!result) {
            log.info("AutoupdateProperty=" + property);
            log.info("autoupdate_bundle=" + autoupdate_bundle);
        }
        return result;
    }

    static boolean isAutoUpdateEnabled(boolean userPreference, boolean bundleEnabled) {
        return userPreference && bundleEnabled;
    }

    // everything here assumes Windows. Sorry!
    private static void autoupdate(String[] args) {
        BundleInfo bundleInfo = BundleUtil.readBundleFullNameNotNull();
        if (BundleInfo.isUndefined(bundleInfo)) {
            log.error("ERROR: Autoupdate: unable to perform without bundleFullName");
            System.exit(-1);
        }

        @NotNull String firstArgument = args.length > 0 ? args[0] : "";

        final Optional<DownloadedAutoupdateFileInfo> downloadedAutoupdateFile;
        if (!isAutoUpdateEnabled()) {
            log.info("Autoupdate is disabled by property or configuration");
            downloadedAutoupdateFile = Optional.empty();
        } else {
            downloadedAutoupdateFile = downloadFreshZipFile(firstArgument, bundleInfo);
            downloadedAutoupdateFile.ifPresent(downloadedFile -> ObsoleteFilesArchiver.INSTANCE.archiveObsoleteFiles());
        }

        // Let's try to get console .exe-file name before we rewrite autoupdate .jar file:
        final String consoleExeFileName = new ConsoleExeFileLocator().getConsoleExeFileName();

        // java lazy class-loader would get broken if we replace rusefi_autoupdate.jar file
        // ATTENTION! To avoid `ClassNotFoundException` we need to load all necessary classes before unzipping
        // autoupdate archive
        unzipAutoUpdate(downloadedAutoupdateFile);
        prefetchLastConnectedBoardFirmware();
        startConsoleAsANewProcess(consoleExeFileName, args);
    }

    /**
     * #9714 At startup, fetch (or refresh to the latest) the
     * firmware of the most recently connected board so a later "Update Firmware" is instant.
     * <p>
     * No-op when: autoupdate is disabled, no board has ever connected, or this is a single-board
     * bundle whose own firmware is the last-connected board (already refreshed by the bundle update
     * above). Only downloads when the server has a newer build than the local cache.
     */
    private static void prefetchLastConnectedBoardFirmware() {
        try {
            if (!isAutoUpdateEnabled()) {
                return;
            }
            String lastBoard = ConnectedEcuTarget.readPersisted();
            if (lastBoard == null) {
                log.info("[universal_bundle] prefetch: no previously connected board recorded");
                return;
            }
            log.info("[universal_bundle] prefetch: ensuring latest firmware for last connected board " + lastBoard);
            ensureFirmwareForTarget(lastBoard, percent -> {}, log::info);
        } catch (Throwable e) {
            log.error("[universal_bundle] prefetch error: " + e);
        }
    }

    /**
     * Relaunches the console as a new process and exits the current JVM.
     * Call this after {@link #runSilentUpdate} reports a completed update so the freshly
     * downloaded rusefi_console.jar is picked up by the new process.
     * <p>
     * Prefers a direct {@code java -jar} invocation (platform-independent) so that
     * {@code -DSKIP_ONE_INSTANCE_CHECK=true} can be passed as a JVM arg, preventing
     * the "already running" dialog that would otherwise appear before the old instance
     * releases its server socket. Falls back to the platform exe launcher when the
     * current JAR path cannot be resolved (e.g. running from an IDE class directory).
     */
    public static void relaunchConsole() {
        String jarPath = getCurrentJarPath();
        if (jarPath != null) {
            // If a staged update exists, launch from it so the new code is picked up immediately.
            File pendingJar = new File(new File(jarPath).getParentFile(), PENDING_CONSOLE_JAR);
            String launchJar = pendingJar.exists() ? pendingJar.getAbsolutePath() : jarPath;
            String javaExe = Paths.get(System.getProperty("java.home"), "bin", "java").toString();
            log.info("relaunchConsole: launching " + launchJar);
            try {
                new ProcessBuilder(javaExe, "-DSKIP_ONE_INSTANCE_CHECK=true", "-jar", launchJar).start();
                System.exit(0);
            } catch (IOException e) {
                log.error("Direct relaunch failed, falling back to exe launcher: " + e);
            }
        }
        String consoleExeFileName = new ConsoleExeFileLocator().getConsoleExeFileName();
        startConsoleAsANewProcess(consoleExeFileName, new String[0]);
        System.exit(0);
    }

    /**
     * If the running JAR is {@value PENDING_CONSOLE_JAR}, copies it over {@code rusefi_console.jar}
     * to complete the update staged during the previous session, then deletes the pending file.
     * Call this early in startup, before any other code locks class files.
     */
    public static void finalizePendingUpdate() {
        String jarPath = getCurrentJarPath();
        if (jarPath == null) return;
        File runningJar = new File(jarPath);
        if (!runningJar.getName().equals(PENDING_CONSOLE_JAR)) return;

        File targetJar = new File(runningJar.getParentFile(), "rusefi_console.jar");
        log.info("finalizePendingUpdate: " + runningJar + " -> " + targetJar);
        // relaunchConsole() starts this process and then System.exit(0)s the previous JVM, but that
        // old JVM was running from rusefi_console.jar and may not have released its file lock yet
        // (on Windows the copy fails with "being used by another process"). Retry to wait it out -
        // giving up on the first failure leaves the pending JAR orphaned and old code running (#9727).
        IOException lastError = null;
        for (int attempt = 1; attempt <= 20; attempt++) {
            try {
                Files.copy(runningJar.toPath(), targetJar.toPath(), StandardCopyOption.REPLACE_EXISTING);
                log.info("finalizePendingUpdate: console JAR replaced successfully on attempt " + attempt);
                lastError = null;
                break;
            } catch (IOException e) {
                lastError = e;
                try {
                    Thread.sleep(250);
                } catch (InterruptedException ie) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        }
        if (lastError != null) {
            // Keep the pending JAR so a later startup can still finalize the swap.
            log.error("finalizePendingUpdate: failed to replace console JAR: " + lastError);
            return;
        }
        // On Linux the pending file can be deleted even while open; on Windows this may fail silently.
        if (!runningJar.delete()) {
            log.info("finalizePendingUpdate: pending JAR will be cleaned up on next update");
        }
    }

    /**
     * Extracts the console JAR entry from the bundle ZIP to {@value PENDING_CONSOLE_JAR} in the
     * same {@code ../console/} directory, so the running JAR is not touched.
     */
    private static void extractConsoleJarAsPending(String zipFileName) {
        // ../console/ relative to working dir (console/) is the console directory itself
        File pendingJar = new File(new File(".."), "console" + File.separator + PENDING_CONSOLE_JAR);
        log.info("extractConsoleJarAsPending: " + pendingJar);
        try (ZipInputStream zis = new ZipInputStream(Files.newInputStream(Paths.get(zipFileName)))) {
            ZipEntry entry;
            while ((entry = zis.getNextEntry()) != null) {
                if (isConsoleJar.test(entry)) {
                    pendingJar.getParentFile().mkdirs();
                    try (FileOutputStream fos = new FileOutputStream(pendingJar)) {
                        byte[] buffer = new byte[4096];
                        int len;
                        while ((len = zis.read(buffer)) > 0)
                            fos.write(buffer, 0, len);
                    }
                    log.info("extractConsoleJarAsPending: done (" + pendingJar.length() + " bytes)");
                    return;
                }
            }
            log.info("extractConsoleJarAsPending: entry " + consoleJarZipEntry + " not found in bundle");
        } catch (IOException e) {
            log.error("extractConsoleJarAsPending: " + e);
        }
    }

    /**
     * @return absolute path to the running JAR file, or {@code null} if running from a class directory.
     */
    public static String getCurrentJarPath() {
        try {
            URI location = Autoupdate.class.getProtectionDomain().getCodeSource().getLocation().toURI();
            File file = new File(location);
            if (file.isFile() && file.getName().endsWith(".jar")) {
                return file.getAbsolutePath();
            }
            log.info("getCurrentJarPath: not a JAR (" + file + ")");
        } catch (Exception e) {
            log.error("getCurrentJarPath error: " + e);
        }
        return null;
    }

    /**
     * Checks server metadata to determine if a newer bundle is available, without downloading anything.
     * Safe to call from a background thread.
     */
    public static boolean isUpdateAvailable() {
        try {
            BundleInfo bundleInfo = BundleUtil.readBundleFullNameNotNull();
            if (BundleInfo.isUndefined(bundleInfo)) {
                return false;
            }
            return isNewerAvailable(bundleInfo);
        } catch (Exception e) {
            log.error("isUpdateAvailable error: " + e);
            return false;
        }
    }

    private static boolean isNewerAvailable(BundleInfo bundleInfo) {
        try {
            String branchUrl = BundleInfoStrategy.getDownloadUrl(bundleInfo, PropertiesHolder.getBaseUrl(), BundleInfo::getBranchName);
            boolean isObfuscated = FindFileHelper.isObfuscated();
            String suffix = isObfuscated ? "_obfuscated_public" : "";
            String folderName = bundleInfo.getTarget() + "_" + bundleInfo.getBranchName();
            String localFolder = userHomeSubDirectory + folderName + File.separator;
            new File(localFolder).mkdirs();
            String fileName = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties()) + "_bundle_" + bundleInfo.getTarget() + suffix + "_autoupdate" + ".zip";
            String localZipFileName = localFolder + fileName;
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(fileName).invoke(branchUrl);
            log.info("isNewerAvailable: local=" + localZipFileName);
            log.info("isNewerAvailable: server size=" + connectionAndMeta.getCompleteFileSize() + " modified=" + new Date(connectionAndMeta.getLastModified()));
            File localFile = new File(localZipFileName);
            log.info("isNewerAvailable: local  size=" + localFile.length() + " modified=" + new Date(localFile.lastModified()));
            boolean upToDate = AutoupdateUtil.hasExistingFile(localZipFileName, connectionAndMeta.getCompleteFileSize(), connectionAndMeta.getLastModified());
            log.info("isNewerAvailable: update " + (upToDate ? "NOT needed (already up to date)" : "IS available"));
            return !upToDate;
        } catch (IOException e) {
            log.error("isNewerAvailable error: " + e);
            return false;
        }
    }

    /**
     * Runs a silent background update from within rusefi_console: downloads and unpacks the bundle
     * (excluding rusefi_console.jar itself, which cannot be replaced while running),
     * @return the result of checking and, when needed, applying the update
     */
    public static UpdateOutcome runSilentUpdate() {
        try {
            log.info("runSilentUpdate: starting");
            BundleInfo bundleInfo = BundleUtil.readBundleFullNameNotNull();
            log.info("runSilentUpdate: bundle=" + bundleInfo);
            if (BundleInfo.isUndefined(bundleInfo)) {
                log.info("runSilentUpdate: no bundle info, skipping");
                return UpdateOutcome.SKIPPED;
            }
            if (!isAutoUpdateEnabled()) {
                log.info("runSilentUpdate: autoupdate is disabled");
                return UpdateOutcome.SKIPPED;
            }
            return performUpdate(bundleInfo);
        } catch (Throwable e) {
            log.error("runSilentUpdate error: " + e);
            return UpdateOutcome.FAILED;
        }
    }

    /**
     * Performs a bundle update regardless of the AutoupdateProperty setting.
     * Intended for the "Update Software" menu item when auto-update is disabled.
     */
    public static void runManualUpdate(Consumer<String> onComplete) {
        try {
            log.info("runManualUpdate: starting");
            BundleInfo bundleInfo = BundleUtil.readBundleFullNameNotNull();
            log.info("runManualUpdate: bundle=" + bundleInfo);
            if (BundleInfo.isUndefined(bundleInfo)) {
                log.info("runManualUpdate: no bundle info, skipping");
                onComplete.accept(null);
                return;
            }
            UpdateOutcome outcome = performUpdate(bundleInfo);
            onComplete.accept(outcome == UpdateOutcome.UPDATED
                ? "Update installed - please restart to apply the new console"
                : null);
        } catch (Throwable e) {
            log.error("runManualUpdate error: " + e);
            onComplete.accept(null);
        }
    }

    private static UpdateOutcome performUpdate(BundleInfo bundleInfo) {
        return performUpdate(bundleInfo, Autoupdate::doDownloadDetailed, Autoupdate::applyUpdate);
    }

    static UpdateOutcome performUpdate(BundleInfo bundleInfo, UpdateDownloader downloader, UpdateApplier applier) {
        log.info("performUpdate: checking for update...");
        try {
            DownloadResult downloaded = downloader.download(bundleInfo);
            if (downloaded.outcome != UpdateOutcome.UPDATED) {
                log.info("performUpdate: " + downloaded.outcome);
                return downloaded.outcome;
            }
            if (!downloaded.file.isPresent()) {
                log.error("performUpdate: downloaded update has no file");
                return UpdateOutcome.FAILED;
            }
            log.info("performUpdate: update downloaded, applying...");
            applier.apply(downloaded.file.get());
            return UpdateOutcome.UPDATED;
        } catch (Throwable e) {
            log.error("performUpdate failed", e);
            return UpdateOutcome.FAILED;
        }
    }

    private static void applyUpdate(DownloadedAutoupdateFileInfo file) throws IOException {
        ObsoleteFilesArchiver.INSTANCE.archiveObsoleteFiles();
        findSrecFile(false);
        // Unzip everything except the console JAR (cannot replace a running JAR).
        FileUtil.unzip(file.zipFileName, new File(".."), isConsoleJar.negate());
        final String srecFile = findSrecFile();
        final String firmwareFile = findFirmwareFile();
        new File(srecFile == null ? firmwareFile : srecFile).setLastModified(file.lastModified);
        tryInstallTsPlugin();
        // Stage the new console JAR under a different name so relaunchConsole() can
        // launch from it and finalizePendingUpdate() can swap it in on next startup.
        extractConsoleJarAsPending(file.zipFileName);
    }

    private static Optional<DownloadedAutoupdateFileInfo> downloadFreshZipFile(String firstArgument, BundleInfo bundleInfo) {
        Optional<DownloadedAutoupdateFileInfo> downloadedAutoupdateFile;
        if (firstArgument.equalsIgnoreCase("release")) {
            // this branch needs progress for custom boards!
            log.info("Release update requested");
            downloadedAutoupdateFile = downloadAutoupdateZipFile(
                bundleInfo,
                ConnectionAndMeta.BASE_URL_RELEASE, FindFileHelper.isObfuscated());
        } else {
            downloadedAutoupdateFile = doDownload(bundleInfo);
        }
        return downloadedAutoupdateFile;
    }

    private static void unzipAutoUpdate(Optional<DownloadedAutoupdateFileInfo> downloadedAutoupdateFile) {
        log.info("unzipAutoUpdate " + downloadedAutoupdateFile);
        // todo: we still have technical debt here! https://github.com/rusefi/rusefi/issues/7971
        downloadedAutoupdateFile.ifPresent(Autoupdate::unzipFreshConsole);
        downloadedAutoupdateFile.ifPresent(autoupdateFile -> {
            findSrecFile(false); // move all current copies into older fw folder
            try {
                String pathname = "..";
                log.info("unzipping everything else into " + pathname);
                // We've already prepared class loader, so now we can unzip rusefi_autoupdate.jar and other files
                // except already unzipped rusefi_console.jar (see #6777):
                FileUtil.unzip(autoupdateFile.zipFileName, new File(pathname), isConsoleJar.negate());
                final String srecFile = findSrecFile();
                final String firmwareFile = findFirmwareFile();
                new File(srecFile == null ? firmwareFile : srecFile)
                    .setLastModified(autoupdateFile.lastModified);

                tryInstallTsPlugin();
            } catch (IOException e) {
                log.error("Error unzipping autoupdate from bundle: " + e);
                if (!AutoupdateUtil.runHeadless) {
                    ErrorMessageHelper.showErrorDialog("Error unzipping autoupdate from bundle: " + e, "Error");
                }
            }
        });
    }

    private static void tryInstallTsPlugin() {
        try {
            TsPluginInstaller.installTsPlugin();
        } catch (NoClassDefFoundError e) {
            log.warn("Error installTsPlugin: " + e);
        }
    }

    private static void unzipFreshConsole(DownloadedAutoupdateFileInfo autoupdateFile) {
        try {
            log.info("unzipFreshConsole " + autoupdateFile.zipFileName + " only " + consoleJarZipEntry);
            // We cannot unzip rusefi_autoupdate.jar file because we need the old one to prepare class loader below
            // (otherwise we get `ZipFile invalid LOC header (bad signature)` exception, see #6777). So now we unzip
            // only rusefi_console.jar:
            FileUtil.unzip(autoupdateFile.zipFileName, new File(".."), isConsoleJar);
        } catch (IOException e) {
            log.error("Error unzipping bundle without autoupdate: " + e);
            if (!AutoupdateUtil.runHeadless) {
                ErrorMessageHelper.showErrorDialog("Error unzipping bundle without autoupdate: " + e, "Error");
            }
        }
    }

    private static final String consoleJarZipEntry = resolveConsoleJarZipEntry();

    private static String resolveConsoleJarZipEntry() {
        try {
            return String.format("console/%s", ConnectionAndMeta.getRusEfiConsoleJarName());
        } catch (Exception e) {
            // JarFileUtil cannot parse the running JAR filename (e.g. when loaded inside the shadow console JAR).
            // Fall back to the well-known default name
            log.info("resolveConsoleJarZipEntry fallback: " + e);
            return "console/rusefi_console.jar";
        }
    }

    private static final Predicate<ZipEntry> isConsoleJar = zipEntry -> consoleJarZipEntry.equals(zipEntry.getName());

    private static Optional<DownloadedAutoupdateFileInfo> doDownload(final BundleInfo bundleInfo) {
        return doDownloadDetailed(bundleInfo).file;
    }

    private static DownloadResult doDownloadDetailed(final BundleInfo bundleInfo) {
        String branchUrl = BundleInfoStrategy.getDownloadUrl(bundleInfo, PropertiesHolder.getBaseUrl(), BundleInfoStrategy::selectBranchName);
        return downloadAutoupdateZipFileDetailed(bundleInfo, branchUrl, FindFileHelper.isObfuscated());
    }

    /**
     * when a universal bundle is connected to an ECU whose board
     * differs from the one this bundle shipped with, fetch that board's firmware on demand by downloading
     * its autoupdate zip from the build server and unpacking the firmware artifacts
     * (rusefi.bin / *_update.srec / openblt.bin) next to the console, where the existing DFU/OpenBLT
     * flashers look for them. Because the target is derived from the connected ECU's signature, the
     * staged firmware always matches the ECU.
     *
     * @param ecuTarget bundle target reported by the connected ECU (its signature's bundle target)
     * @return true if firmware for {@code ecuTarget} is present locally afterwards
     */
    public static boolean ensureFirmwareForTarget(String ecuTarget,
                                                  ConnectionAndMeta.DownloadProgressListener progressListener,
                                                  Consumer<String> logger) {
        if (ecuTarget == null || ecuTarget.trim().isEmpty()) {
            log.error("[universal_bundle] ensureFirmwareForTarget: no ECU target");
            return false;
        }
        BundleInfo local = BundleUtil.readBundleFullNameNotNull();
        String localTarget = local.getTarget();
        if (localTarget != null && localTarget.equalsIgnoreCase(ecuTarget)) {
            // the connected ECU is this bundle's own board - firmware already shipped locally
            return true;
        }
        String branchName = BundleInfo.isUndefined(local) ? "master" : local.getBranchName();
        BundleInfo wanted = new BundleInfo(branchName, null, ecuTarget);
        String baseUrl = BundleInfoStrategy.getDownloadUrl(wanted, PropertiesHolder.getBaseUrl(), BundleInfo::getBranchName);
        String zip = downloadZipForTarget(wanted, baseUrl, progressListener, logger);
        if (zip == null) {
            return false;
        }
        try {
            findSrecFile(false); // move firmware of the previously-selected board into older-fw
            log.info("[universal_bundle] ensureFirmwareForTarget: unpacking firmware for " + ecuTarget + " from " + zip);
            FileUtil.unzip(zip, new File(".."), isFirmwareArtifact);
            return true;
        } catch (IOException e) {
            log.error("[universal_bundle] ensureFirmwareForTarget: unzip failed: " + e);
            return false;
        }
    }

    /**
     * Like the firmware half of {@link #downloadAutoupdateZipFile} but returns the local zip path even
     * when the cached copy is already up to date (we still need to unpack it for the selected board).
     *
     * @return local zip path, or null on error
     */
    private static String downloadZipForTarget(BundleInfo info, String baseUrl,
                                               ConnectionAndMeta.DownloadProgressListener progressListener,
                                               Consumer<String> logger) {
        // The universal console cannot know prior whether a given board ships open or obfuscated
        // firmware. Try the public zip first and fall back to the obfuscated one - whichever the server has. #9714
        String zip = downloadZipForTarget(info, baseUrl, progressListener, logger, false);
        if (zip == null) {
            zip = downloadZipForTarget(info, baseUrl, progressListener, logger, true);
        }
        return zip;
    }

    private static String downloadZipForTarget(BundleInfo info, String baseUrl,
                                               ConnectionAndMeta.DownloadProgressListener progressListener,
                                               Consumer<String> logger, boolean obfuscated) {
        try {
            String suffix = obfuscated ? "_obfuscated_public" : "";
            String folderName = info.getTarget() + "_" + info.getBranchName();
            String localFolder = userHomeSubDirectory + folderName + File.separator;
            new File(localFolder).mkdirs();
            String fileName = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties())
                + "_bundle_" + info.getTarget() + suffix + "_autoupdate.zip";
            String localZipFileName = localFolder + fileName;
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(fileName).invoke(baseUrl);
            long lastModified = connectionAndMeta.getLastModified();
            if (AutoupdateUtil.hasExistingFile(localZipFileName, connectionAndMeta.getCompleteFileSize(), lastModified)) {
                log.info("[universal_bundle] downloadZipForTarget: reusing cached " + localZipFileName);
                logger.accept("Using cached firmware download for " + info.getTarget());
            } else {
                logger.accept("[universal_bundle] Downloading firmware for " + info.getTarget()
                    + (obfuscated ? " (obfuscated)" : "")
                    + " (" + (connectionAndMeta.getCompleteFileSize() / 1024) + " KB)...");
                // Route progress through the caller's callbacks (the reflash progress bar) instead of
                // popping up a separate download dialog - #9714 makes it an inline step.
                ConnectionAndMeta.downloadFile(localZipFileName, connectionAndMeta, progressListener);
                new File(localZipFileName).setLastModified(lastModified);
                logger.accept("[universal_bundle] Firmware download complete");
            }
            return localZipFileName;
        } catch (IOException e) {
            // Expected when this variant does not exist on the server (e.g. public miss -> try obfuscated).
            log.info("[universal_bundle] downloadZipForTarget (" + (obfuscated ? "obfuscated" : "public")
                + ") not available for " + info.getTarget() + ": " + e);
            return null;
        }
    }

    public static FirmwareArtifacts downloadFirmware(FirmwareRollbackResolver.Build build,
                                                      ConnectionAndMeta.DownloadProgressListener progress,
                                                      Consumer<String> logger) {
        Path ltsRoot = Paths.get(userHomeSubDirectory, "lts");
        return downloadFirmware(build, progress, logger, ltsRoot, Autoupdate::downloadFirmwareZip);
    }

    @FunctionalInterface
    interface FirmwareZipDownloader {
        Path download(FirmwareRollbackResolver.Build build, Path destination, boolean obfuscated,
                      ConnectionAndMeta.DownloadProgressListener progress, Consumer<String> logger) throws IOException;
    }

    static FirmwareArtifacts downloadFirmware(FirmwareRollbackResolver.Build build,
                                               ConnectionAndMeta.DownloadProgressListener progress,
                                               Consumer<String> logger,
                                               Path ltsRoot,
                                               FirmwareZipDownloader downloader) {
        try {
            Path destination = ltsRoot.resolve(build.getBoard())
                .resolve(build.getBranch())
                .resolve(build.getSha())
                .toAbsolutePath();
            Files.createDirectories(destination);

            Path zip = downloader.download(build, destination, false, progress, logger);
            if (zip == null) {
                zip = downloader.download(build, destination, true, progress, logger);
            }
            if (zip == null) {
                logger.accept("Firmware build " + build.getSha() + " is unavailable");
                return FirmwareArtifacts.empty();
            }

            final String[] paths = new String[2];
            FileUtil.unzip(zip.toString(), destination.toFile(), entry -> {
                if (!isFirmwareArtifact.test(entry)) {
                    return false;
                }
                String lower = entry.getName().toLowerCase();
                String absolutePath = destination.resolve(entry.getName()).normalize().toAbsolutePath().toString();
                if (lower.endsWith(".srec")) {
                    paths[0] = absolutePath;
                } else if (isDfuBin(lower)) {
                    paths[1] = absolutePath;
                }
                return true;
            });
            FirmwareArtifacts artifacts = new FirmwareArtifacts(paths[0], paths[1]);
            if (!artifacts.getSrecPath().isPresent() && !artifacts.getBinPath().isPresent()) {
                logger.accept("Firmware build " + build.getSha() + " contains no SREC or BIN artifact");
            }
            return artifacts;
        } catch (IOException e) {
            log.error("Firmware download failed for " + build.getSha() + ": " + e);
            logger.accept("Firmware download failed: " + e.getMessage());
            return FirmwareArtifacts.empty();
        }
    }

    private static Path downloadFirmwareZip(FirmwareRollbackResolver.Build build, Path destination,
                                            boolean obfuscated,
                                            ConnectionAndMeta.DownloadProgressListener progress,
                                            Consumer<String> logger) throws IOException {
        String suffix = obfuscated ? "_obfuscated_public" : "";
        String fileName = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties())
            + "_bundle_" + build.getBoard() + suffix + "_autoupdate.zip";
        Path localZip = destination.resolve(fileName);
        String baseUrl = build.getDirectoryUrl() + "autoupdate/";
        try {
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(fileName).invoke(baseUrl);
            if (AutoupdateUtil.hasExistingFile(localZip.toString(), connectionAndMeta.getCompleteFileSize(),
                connectionAndMeta.getLastModified())) {
                logger.accept("Using cached firmware build " + build.getSha());
            } else {
                logger.accept("Downloading firmware build " + build.getSha()
                    + (obfuscated ? " (obfuscated)" : ""));
                ConnectionAndMeta.downloadFile(localZip.toString(), connectionAndMeta, progress);
            }
            return localZip;
        } catch (IOException e) {
            Files.deleteIfExists(localZip);
            log.info("Firmware ZIP " + fileName + " is unavailable: " + e);
            return null;
        }
    }

    private static boolean isDfuBin(String lowerName) {
        return lowerName.endsWith("rusefi.bin")
            || (lowerName.contains("rusefi_") && lowerName.endsWith(".bin"));
    }

    public static final class FirmwareArtifacts {
        private final Optional<String> srecPath;
        private final Optional<String> binPath;

        private FirmwareArtifacts(String srecPath, String binPath) {
            this.srecPath = Optional.ofNullable(srecPath);
            this.binPath = Optional.ofNullable(binPath);
        }

        private static FirmwareArtifacts empty() {
            return new FirmwareArtifacts(null, null);
        }

        public Optional<String> getSrecPath() {
            return srecPath;
        }

        public Optional<String> getBinPath() {
            return binPath;
        }
    }

    /**
     * Firmware binaries that live at the root of an autoupdate zip. Deliberately excludes the console
     * jar, release.txt, shared_io.properties and the .ini
     */
    private static final Predicate<ZipEntry> isFirmwareArtifact = entry -> {
        if (entry.isDirectory())
            return false;
        String lower = entry.getName().toLowerCase();
        return lower.endsWith(".srec") || lower.endsWith(".hex")
            || lower.endsWith("rusefi.bin") || lower.endsWith("openblt.bin")
            // "rusefi_" (underscore) excludes rusefi-obfuscated.bin
            || (lower.contains("rusefi_") && lower.endsWith(".bin"));
    };

    /**
     * rusefi_updater.exe/invokes rusefi_console.jar - entry point is Launcher#main
     */
    private static void startConsoleAsANewProcess(final String consoleExeFileName, final String[] args) {
        if (!Files.exists(Paths.get(consoleExeFileName))) {
            log.error(String.format("File `%s` to launch isn't found", consoleExeFileName));
            if (!AutoupdateUtil.runHeadless) {
                ErrorMessageHelper.showErrorDialog(String.format("File `%s` to launch isn't found.", consoleExeFileName), "Error");
            }
            return;
        }
        log.info(String.format("File `%s` to launch is found", consoleExeFileName));
        final String[] processBuilderArgs = new String[args.length + 1];
        processBuilderArgs[0] = consoleExeFileName;
        System.arraycopy(args, 0, processBuilderArgs, 1, args.length);
        try {
            log.info(String.format("We're starting `%s` process", consoleExeFileName));
            new ProcessBuilder(processBuilderArgs).start();
            log.info(String.format("Process `%s` is started", consoleExeFileName));
        } catch (final IOException e) {
            final String command = String.join(" ", processBuilderArgs);
            log.error(String.format("Failed to run `$s` command", command), e);
            if (!AutoupdateUtil.runHeadless) {
                ErrorMessageHelper.showErrorDialog(String.format(
                    "Error running `%s` command.\nPlease try to run it manually again.",
                    command
                ), "Error");
            }
        }
    }

    public static class DownloadedAutoupdateFileInfo {
        final String zipFileName;
        final long lastModified;

        DownloadedAutoupdateFileInfo(final String zipFileName, final long lastModified) {
            this.zipFileName = zipFileName;
            this.lastModified = lastModified;
        }
    }

    /**
     * sad fact: java does not have a way to poll 'shift' key state, modifiers are only available via InputEvents!
     */
    private static boolean isSkipUpdater() {
        return Toolkit.getDefaultToolkit().getLockingKeyState(KeyEvent.VK_CAPS_LOCK);
    }


    /**
     * @return empty if we already have latest, or in case of error
     */
    public static Optional<DownloadedAutoupdateFileInfo> downloadAutoupdateZipFile(
        final BundleInfo info,
        final String baseUrl,
        boolean isObfuscated) {
        return downloadAutoupdateZipFileDetailed(info, baseUrl, isObfuscated).file;
    }

    private static DownloadResult downloadAutoupdateZipFileDetailed(
        final BundleInfo info,
        final String baseUrl,
        boolean isObfuscated) {
        if (isSkipUpdater()) {
            log.info("User wants to skip auto-update");
            return DownloadResult.of(UpdateOutcome.SKIPPED);
        }

        try {
            String suffix = isObfuscated ? "_obfuscated_public" : "";
            String folderName = info.getTarget() + "_" + info.getBranchName();
            String localFolder = userHomeSubDirectory + folderName + File.separator;
            new File(localFolder).mkdirs();

            String fileName = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties()) + "_bundle_" + info.getTarget() + suffix + "_autoupdate" + ".zip";
            String localZipFileName = localFolder + fileName;
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(fileName).invoke(baseUrl);
            log.info("Local file " + localZipFileName);
            log.info("Remote file " + fileName);
            log.info("Server has " + connectionAndMeta.getCompleteFileSize() + " from " + new Date(connectionAndMeta.getLastModified()));

            if (AutoupdateUtil.hasExistingFile(localZipFileName, connectionAndMeta.getCompleteFileSize(), connectionAndMeta.getLastModified())) {
                log.info("We already have latest update " + new Date(connectionAndMeta.getLastModified()));
                return DownloadResult.of(UpdateOutcome.UP_TO_DATE);
            }

            // todo: user could have waited hours to respond to question above, we probably need to re-establish connection
            long completeFileSize = connectionAndMeta.getCompleteFileSize();
            final long lastModified = connectionAndMeta.getLastModified();

            log.info(info + " " + completeFileSize + " bytes, last modified " + new Date(lastModified));

            AutoupdateUtil.downloadAutoupdateFile(localZipFileName, connectionAndMeta, TITLE);

            File file = new File(localZipFileName);
            file.setLastModified(lastModified);
            log.info("Downloaded " + file.length() + " bytes, lastModified=" + lastModified);

            return DownloadResult.updated(new DownloadedAutoupdateFileInfo(localZipFileName, lastModified));
        } catch (ReportedIOException e) {
            // we had already reported error with a UI dialog when we had parent frame
            log.error("Error downloading bundle: " + e);
        } catch (IOException e) {
            // we are here if error happened while we did not have UI frame
            // todo: open frame prior to network connection and keep frame opened while uncompressing?
            log.error("Error downloading bundle: " + e);
            if (!AutoupdateUtil.runHeadless) {
                ErrorMessageHelper.showErrorDialog("Error downloading " + e, "Error");
            }
        }
        return DownloadResult.of(UpdateOutcome.FAILED);
    }
}
