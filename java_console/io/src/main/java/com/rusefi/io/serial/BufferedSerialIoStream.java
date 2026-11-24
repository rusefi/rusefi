package com.rusefi.io.serial;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.IoStream;
import org.jetbrains.annotations.Nullable;

import static com.devexperts.logging.Logging.getLogging;

/**
 * https://github.com/Fazecast/jSerialComm looks to be alive as of 2020
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 06/03/2019
 */
public class BufferedSerialIoStream extends SerialIoStream {
    private static final Logging log = getLogging(BufferedSerialIoStream.class);
    private final IncomingDataBuffer dataBuffer;

    /**
     * @see #openPort(String)
     */
    private BufferedSerialIoStream(NativePort nativePort, String port) {
        super(nativePort, port);
        dataBuffer = createDataBuffer();
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    /**
     * Just open physical serial and not much more
     * @see PortHolder
     */
    @Nullable
    public static IoStream openPort(String port) {
        log.info("[BufferedSerialIoStream] openPort " + port);
        return openPort(port, false, new NativePort(SerialPort.getCommPort(port)));
    }

    /** Requests a finite native write timeout on Windows only, not a whole-probe deadline. */
    @Nullable
    public static IoStream openPortForProbe(String port) {
        log.info("[BufferedSerialIoStream] openPortForProbe " + port);
        return openPort(port, true, new NativePort(SerialPort.getCommPort(port)));
    }

    @Nullable
    static IoStream openPort(String port, boolean probe, NativePort nativePort) {
        NativePort openedPort = openSerial(port, probe, nativePort);
        if (openedPort == null) {
            return null;
        }
//        FileLog.LOGGER.info("[SerialIoStreamJSerialComm] opened " + port);
        return new BufferedSerialIoStream(openedPort, port);
    }

    @Override
    public String toString() {
        return "serial=" + port;
    }
}
