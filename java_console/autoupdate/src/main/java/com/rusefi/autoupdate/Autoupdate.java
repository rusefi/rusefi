package com.rusefi.autoupdate;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.rusefi.AutoupdateProperty;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleInfoStrategy;
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
 * entry point of rusefi_autoupdate.exe
 *
 * We've given up on complex classloader logic for auto-update and ended up with startConsoleAsANewProcess approach
 * @see com.rusefi.core.ui.ProgressView
 */
public class Autoupdate {
    private static final Logging log = getLogging(Autoupdate.class);
    private static final int AUTOUPDATE_VERSION = 20260325; // separate from rusEFIVersion#CONSOLE_VERSION
    private static final String userHomeSubDirectory = FileUtil.RUSEFI_SETTINGS_FOLDER + "updates" + File.separator;

    /**
     * Filename used to stage a downloaded rusefi_console.jar before it can replace the running one.
     * The new process launched by {@link #relaunchConsole()} finalizes the swap via
     * {@link #finalizePendingUpdate()}.
     */
    public static final String PENDING_CONSOLE_JAR = "rusefi_console_pending.jar";

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

    // everything here assumes Windows. Sorry!
    private static void autoupdate(String[] args) {
        BundleInfo bundleInfo = BundleUtil.readBundleFullNameNotNull();
        if (BundleInfo.isUndefined(bundleInfo)) {
            log.error("ERROR: Autoupdate: unable to perform without bundleFullName");
            System.exit(-1);
        }

        @NotNull String firstArgument = args.length > 0 ? args[0] : "";

        final Optional<DownloadedAutoupdateFileInfo> downloadedAutoupdateFile;
        if (!AutoupdateProperty.get()) {
            log.info(AutoupdateProperty.AUTO_UPDATE_BUNDLE_PROPERTY + " says 'do not update'");
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
        startConsoleAsANewProcess(consoleExeFileName, args);
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
        try {
            Files.copy(runningJar.toPath(), targetJar.toPath(), StandardCopyOption.REPLACE_EXISTING);
            log.info("finalizePendingUpdate: console JAR replaced successfully");
        } catch (IOException e) {
            log.error("finalizePendingUpdate: failed to replace console JAR: " + e);
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
     * Runs a silent background update from within rusefi_console: downloads and unpacks the bundle
     * (excluding rusefi_console.jar itself, which cannot be replaced while running),
     * then invokes {@code onComplete} with a non-null restart message when an update was applied,
     * or with {@code null} when no update was needed.
     */
    public static void runSilentUpdate(Consumer<String> onComplete) {
        try {
            log.info("runSilentUpdate: starting");
            BundleInfo bundleInfo = BundleUtil.readBundleFullNameNotNull();
            log.info("runSilentUpdate: bundle=" + bundleInfo);
            if (BundleInfo.isUndefined(bundleInfo)) {
                log.info("runSilentUpdate: no bundle info, skipping");
                onComplete.accept(null);
                return;
            }
            if (!AutoupdateProperty.get()) {
                log.info("runSilentUpdate: " + AutoupdateProperty.AUTO_UPDATE_BUNDLE_PROPERTY + " says 'do not update'");
                onComplete.accept(null);
                return;
            }
            log.info("runSilentUpdate: checking for update...");
            Optional<DownloadedAutoupdateFileInfo> downloaded = doDownload(bundleInfo);
            if (!downloaded.isPresent()) {
                log.info("runSilentUpdate: no update available or download skipped");
                onComplete.accept(null);
                return;
            }
            log.info("runSilentUpdate: update downloaded, applying...");
            ObsoleteFilesArchiver.INSTANCE.archiveObsoleteFiles();
            DownloadedAutoupdateFileInfo file = downloaded.get();
            findSrecFile(false);
            try {
                // Unzip everything except the console JAR (cannot replace a running JAR).
                FileUtil.unzip(file.zipFileName, new File(".."), isConsoleJar.negate());
                final String srecFile = findSrecFile();
                final String firmwareFile = findFirmwareFile();
                new File(srecFile == null ? firmwareFile : srecFile).setLastModified(file.lastModified);
                tryInstallTsPlugin();
                // Stage the new console JAR under a different name so relaunchConsole() can
                // launch from it and finalizePendingUpdate() can swap it in on next startup.
                extractConsoleJarAsPending(file.zipFileName);
            } catch (IOException e) {
                log.error("runSilentUpdate: error unzipping: " + e);
            }
            onComplete.accept("Update installed — please restart to apply the new console");
        } catch (Throwable e) {
            log.error("runSilentUpdate error: " + e);
            onComplete.accept(null);
        }
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
        String branchUrl = BundleInfoStrategy.getDownloadUrl(bundleInfo, PropertiesHolder.getBaseUrl(), BundleInfoStrategy::selectBranchName);
        return downloadAutoupdateZipFile(bundleInfo, branchUrl, FindFileHelper.isObfuscated());
    }

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
        if (isSkipUpdater()) {
            log.info("User wants to skip auto-update");
            return Optional.empty();
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
                return Optional.empty();
            }

            // todo: user could have waited hours to respond to question above, we probably need to re-establish connection
            long completeFileSize = connectionAndMeta.getCompleteFileSize();
            final long lastModified = connectionAndMeta.getLastModified();

            log.info(info + " " + completeFileSize + " bytes, last modified " + new Date(lastModified));

            AutoupdateUtil.downloadAutoupdateFile(localZipFileName, connectionAndMeta, TITLE);

            File file = new File(localZipFileName);
            file.setLastModified(lastModified);
            log.info("Downloaded " + file.length() + " bytes, lastModified=" + lastModified);

            return Optional.of(new DownloadedAutoupdateFileInfo(localZipFileName, lastModified));
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
        return Optional.empty();
    }
}
