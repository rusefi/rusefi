package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.rusefi.NamedThreadFactory;
import com.rusefi.SerialPortScanner;

import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.BinaryProtocol.iniFileProvider;

public class EcuPortInfo extends SerialPortScanner.PortResult {
    private static final Logging log = getLogging(EcuPortInfo.class);

    private volatile IniFileModel ecuIniFile = null;

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
        final Runnable iniFileLoadedCallback
    ) {
        super(ecuPort);
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
}
