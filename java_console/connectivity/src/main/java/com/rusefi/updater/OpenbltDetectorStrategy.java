package com.rusefi.updater;

import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.IoStream;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class OpenbltDetectorStrategy {
    private static final byte XCP_CMD_CONNECT = (byte) 0xFF;
    private static final byte XCP_CMD_GET_SIGNATURE = (byte) 0xBF;
    private static final byte XCP_PID_RES = (byte) 0xFF;

    public static class OpenbltInfo {
        public final boolean isOpenblt;
        public final String brand;
        public final String board;
        public final String raw;

        public OpenbltInfo(boolean isOpenblt, String raw) {
            this.isOpenblt = isOpenblt;
            this.raw = raw;

            int separator = raw == null ? -1 : raw.indexOf('.');
            if (separator > 0 && separator < raw.length() - 1) {
                brand = raw.substring(0, separator);
                board = raw.substring(separator + 1);
            } else {
                brand = null;
                board = null;
            }
        }

        private static OpenbltInfo notOpenblt() {
            return new OpenbltInfo(false, null);
        }
    }

    public static boolean isPortOpenblt(IoStream stream) throws IOException {
        return detectOpenbltWithSignature(stream).isOpenblt;
    }

    public static OpenbltInfo detectOpenbltWithSignature(IoStream stream) throws IOException {
        if (stream == null) {
            return OpenbltInfo.notOpenblt();
        }

        stream.write(new byte[]{2, XCP_CMD_CONNECT, 0});

        IncomingDataBuffer idb = stream.getDataBuffer();

        int responseLength = idb.readByte(250);

        // Invalid length, ignore
        if (responseLength != 8) {
            return OpenbltInfo.notOpenblt();
        }

        // Read length worth of bytes
        byte[] response = new byte[responseLength];
        idb.waitForBytes(100, "isPortOpenblt", System.currentTimeMillis(), responseLength);
        idb.read(response);

        // Response packet should start with FF
        // Not much else to check, as the rest of the response is protocol settings from the device.
        if (response[0] != XCP_PID_RES) {
            return OpenbltInfo.notOpenblt();
        }

        try {
            stream.write(new byte[]{1, XCP_CMD_GET_SIGNATURE});

            int signatureResponseLength = idb.readByte(250);
            if (signatureResponseLength < 1) {
                return new OpenbltInfo(true, null);
            }

            byte[] signatureResponse = new byte[signatureResponseLength];
            idb.waitForBytes(100, "getOpenbltSignature", System.currentTimeMillis(), signatureResponseLength);
            idb.read(signatureResponse);
            if (signatureResponse[0] != XCP_PID_RES) {
                return new OpenbltInfo(true, null);
            }

            String signature = new String(signatureResponse, 1, signatureResponseLength - 1, StandardCharsets.US_ASCII);
            return new OpenbltInfo(true, signature);
        } catch (IOException ignored) {
            // Signature discovery is optional so older bootloaders remain usable during rollout.
            return new OpenbltInfo(true, null);
        }
    }

    public static boolean streamHasOpenBlt(IoStream stream) throws IOException {
        stream.sendPacket(new byte[]{(byte) Integration.TS_QUERY_BOOTLOADER});

        byte[] response = stream.getDataBuffer().getPacket(500, "ecuHasOpenblt");
        if (!IoHelper.checkResponseCode(response, (byte) Integration.TS_RESPONSE_OK)) {
            // ECU didn't understand request, bootloader certainly not supported
            return false;
        }

        // Data byte indicates bootloader type
        return response[1] == Integration.TS_QUERY_BOOTLOADER_OPENBLT;
    }
}
