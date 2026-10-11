package com.rusefi.libopenblt;

import com.rusefi.libopenblt.transport.IXcpTransport;
import org.junit.jupiter.api.Test;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import static org.junit.jupiter.api.Assertions.*;

class XcpLoaderReadTest {
    @Test void oldBootloaderIsRejectedBeforeProgramStart() {
        MemoryTransport transport = new MemoryTransport();
        XcpLoader loader = new XcpLoader(transport, new XcpSettings());
        loader.requireStorageLayout(0x90100000, 65536, 0xa0000000, 52428800);
        assertThrows(IOException.class, loader::start);
        assertEquals(0, transport.starts);
        assertEquals(1, transport.disconnects);
    }
    @Test void readsExternalUnsignedAddressAcrossPacketsAndAbortsWithoutCommit() throws Exception {
        MemoryTransport transport = new MemoryTransport();
        XcpLoader loader = new XcpLoader(transport, new XcpSettings());
        loader.start();
        byte[] actual = loader.readData(0x90100000, 130);
        assertEquals(3, transport.uploads);
        for (int i = 0; i < actual.length; i++) {
            assertEquals((byte) i, actual[i]);
        }
        assertThrows(IllegalArgumentException.class, () -> loader.readData(0xfffffff0, 32));
        loader.abort();
        assertEquals(1, transport.disconnects);
        assertEquals(0, transport.programs);
        assertThrows(IllegalStateException.class, () -> loader.readData(0x90100000, 1));
    }

    @Test void rejectsNegativeAndShortUploadResponses() throws Exception {
        MemoryTransport transport = new MemoryTransport();
        XcpLoader loader = new XcpLoader(transport, new XcpSettings());
        loader.start();
        transport.error = true;
        assertThrows(IOException.class, () -> loader.readData(0x90100000, 1));
        transport.error = false;
        transport.shortReply = true;
        assertThrows(IOException.class, () -> loader.readData(0x90100000, 63));
        loader.abort();
    }

    static class MemoryTransport implements IXcpTransport {
        int address, uploads, disconnects, programs, starts;
        boolean error, shortReply;
        public void connect() {}
        public void disconnect() { disconnects++; }
        public byte[] sendPacket(byte[] request, int timeout, int expected) {
            switch (request[0] & 0xff) {
                case 0xff: return new byte[]{(byte) 0xff, 0, 0, 64, 64, 0, 0, 0};
                case 0xd2: starts++; return new byte[]{(byte) 0xff, 0, 0, 64, 0, 0, 0};
                case 0xf6:
                    address = ByteBuffer.wrap(request, 4, 4).order(ByteOrder.LITTLE_ENDIAN).getInt();
                    return new byte[]{(byte) 0xff};
                case 0xf5:
                    uploads++;
                    if (error) { return new byte[]{(byte) 0xfe, 0x22}; }
                    int count = request[1] & 0xff;
                    byte[] response = new byte[count + (shortReply ? 0 : 1)];
                    response[0] = (byte) 0xff;
                    for (int i = 1; i < response.length; i++) { response[i] = (byte) address++; }
                    return response;
                case 0xd0: programs++; break;
                default: break;
            }
            return new byte[]{(byte) 0xff};
        }
    }
}
