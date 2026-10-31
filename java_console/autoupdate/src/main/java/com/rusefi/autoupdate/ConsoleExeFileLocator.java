package com.rusefi.autoupdate;

import com.devexperts.logging.Logging;
import com.rusefi.core.OsUtil;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.net.JarFileUtil;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.CodeSource;
import java.util.Locale;
import java.util.Optional;
import java.util.Properties;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Finds the bundle launcher which {@link Autoupdate#startConsoleAsANewProcess} re-invokes once the
 * fresh console has been unpacked.
 * <p>
 * Two bundle layout facts drive this class (see {@code firmware/bundle.mk} and
 * {@code firmware/bin/assemble_universal_bundle.sh}):
 * <ul>
 *   <li>The launchers ({@code rusefi_updater.exe} on Windows, {@code rusefi_updater.sh} elsewhere)
 *       live at the <b>bundle root</b>, while the console jar lives in {@code console/}.</li>
 *   <li>Both launchers chdir into {@code console/} before starting the JVM, so at runtime our
 *       working directory is {@code console/} and the launcher sits one level <b>up</b>.</li>
 * </ul>
 * That is why the default name is resolved against the working directory first and its parent
 * second - see issue #8360, where the updater could not invoke the new process on Linux.
 */
public class ConsoleExeFileLocator {
    private static final Logging log = getLogging(ConsoleExeFileLocator.class);

    private static final String INSTALLATION_PROPERTIES_FILE_NAME = "installation.properties";

    private static final String CONSOLE_EXE_FILE_NAME_PROPERTY_NAME = "console-exe-file";

    static final String WINDOWS_LAUNCHER_SUFFIX = "_updater.exe";

    static final String UNIX_LAUNCHER_SUFFIX = "_updater.sh";

    public static boolean isRunningFromUnzippedBundle() {
        return isRunningFromUnzippedBundle(Paths.get("."), isRunningFromJar());
    }

    static boolean isRunningFromUnzippedBundle(final Path workingDirectory, final boolean runningFromJar) {
        if (!runningFromJar || Files.exists(workingDirectory.resolve(INSTALLATION_PROPERTIES_FILE_NAME))) {
            return false;
        }

        final Path releaseFile = workingDirectory.resolve(BundleUtil.BRANCH_REF_FILE);
        try {
            BundleInfo bundleInfo = BundleUtil.parse(Files.readAllLines(releaseFile));
            return !BundleInfo.isUndefined(bundleInfo);
        } catch (IOException e) {
            return false;
        }
    }

    private static boolean isRunningFromJar() {
        CodeSource codeSource = ConsoleExeFileLocator.class.getProtectionDomain().getCodeSource();
        return codeSource != null
            && codeSource.getLocation().getPath().toLowerCase(Locale.ROOT).endsWith(".jar");
    }

    String getConsoleExeFileName() {
        final Optional<Properties> installationProperties = loadInstallationProperties();
        if (installationProperties.isPresent()) {
            final String consoleExeFileName = installationProperties.get().getProperty(
                CONSOLE_EXE_FILE_NAME_PROPERTY_NAME
            );
            if (consoleExeFileName != null) {
                return consoleExeFileName;
            } else {
                log.info(String.format("`%s` property is not found", CONSOLE_EXE_FILE_NAME_PROPERTY_NAME));
            }
        } else {
            log.info(String.format("It looks like `%s` file is missed", INSTALLATION_PROPERTIES_FILE_NAME));
        }
        final String defaultConsoleExeFileName = resolveDefaultLauncher(
            JarFileUtil.getJarFileNamePrefix(),
            OsUtil.isWindows(),
            Paths.get(".")
        );
        log.info(String.format("We are using default console exe file name: `%s`", defaultConsoleExeFileName));
        return defaultConsoleExeFileName;
    }

    /**
     * Package-private seam for unit tests: pure function of the jar prefix, the platform and the
     * working directory, with no dependency on the real JVM environment.
     *
     * @return absolute path of the launcher when it is found either in {@code workingDirectory} or in
     *         its parent, otherwise the bare file name so that the caller still reports the familiar
     *         "file to launch isn't found" error.
     */
    static String resolveDefaultLauncher(
        final String jarFileNamePrefix,
        final boolean isWindows,
        final Path workingDirectory
    ) {
        final String launcherFileName = jarFileNamePrefix + (isWindows ? WINDOWS_LAUNCHER_SUFFIX : UNIX_LAUNCHER_SUFFIX);
        // the console runs with the bundle's `console` folder as working directory while the launcher
        // itself sits at the bundle root, hence we also look one level up
        final Path[] candidates = { workingDirectory, workingDirectory.toAbsolutePath().normalize().getParent() };
        for (final Path candidate : candidates) {
            if (candidate == null) {
                continue;
            }
            final Path launcherPath = candidate.resolve(launcherFileName);
            if (Files.isRegularFile(launcherPath)) {
                return launcherPath.toAbsolutePath().normalize().toString();
            }
        }
        log.info(String.format("Launcher `%s` is not found next to the console nor at the bundle root", launcherFileName));
        return launcherFileName;
    }

    private static Optional<Properties> loadInstallationProperties() {
        final Properties result = new Properties();
        Path path = Paths.get(INSTALLATION_PROPERTIES_FILE_NAME);
        try (final InputStream is = Files.newInputStream(path)) {
            result.load(is);
            return Optional.of(result);
        } catch (NoSuchFileException e) {
            log.info("NoSuchFile " + path);
        } catch (Throwable e) {
            log.error(String.format("Failed to load properties from `%s` file.", INSTALLATION_PROPERTIES_FILE_NAME), e);
        }
        return Optional.empty();
    }
}
