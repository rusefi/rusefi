package com.rusefi.io.tcp;

import com.devexperts.logging.Logging;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.IoStream;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.function.Function;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.BinaryProtocolCommands.COMMAND_PROTOCOL;
import static com.rusefi.config.generated.Fields.TS_PROTOCOL;

public class BinaryProtocolProxy {
    private static final Logging log = getLogging(BinaryProtocolProxy.class);
    /**
     * we expect server to at least request output channels once in a while
     * it could be a while between user connecting authenticator and actually connecting application to authenticator
     * See Backend#APPLICATION_INACTIVITY_TIMEOUT
     */
    public static final int USER_IO_TIMEOUT = 10 * Timeouts.MINUTE;

    public static ServerHolder createProxy(IoStream targetEcuSocket, int serverProxyPort) {
        Function<Socket, Runnable> clientSocketRunnableFactory = clientSocket -> () -> {
            try {
                TcpIoStream clientStream = new TcpIoStream("[[proxy]] ", clientSocket);
                runProxy(targetEcuSocket, clientStream);
            } catch (IOException e) {
                log.error("BinaryProtocolProxy::run " + e);
            }
        };
        return BinaryProtocolServer.tcpServerSocket(serverProxyPort, "proxy", clientSocketRunnableFactory, null);
    }

    public static void runProxy(IoStream targetEcu, IoStream clientStream) throws IOException {
        /*
         * Each client socket is running on it's own thread
         */
        //noinspection InfiniteLoopStatement
        while (true) {
            byte firstByte = clientStream.getDataBuffer().readByte(USER_IO_TIMEOUT);
            if (firstByte == COMMAND_PROTOCOL) {
                clientStream.write(TS_PROTOCOL.getBytes());
                continue;
            }
            proxyClientRequestToController(clientStream.getDataBuffer(), firstByte, targetEcu);

            proxyControllerResponseToClient(targetEcu, clientStream);
        }
    }

    public static void proxyControllerResponseToClient(IoStream targetInputStream, IoStream clientOutputStream) throws IOException {
        BinaryProtocolServer.Packet packet = targetInputStream.readPacket();

        log.info("Relaying controller response length=" + packet.getPacket().length);
        clientOutputStream.sendPacket(packet);
    }

    private static void proxyClientRequestToController(IncomingDataBuffer in, byte firstByte, IoStream targetOutputStream) throws IOException {
        byte secondByte = in.readByte();
        int length = firstByte * 256 + secondByte;

        BinaryProtocolServer.Packet packet = BinaryProtocolServer.readPromisedBytes(in, length);

        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(packet.getPacket()));
        byte command = (byte) dis.read();

        log.info("Relaying client command " + BinaryProtocol.findCommand(command));
        // sending proxied packet to controller
        targetOutputStream.sendPacket(packet);
    }
}
