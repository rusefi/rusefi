package com.rusefi.tools;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.io.can.Elm327Connector;

public class Elm327ConnectorStartup {
    private final static Logging log = Logging.getLogging(Elm327ConnectorStartup.class);

    public static void start() {
        //String autoDetectedPort = PortDetector.autoDetectSerial(null, PortDetector.DetectorMode.DETECT_ELM327);
        String autoDetectedPort = "COM73";
        if (autoDetectedPort == null) {
            System.err.println(ConsoleTools.RUS_EFI_NOT_DETECTED);
            return;
        }

        (new Elm327Connector()).start(autoDetectedPort);
        log.info("Running Elm327 connector for " + autoDetectedPort);
    }
}
