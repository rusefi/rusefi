package com.rusefi.updater;

import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.IoStream;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.List;

public class OpenbltDetectorStrategy {
    private static final byte XCP_CMD_CONNECT = (byte) 0xFF;
    private static final byte XCP_CMD_GET_SIGNATURE = (byte) 0xBF;
    private static final byte XCP_CMD_GET_VERSION = '?';
    private static final byte XCP_PID_RES = (byte) 0xFF;

    public static class OpenbltInfo {
        public final boolean isOpenblt;
        public final String brand;
        public final String board;
        public final String raw;
        public final String version;
        public final List<String> features;

        public OpenbltInfo(boolean isOpenblt, String raw, String version, List<String> features) {
            this.isOpenblt = isOpenblt;
            this.raw = raw;
            this.version = version;
            this.features = features;

            int separator = raw == null ? -1 : raw.indexOf('.');
            if (separator > 0 && separator < raw.length() - 1) {
                brand = raw.substring(0, separator);
                board = raw.substring(separator + 1);
            } else {
                brand = null;
                board = null;
            }
        }

        public OpenbltInfo(boolean isOpenblt, String raw, List<String> features) {
            this(isOpenblt, raw, null, features);
        }

        public OpenbltInfo(boolean isOpenblt, String raw) {
            this(isOpenblt, raw, null, null);
        }

        private static OpenbltInfo notOpenblt() {
            return new OpenbltInfo(false, null, null, null);
        }
    }

    private static class BltBanner {
        final String version;
        final List<String> features;

        BltBanner(String version, List<String> features) {
            this.version = version;
            this.features = features;
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

        BltBanner banner = queryBanner(stream);
        String version = banner == null ? null : banner.version;
        List<String> features = banner == null ? null : banner.features;

        try {

            stream.write(new byte[]{1, XCP_CMD_GET_SIGNATURE});

            int signatureResponseLength = idb.readByte(250);
            if (signatureResponseLength < 1) {
                return new OpenbltInfo(true, null, version, features);
            }

            byte[] signatureResponse = new byte[signatureResponseLength];
            idb.waitForBytes(100, "getOpenbltSignature", System.currentTimeMillis(), signatureResponseLength);
            idb.read(signatureResponse);
            if (signatureResponse[0] != XCP_PID_RES) {
                return new OpenbltInfo(true, null, version, features);
            }

            String signature = new String(signatureResponse, 1, signatureResponseLength - 1, StandardCharsets.US_ASCII);
            return new OpenbltInfo(true, signature, version, features);
        } catch (IOException ignored) {
            // Signature discovery is optional so older bootloaders remain usable during rollout.
            return new OpenbltInfo(true, null, version, features);
        }
    }

    /**
     * Queries the bootloader plain-text version/features banner: packet '?' (0x3F)
     * replies with "blt &lt;version&gt;" and optionally
     * "features=&lt;comma-separated list&gt;". Returns null when the bootloader does
     * not answer the probe.
     */
    private static BltBanner queryBanner(IoStream stream) {
        try {
            stream.write(new byte[]{1, XCP_CMD_GET_VERSION});

            IncomingDataBuffer idb = stream.getDataBuffer();
            int bannerLen = idb.readByte(250);
            if (bannerLen < 1 || bannerLen > 128) {
                return null;
            }

            byte[] payload = new byte[bannerLen];
            idb.waitForBytes(100, "bltBanner", System.currentTimeMillis(), bannerLen);
            idb.read(payload);

            String banner = new String(payload, StandardCharsets.US_ASCII).trim();
            String[] tokens = banner.split("\\s+");
            if (tokens.length < 2 || !"blt".equals(tokens[0])) {
                return null;
            }
            List<String> features = null;
            for (int i = 2; i < tokens.length; i++) {
                if (tokens[i].startsWith("features=")) {
                    String featureList = tokens[i].substring("features=".length());
                    if (!featureList.isEmpty()) {
                        features = Arrays.asList(featureList.split(","));
                    }
                    break;
                }
            }
            return new BltBanner(tokens[1], features);
        } catch (IOException ignored) {
            // Feature discovery is optional, older bootloaders simply have no features.
            return null;
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
