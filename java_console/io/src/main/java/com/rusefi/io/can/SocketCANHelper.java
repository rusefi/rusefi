package com.rusefi.io.can;

import com.devexperts.logging.Logging;
import com.rusefi.uds.CanConnector;
import org.jetbrains.annotations.NotNull;
import tel.schich.javacan.CanChannels;
import tel.schich.javacan.CanFrame;
import tel.schich.javacan.NetworkDevice;
import tel.schich.javacan.RawCanChannel;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;
import static tel.schich.javacan.CanFrame.FD_NO_FLAGS;
import static tel.schich.javacan.CanSocketOptions.RECV_OWN_MSGS;

public class SocketCANHelper {
    private static Logging log = getLogging(SocketCANIoStream.class);

    @NotNull
    public static RawCanChannel createSocket() {
        try {
            NetworkDevice canInterface = NetworkDevice.lookup(System.getProperty("CAN_DEVICE_NAME", "can0"));
            return createSocket(canInterface, CanChannels::newRawChannel);
        } catch (IOException e) {
            throw new IllegalStateException("Error looking up", e);
        }
    }

    static RawCanChannel createSocket(NetworkDevice canInterface, ChannelFactory channelFactory) {
        RawCanChannel socket = null;
        try {
            socket = channelFactory.create();
            socket.bind(canInterface);

            socket.configureBlocking(true); // we want reader thread to wait for messages
            socket.setOption(RECV_OWN_MSGS, false);
            return socket;
        } catch (IOException e) {
            if (socket != null) {
                try {
                    socket.close();
                } catch (IOException closeException) {
                    e.addSuppressed(closeException);
                }
            }
            throw new IllegalStateException("Error looking up", e);
        }
    }

    @FunctionalInterface
    interface ChannelFactory {
        RawCanChannel create() throws IOException;
    }

    public static void send(int id, byte[] payload, RawCanChannel channel) {
        CanFrame packet = CanFrame.create(id, FD_NO_FLAGS, payload);
        try {
            channel.write(packet);
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public static CanConnector.CanPacket read(RawCanChannel socket) throws IOException {
        CanFrame rx = socket.read();
        if (log.debugEnabled())
            log.debug("GOT " + String.format("%X", rx));
        byte[] raw = new byte[rx.getDataLength()];
        rx.getData(raw, 0, raw.length);
        return new CanConnector.CanPacket() {
            @Override
            public int id() {
                return rx.getId();
            }

            @Override
            public byte[] payload() {
                return raw;
            }
        };
    }
}
