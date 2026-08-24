package com.rusefi.libopenblt;

import com.rusefi.libopenblt.transport.IXcpTransport;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class XcpLoaderStationIdTest {
    @Test
    void legacyClassificationRejectsLiveSignedTargetBeforeProgramStart() {
        StationTransport transport = new StationTransport();
        XcpLoader loader = new XcpLoader(transport, new XcpSettings());
        loader.requireStationIdCheck(null);

        assertThrows(IOException.class, loader::start);

        assertEquals(1, transport.signatureQueries);
        assertEquals(0, transport.programStarts);
        assertEquals(1, transport.disconnects);
    }

    @Test
    void legacyClassificationAcceptsLiveLegacyTarget() throws IOException {
        StationTransport transport = new StationTransport();
        transport.liveSigned = false;
        XcpLoader loader = new XcpLoader(transport, new XcpSettings());
        loader.requireStationIdCheck(null);

        loader.start();
        loader.stop();

        assertEquals(1, transport.signatureQueries);
        assertEquals(1, transport.programStarts);
        assertEquals(1, transport.disconnects);
    }

    private static class StationTransport implements IXcpTransport {
        int signatureQueries;
        int programStarts;
        int disconnects;
        boolean liveSigned = true;

        @Override
        public void connect() {
        }

        @Override
        public void disconnect() {
            disconnects++;
        }

        @Override
        public byte[] sendPacket(byte[] request, int timeoutMs, int expectResponseBytes) {
            int command = request[0] & 0xFF;
            if (command == 0xFF) {
                return new byte[]{(byte) 0xFF, 0, 0, (byte) 240, (byte) 240, 0, 0, 0};
            }
            if (command == 0xBF) {
                signatureQueries++;
                if (!liveSigned) {
                    return new byte[]{(byte) 0xFE, 0x20};
                }
                byte[] station = "rusEFI.stm32f429_nucleo".getBytes(StandardCharsets.US_ASCII);
                byte[] response = new byte[station.length + 1];
                response[0] = (byte) 0xFF;
                System.arraycopy(station, 0, response, 1, station.length);
                return response;
            }
            if (command == 0xD2) {
                programStarts++;
                return new byte[]{(byte) 0xFF, 0, 0, (byte) 240, 0, 0, 0};
            }
            return new byte[]{(byte) 0xFF};
        }
    }
}
