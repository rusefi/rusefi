package com.rusefi.uds;

public interface CanConnector {
    int UDS_IN = 0x7E8;

    void send(int id, byte[] payLoad);

    CanPacket read();

    interface CanPacket {
        int id();

        byte[] payload();
    }
}
