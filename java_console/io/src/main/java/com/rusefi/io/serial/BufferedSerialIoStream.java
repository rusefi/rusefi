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
    private BufferedSerialIoStream(SerialPort sp, String port) {
        super(sp, port);
        dataBuffer = createDataBuffer("[serial] ");
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    /**
     * Just open physical serial and not much more
     * @see PortHolder#connectAndReadConfiguration(com.rusefi.binaryprotocol.BinaryProtocol.Arguments)
     */
    @Nullable
    public static IoStream openPort(String port) {
        log.info("[BufferedSerialIoStream] openPort " + port);
        SerialPort serialPort = openSerial(port);
        if (serialPort == null)
            return null;
//        FileLog.LOGGER.info("[SerialIoStreamJSerialComm] opened " + port);
        return new BufferedSerialIoStream(serialPort, port);
    }

    @Override
    public String toString() {
        return "serial=" + port;
    }
}
