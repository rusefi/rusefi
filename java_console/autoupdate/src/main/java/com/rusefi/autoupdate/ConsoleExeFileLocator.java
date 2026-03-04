package com.rusefi.autoupdate;

import com.devexperts.logging.Logging;
import com.rusefi.core.net.JarFileUtil;

import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Optional;
import java.util.Properties;

import static com.devexperts.logging.Logging.getLogging;

public class ConsoleExeFileLocator {
    private static final Logging log = getLogging(ConsoleExeFileLocator.class);

    private static final String INSTALLATION_PROPERTIES_FILE_NAME = "installation.properties";

    private static final String CONSOLE_EXE_FILE_NAME_PROPERTY_NAME = "console-exe-file";

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
        final String defaultConsoleExeFileName = JarFileUtil.getJarFileNamePrefix() + "_console.exe";
        log.info(String.format("We are using default console exe file name: `%s`", defaultConsoleExeFileName));
        return defaultConsoleExeFileName;
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
