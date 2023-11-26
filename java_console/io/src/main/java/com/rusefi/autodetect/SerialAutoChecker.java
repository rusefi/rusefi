package com.rusefi.autodetect;

import com.devexperts.logging.Logging;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.serial.BufferedSerialIoStream;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public final class SerialAutoChecker {
    private final static Logging log = Logging.getLogging(SerialAutoChecker.class);

    // static class - no instances
    private SerialAutoChecker() { }

    /**
     * @return ECU signature from specified stream
     */
    public static String checkResponse(IoStream stream) {
        if (stream == null)
            return null;

        IncomingDataBuffer incomingData = stream.getDataBuffer();
        try {
            HelloCommand.send(stream);
            byte[] response = incomingData.getPacket(500, "auto detect");
            if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK))
                return null;
            String signature = new String(response, 1, response.length - 1);
            if (!signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX)) {
                return null;
            }

            return signature;
        } catch (IOException ignore) {
            return null;
        }
    }

    public static AutoDetectResult openAndCheckResponse(String serialPort) {
        String signature;
        // java 101: just a reminder that try-with syntax would take care of closing stream and that's important here!
        try (IoStream stream = BufferedSerialIoStream.openPort(serialPort)) {
            signature = checkResponse(stream);
            log.info("Got signature=" + signature + " from " + serialPort);
        }

        if (signature != null) {
            return new AutoDetectResult(serialPort, signature);
        }

        return null;
    }

    public static class AutoDetectResult {

        private final String serialPort;
        private final String signature;

        public AutoDetectResult(String serialPort, String signature) {
            this.serialPort = serialPort;
            this.signature = signature;
        }

        @Nullable
        public String getSerialPort() {
            return serialPort;
        }

        @Nullable
        public String getSignature() {
            return signature;
        }

        @Override
        public String toString() {
            return "AutoDetectResult{" +
                    "serialPort='" + serialPort + '\'' +
                    ", signature='" + signature + '\'' +
                    '}';
        }
    }
}
