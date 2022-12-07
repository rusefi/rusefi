package com.rusefi.tools;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.io.can.Elm327Connector;
import com.rusefi.io.serial.SerialIoStream;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.ui.StatusConsumer;

import java.io.IOException;

public class Elm327ConnectorStartup {
    private final static Logging log = Logging.getLogging(Elm327ConnectorStartup.class);

    public static void start() throws IOException {
        SerialAutoChecker.AutoDetectResult detectResult = PortDetector.autoDetectSerial(null, PortDetector.DetectorMode.DETECT_ELM327);
        String autoDetectedPort = detectResult.getSerialPort();
        //String autoDetectedPort = "COM73";
        if (autoDetectedPort == null) {
            System.err.println(ConsoleTools.RUS_EFI_NOT_DETECTED);
            return;
        }

        Elm327Connector elm327Connector = new Elm327Connector(SerialIoStream.openPort(autoDetectedPort));
        elm327Connector.start(autoDetectedPort);

        BinaryProtocolProxy.createProxy(elm327Connector.getTsStream(), TcpConnector.DEFAULT_PORT, new BinaryProtocolProxy.ClientApplicationActivityListener() {
            @Override
            public void onActivity() {
                System.out.println("onActivity");
                Elm327Connector.whyDoWeNeedToSleepBetweenCommands();
            }
        }, StatusConsumer.ANONYMOUS);

        log.info("Running Elm327 connector for " + autoDetectedPort);
    }
}
