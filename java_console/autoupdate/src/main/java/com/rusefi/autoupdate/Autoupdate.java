package com.rusefi.autoupdate;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.FileUtil;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.core.ui.AutoupdateUtil;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.io.*;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URLClassLoader;
import java.util.Arrays;
import java.util.Date;
import java.util.Optional;
import java.util.Properties;
import java.util.function.Predicate;
import java.util.zip.ZipEntry;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.FindFileHelper.findSrecFile;

public class Autoupdate {
    private static final Logging log = getLogging(Autoupdate.class);
    private static final int AUTOUPDATE_VERSION = 20250426; // separate from rusEFIVersion#CONSOLE_VERSION

    private static final String TITLE = getTitle();

    private static String getTitle() {
        try {
            return ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties()) + " Bundle Updater " + AUTOUPDATE_VERSION;
        } catch (Throwable e) {
            log.error("Error", e);
            return "Title error: " + e;
        }
    }

    private static final String COM_RUSEFI_LAUNCHER = "com.rusefi.Launcher";

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
            JOptionPane.showMessageDialog(null, stackTrace, "Autoupdate Error " + TITLE, JOptionPane.ERROR_MESSAGE);
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

    private static void autoupdate(String[] args) {
        BundleUtil.BundleInfo bundleInfo = BundleUtil.readBundleFullNameNotNull();
        if (BundleUtil.BundleInfo.isUndefined(bundleInfo)) {
            log.error("ERROR: Autoupdate: unable to perform without bundleFullName (check parent folder name)");
            System.exit(-1);
        }

        @NotNull String firstArgument = args.length > 0 ? args[0] : "";

        final Optional<DownloadedAutoupdateFileInfo> downloadedAutoupdateFile = downloadFreshZipFile(args, firstArgument, bundleInfo);
        downloadedAutoupdateFile.ifPresent(downloadedFile -> ObsoleteFilesArchiver.INSTANCE.archiveObsoleteFiles());
        URLClassLoader jarClassLoader = safeUnzipMakingSureClassloaderIsHappy(downloadedAutoupdateFile);
        log.info("extremely dark magic: XML binding seems to depend on this");
        Thread.currentThread().setContextClassLoader(jarClassLoader);
        startConsole(args, jarClassLoader);
    }

    private static Optional<DownloadedAutoupdateFileInfo> downloadFreshZipFile(String[] args, String firstArgument, BundleUtil.BundleInfo bundleInfo) {
        Optional<DownloadedAutoupdateFileInfo> downloadedAutoupdateFile;
        if (firstArgument.equalsIgnoreCase("basic-ui")) {
            downloadedAutoupdateFile = doDownload(bundleInfo);
        } else if (args.length > 0 && args[0].equalsIgnoreCase("release")) {
            // this branch needs progress for custom boards!
            log.info("Release update requested");
            downloadedAutoupdateFile = downloadAutoupdateZipFile(
                bundleInfo,
                ConnectionAndMeta.BASE_URL_RELEASE
            );
        } else {
            downloadedAutoupdateFile = doDownload(bundleInfo);
        }
        return downloadedAutoupdateFile;
    }

    private static URLClassLoader safeUnzipMakingSureClassloaderIsHappy(Optional<DownloadedAutoupdateFileInfo> downloadedAutoupdateFile) {
        downloadedAutoupdateFile.ifPresent(Autoupdate::unzipFreshConsole);
        URLClassLoader jarClassLoader = prepareClassLoaderToStartConsole();
        downloadedAutoupdateFile.ifPresent(autoupdateFile -> {
            try {
/*
                // technical dept here, guilty as charged!
                boolean installedIntoProgramFilesHack = new File("uninstall.exe").exists();
                log.info("installedIntoProgramFilesHack " + installedIntoProgramFilesHack);
                String pathname = installedIntoProgramFilesHack ? "." : "..";
*/
                String pathname = "..";
                log.info("unzipping everything else into " + pathname);
                // We've already prepared class loader, so now we can unzip rusefi_autoupdate.jar and other files
                // except already unzipped rusefi_console.jar (see #6777):
                FileUtil.unzip(autoupdateFile.zipFileName, new File(pathname), isConsoleJar.negate());
                final String srecFile = findSrecFile();
                new File(srecFile == null ? FindFileHelper.FIRMWARE_BIN_FILE : srecFile)
                    .setLastModified(autoupdateFile.lastModified);
            } catch (IOException e) {
                log.error("Error unzipping autoupdate from bundle: " + e);
                if (!AutoupdateUtil.runHeadless) {
                    JOptionPane.showMessageDialog(
                        null,
                        "Error unzipping autoupdate from bundle: " + e,
                        "Error",
                        JOptionPane.ERROR_MESSAGE
                    );
                }
            }
        });
        return jarClassLoader;
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
                JOptionPane.showMessageDialog(
                    null,
                    "Error unzipping bundle without autoupdate: " + e,
                    "Error",
                    JOptionPane.ERROR_MESSAGE
                );
            }
        }
    }

    private static final String consoleJarZipEntry =
        String.format("console/%s", ConnectionAndMeta.getRusEfiConsoleJarName());

    private static final Predicate<ZipEntry> isConsoleJar = zipEntry -> consoleJarZipEntry.equals(zipEntry.getName());

    private static Optional<DownloadedAutoupdateFileInfo> doDownload(final BundleUtil.BundleInfo bundleInfo) {
        if (bundleInfo.isMaster()) {
            log.info("Snapshot requested");
            return downloadAutoupdateZipFile(bundleInfo, ConnectionAndMeta.getBaseUrl() + ConnectionAndMeta.AUTOUPDATE);
        } else {
            final String branchName = selectBranchName(bundleInfo);
            return downloadAutoupdateZipFile(bundleInfo, ConnectionAndMeta.getBaseUrl() + "/lts/" + branchName + ConnectionAndMeta.AUTOUPDATE);
        }
    }

    private static String selectBranchName(BundleUtil.BundleInfo bundleInfo) {
        final String branchName = bundleInfo.getBranchName();
        final String nextBranchName = bundleInfo.getNextBranchName();
        if (nextBranchName != null && !nextBranchName.isBlank()) {
            if (JOptionPane.showConfirmDialog(
                null,
                String.format("A new version `%s` is available!\nWould you like to update from `%s` to `%s` now?",
                    nextBranchName,
                    branchName,
                    nextBranchName
                ),
                "Release selection",
                JOptionPane.YES_NO_OPTION
            ) == JOptionPane.YES_OPTION) {
                return nextBranchName;
            }
        }
        return branchName;
    }

    private static URLClassLoader prepareClassLoaderToStartConsole() {
        final URLClassLoader jarClassLoader;
        String consoleJarFileName = ConnectionAndMeta.getRusEfiConsoleJarName();
        if (!new File(consoleJarFileName).exists()) {
            throw log.log(new RuntimeException("Looks like corrupted installation: " + consoleJarFileName + " not found"));
        }

        try {
            jarClassLoader = AutoupdateUtil.getClassLoaderByJar(consoleJarFileName);
        } catch (MalformedURLException e) {
            log.error("Failed to start", e);
            throw new IllegalStateException("Problem with " + consoleJarFileName, e);
        }
        // we want to make sure that files are available to write so we use reflection to get lazy class initialization
        // since we are overriding file we cannot just use static java classpath while launching
        try {
            hackProperties(jarClassLoader);
        } catch (ClassNotFoundException e) {
            throw log.log(new IllegalStateException("Class not found: " + e, e));
        } catch (NoSuchMethodException | InvocationTargetException |
                 IllegalAccessException e) {
            throw log.log(new IllegalStateException("Failed to update properties: " + e, e));
        }
        return jarClassLoader;
    }

    private static void startConsole(final String[] args, final URLClassLoader jarClassLoader) {
        log.info("Running console with " + Arrays.toString(args));
        try {
            Class mainClass = Class.forName(COM_RUSEFI_LAUNCHER, true, jarClassLoader);
            Method mainMethod = mainClass.getMethod("main", args.getClass());
            mainMethod.invoke(null, new Object[]{args});
        } catch (ClassNotFoundException | IllegalAccessException | InvocationTargetException |
                 NoSuchMethodException e) {
            log.error("Failed to start", e);
            throw new IllegalStateException("Invoking console: " + e, e);
        }
    }

    private static void hackProperties(URLClassLoader jarClassLoader) throws ClassNotFoundException, NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        // in case of fresh jar file for some reason we are failing with ZipException if executed within console domain
        Class uiProperties = Class.forName("com.rusefi.UiProperties", true, jarClassLoader);
        for (Method m : uiProperties.getMethods())
            System.out.println(m);
        Method setter = uiProperties.getMethod("setProperties", Properties.class);
        setter.invoke(null, ConnectionAndMeta.getProperties());
    }

    private static class DownloadedAutoupdateFileInfo {
        final String zipFileName;
        final long lastModified;

        DownloadedAutoupdateFileInfo(final String zipFileName, final long lastModified) {
            this.zipFileName = zipFileName;
            this.lastModified = lastModified;
        }
    }

    private static Optional<DownloadedAutoupdateFileInfo> downloadAutoupdateZipFile(
        final BundleUtil.BundleInfo info,
        final String baseUrl
    ) {
        try {
            String suffix = FindFileHelper.isObfuscated() ? "_obfuscated_public" : "";
            String zipFileName = ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties()) + "_bundle_" + info.getTarget() + suffix + "_autoupdate" + ".zip";
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(zipFileName).invoke(baseUrl);
            log.info("Remote file " + zipFileName);
            log.info("Server has " + connectionAndMeta.getCompleteFileSize() + " from " + new Date(connectionAndMeta.getLastModified()));

            if (AutoupdateUtil.hasExistingFile(zipFileName, connectionAndMeta.getCompleteFileSize(), connectionAndMeta.getLastModified())) {
                log.info("We already have latest update " + new Date(connectionAndMeta.getLastModified()));
                return Optional.empty();
            }

            // todo: user could have waited hours to respond to question above, we probably need to re-establish connection
            long completeFileSize = connectionAndMeta.getCompleteFileSize();
            final long lastModified = connectionAndMeta.getLastModified();

            log.info(info + " " + completeFileSize + " bytes, last modified " + new Date(lastModified));

            AutoupdateUtil.downloadAutoupdateFile(zipFileName, connectionAndMeta, TITLE);

            File file = new File(zipFileName);
            file.setLastModified(lastModified);
            log.info("Downloaded " + file.length() + " bytes, lastModified=" + lastModified);

            return Optional.of(new DownloadedAutoupdateFileInfo(zipFileName, lastModified));
        } catch (ReportedIOException e) {
            // we had already reported error with a UI dialog when we had parent frame
            log.error("Error downloading bundle: " + e);
        } catch (IOException e) {
            // we are here if error happened while we did not have UI frame
            // todo: open frame prior to network connection and keep frame opened while uncompressing?
            log.error("Error downloading bundle: " + e);
            if (!AutoupdateUtil.runHeadless) {
                JOptionPane.showMessageDialog(null, "Error downloading " + e, "Error",
                    JOptionPane.ERROR_MESSAGE);
            }
        }
        return Optional.empty();
    }
}
