package com.rusefi.updater;

import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.IoStream;

import java.io.IOException;

public class OpenbltDetectorStrategy {
    public static boolean isPortOpenblt(IoStream stream) throws IOException {
        if (stream == null) {
            return false;
        }

        byte[] request = new byte[3];
        request[0] = 2; // packet length
        request[1] = (byte) 0xff; // XCPLOADER_CMD_CONNECT
        request[2] = 0; // connectMode

        stream.write(request);

        IncomingDataBuffer idb = stream.getDataBuffer();

        byte responseLength = idb.readByte(250);

        // Invalid length, ignore
        if (responseLength != 8) {
            return false;
        }

        // Read length worth of bytes
        byte[] response = new byte[responseLength];
        idb.waitForBytes(100, "isPortOpenblt", System.currentTimeMillis(), responseLength);
        idb.read(response);

        // Response packet should start with FF
        // Not much else to check, as the rest of the response is protocol settings from the device.
        return response[0] == (byte) 0xFF;
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
