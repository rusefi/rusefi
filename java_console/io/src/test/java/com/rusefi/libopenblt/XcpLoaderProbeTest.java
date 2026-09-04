package com.rusefi.libopenblt;

import com.rusefi.libopenblt.transport.IXcpTransport;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class XcpLoaderProbeTest {
    @Test
    void availabilityProbeConnectsWithoutStartingProgramming() throws IOException {
        RecordingTransport transport = new RecordingTransport();
        XcpLoader loader = new XcpLoader(transport, new XcpSettings());

        loader.probeAvailability();

        assertEquals(1, transport.connects);
        assertEquals(1, transport.disconnects);
        assertEquals(java.util.Collections.singletonList(0xFF), transport.commands);
    }

    @Test
    void failedAvailabilityProbeStillDisconnects() {
        RecordingTransport transport = new RecordingTransport();
        transport.failConnectCommand = true;
        XcpLoader loader = new XcpLoader(transport, new XcpSettings());

        assertThrows(IOException.class, loader::probeAvailability);

        assertEquals(1, transport.connects);
        assertEquals(1, transport.disconnects);
        assertEquals(java.util.Collections.singletonList(0xFF), transport.commands);
    }

    private static class RecordingTransport implements IXcpTransport {
        final List<Integer> commands = new ArrayList<>();
        int connects;
        int disconnects;
        boolean failConnectCommand;

        @Override
        public void connect() {
            connects++;
        }

        @Override
        public void disconnect() {
            disconnects++;
        }

        @Override
        public byte[] sendPacket(byte[] request, int timeoutMs, int expectResponseBytes) throws IOException {
            commands.add(request[0] & 0xFF);
            if (failConnectCommand) {
                throw new IOException("No OpenBLT response");
            }
            return new byte[]{(byte) 0xFF, 0, 0, 8, 8, 0, 0, 0};
        }
    }
}
