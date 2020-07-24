package com.rusefi.io.tcp;

import com.opensr5.Logger;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.IoStream;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.function.Function;

import static com.rusefi.binaryprotocol.BinaryProtocolCommands.COMMAND_PROTOCOL;
import static com.rusefi.config.generated.Fields.TS_PROTOCOL;

public class BinaryProtocolProxy {
    /**
     * we expect server to at least request output channels once in a while
     * it could be a while between user connecting authenticator and actually connecting application to authenticator
     */
    public static final int USER_IO_TIMEOUT = 600 * Timeouts.SECOND;

    public static ServerHolder createProxy(Logger logger, IoStream targetEcuSocket, int serverProxyPort) {
        Function<Socket, Runnable> clientSocketRunnableFactory = clientSocket -> () -> {
            try {
                TcpIoStream clientStream = new TcpIoStream("[[proxy]] ", logger, clientSocket);
                runProxy(targetEcuSocket, clientStream);
            } catch (IOException e) {
                logger.error("BinaryProtocolProxy::run" + e);
            }
        };
        return BinaryProtocolServer.tcpServerSocket(serverProxyPort, "proxy", clientSocketRunnableFactory, Logger.CONSOLE, null);
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

        System.out.println("Relaying controller response length=" + packet.getPacket().length);
        clientOutputStream.sendPacket(packet);
    }

    private static void proxyClientRequestToController(IncomingDataBuffer in, byte firstByte, IoStream targetOutputStream) throws IOException {
        byte secondByte = in.readByte();
        int length = firstByte * 256 + secondByte;

        BinaryProtocolServer.Packet packet = BinaryProtocolServer.readPromisedBytes(in, length);

        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(packet.getPacket()));
        byte command = (byte) dis.read();

        System.out.println("Relaying client command " + BinaryProtocol.findCommand(command));
        // sending proxied packet to controller
        targetOutputStream.sendPacket(packet);
    }
}
