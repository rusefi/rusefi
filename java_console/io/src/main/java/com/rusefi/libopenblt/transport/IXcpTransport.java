package com.rusefi.libopenblt.transport;

import java.io.IOException;

public interface IXcpTransport extends AutoCloseable {
    void connect() throws IOException;
    void disconnect() throws IOException;

    byte[] sendPacket(byte[] request, int timeoutMs, int expectResponseBytes) throws IOException;
}
