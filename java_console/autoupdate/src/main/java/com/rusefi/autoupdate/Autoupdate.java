package com.rusefi.autoupdate;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
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
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Date;
import java.util.Optional;
import java.util.function.Predicate;
import java.util.zip.ZipEntry;

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
    private static final int AUTOUPDATE_VERSION = 20260101; // separate from rusEFIVersion#CONSOLE_VERSION
    private static final String userHomeSubDirectory = FileUtil.RUSEFI_SETTINGS_FOLDER + "updates" + File.separator;
    private static final String DO_NOT_UPDATE_PROPERTY_KEY = "Autoupdate.do_not_download";
    private static final boolean doNotDownloadPropertyValue;
    private static final String SUPPRESS_FILE_NAME = FileUtil.RUSEFI_SETTINGS_FOLDER + "donotdownload";
    private static final boolean suppressDownloadViaFlagFile = new File(SUPPRESS_FILE_NAME).exists();

    static {
        doNotDownloadPropertyValue = Boolean.getBoolean(DO_NOT_UPDATE_PROPERTY_KEY);
        log.info(DO_NOT_UPDATE_PROPERTY_KEY + "=" + doNotDownloadPropertyValue);
        log.info(SUPPRESS_FILE_NAME + " exists: " + suppressDownloadViaFlagFile);
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
        if (doNotDownloadPropertyValue || suppressDownloadViaFlagFile) {
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
        safeUnzipMakingSureClassloaderIsHappy(downloadedAutoupdateFile);
        startConsoleAsANewProcess(consoleExeFileName, args);
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

    private static void safeUnzipMakingSureClassloaderIsHappy(Optional<DownloadedAutoupdateFileInfo> downloadedAutoupdateFile) {
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

    private static final String consoleJarZipEntry =
        String.format("console/%s", ConnectionAndMeta.getRusEfiConsoleJarName());

    private static final Predicate<ZipEntry> isConsoleJar = zipEntry -> consoleJarZipEntry.equals(zipEntry.getName());

    private static Optional<DownloadedAutoupdateFileInfo> doDownload(final BundleInfo bundleInfo) {
        String branchUrl = BundleInfoStrategy.getDownloadUrl(bundleInfo, PropertiesHolder.getBaseUrl(), BundleInfoStrategy::selectBranchName);
        return downloadAutoupdateZipFile(bundleInfo, branchUrl, FindFileHelper.isObfuscated());
    }

    /**
     * rusefi_console.exe/invokes rusefi_console.jar - entry point is Launcher#main
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
