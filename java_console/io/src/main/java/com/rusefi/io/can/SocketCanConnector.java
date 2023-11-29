package com.rusefi.io.can;

import com.rusefi.uds.CanConnector;
import tel.schich.javacan.RawCanChannel;

import java.io.IOException;

public class SocketCanConnector {
    public static CanConnector create() {
        RawCanChannel socket = SocketCANHelper.createSocket();
        return new CanConnector() {
            @Override
            public void send(int id, byte[] payLoad) {
                SocketCANHelper.send(id, payLoad, socket);
            }

            @Override
            public CanPacket read() {
                try {
                    return SocketCANHelper.read(socket);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }
}
