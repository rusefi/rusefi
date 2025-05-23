package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.NamedThreadFactory;
import com.rusefi.SerialPortScanner;
import com.rusefi.maintenance.CalibrationsInfo;

import java.util.Collections;
import java.util.Optional;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.BinaryProtocol.iniFileProvider;

public class EcuPortInfo extends SerialPortScanner.PortResult {
    public static final String HASH3_FIELD_NAME = "hash3";

    private static final Logging log = getLogging(EcuPortInfo.class);

    private volatile IniFileModel ecuIniFile = null;

    private volatile CalibrationsInfo calibrationsInfo = null;

    private final Runnable calibrationsLoadedCallback;

    public final ExecutorService iniFileLoaderExecutor = new ThreadPoolExecutor(
        1,
        1,
        0L,
        TimeUnit.MILLISECONDS,
        new LinkedBlockingQueue<>(),
        new NamedThreadFactory("Load .ini-file Executor",
            true
        )
    );

    public EcuPortInfo(
        final SerialPortScanner.PortResult ecuPort,
        final Runnable iniFileLoadedCallback,
        final Runnable calibrationsLoadedCallback
    ) {
        super(ecuPort);
        this.calibrationsLoadedCallback = calibrationsLoadedCallback;
        if (ecuPort.signature != null) {
            iniFileLoaderExecutor.submit(() -> {
                final String signature = ecuPort.signature.toString();
                log.info(String.format("We are getting .ini-file for signature `%s`", signature));
                ecuIniFile = iniFileProvider.provide(signature);
                log.info(String.format(".ini-file for signature `%s` is : `%s`", signature, ecuIniFile.getIniFilePath()));
                iniFileLoadedCallback.run();
            });
        }
    }

    public boolean existsAnyOfIniFields(final Set<String> iniFieldNames) {
        final IniFileModel iniFile = ecuIniFile;
        if (iniFile != null) {
            for (final String iniFieldName: iniFieldNames) {
                if (iniFile.findIniField(iniFieldName).isPresent()) {
                    return true;
                }
            }
        }
        return false;
    }

    public Optional<String> getFirmwareHash() {
        final CalibrationsInfo calibrations = calibrationsInfo;
        if (calibrations != null) {
            final Optional<IniField> hash3IniField = calibrations.getIniFile().findIniField(HASH3_FIELD_NAME);
            if (hash3IniField.isPresent()) {
                final String hash3Value = hash3IniField.get().getValue(calibrations.getImage().getConfigurationImage());
                return Optional.of(hash3Value);
            }
        }
        return Optional.empty();
    }

    public boolean shouldLoadCalibrationsInfo() {
        return (calibrationsInfo == null) && existsAnyOfIniFields(Collections.singleton(HASH3_FIELD_NAME));
    }

    public void setCalibrationsInfo(final CalibrationsInfo calibrations) {
        calibrationsInfo = calibrations;
        calibrationsLoadedCallback.run();
    }
}
