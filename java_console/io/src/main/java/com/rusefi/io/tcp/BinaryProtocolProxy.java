package com.rusefi.io.tcp;

import com.devexperts.logging.Logging;
import com.rusefi.CompatibleFunction;
import com.rusefi.Listener;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.proxy.NetworkConnector;
import org.jetbrains.annotations.NotNull;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Function;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.BinaryProtocolCommands.COMMAND_PROTOCOL;
import static com.rusefi.config.generated.Fields.TS_PROTOCOL;
import static com.rusefi.shared.FileUtil.close;

public class BinaryProtocolProxy {
    private static final Logging log = getLogging(BinaryProtocolProxy.class);
    /**
     * we expect server to at least request output channels once in a while
     * it could be a while between user connecting authenticator and actually connecting application to authenticator
     * See Backend#APPLICATION_INACTIVITY_TIMEOUT
     */
    public static final int USER_IO_TIMEOUT = 10 * Timeouts.MINUTE;

    public static ServerSocketReference createProxy(IoStream targetEcuSocket, int serverProxyPort, AtomicInteger relayCommandCounter) throws IOException {
        CompatibleFunction<Socket, Runnable> clientSocketRunnableFactory = clientSocket -> () -> {
            TcpIoStream clientStream = null;
            try {
                clientStream = new TcpIoStream("[[proxy]] ", clientSocket);
                runProxy(targetEcuSocket, clientStream, relayCommandCounter, USER_IO_TIMEOUT);
            } catch (IOException e) {
                log.error("BinaryProtocolProxy::run " + e);
                close(clientStream);
            }
        };
        return BinaryProtocolServer.tcpServerSocket(serverProxyPort, "proxy", clientSocketRunnableFactory, Listener.empty());
    }

    public static void runProxy(IoStream targetEcu, IoStream clientStream, AtomicInteger relayCommandCounter, int timeoutMs) throws IOException {
        /*
         * Each client socket is running on it's own thread
         */
        while (!targetEcu.isClosed()) {
            byte firstByte = clientStream.getDataBuffer().readByte(timeoutMs);
            if (firstByte == COMMAND_PROTOCOL) {
                clientStream.write(TS_PROTOCOL.getBytes());
                clientStream.flush();
                continue;
            }
            BinaryProtocolServer.Packet clientRequest = readClientRequest(clientStream.getDataBuffer(), firstByte);
            byte[] packet = clientRequest.getPacket();
            if (packet.length > 1 && packet[0] == Fields.TS_ONLINE_PROTOCOL && packet[1] == NetworkConnector.DISCONNECT)
                throw new IOException("User requested disconnect");

            /**
             * Two reasons for synchronization:
             * - we run gauge poking thread until TunerStudio connects
             * - technically there could be two parallel connections to local application port
             */
            BinaryProtocolServer.Packet controllerResponse;
            synchronized (targetEcu) {
                sendToTarget(targetEcu, clientRequest);
                controllerResponse = targetEcu.readPacket();
                relayCommandCounter.incrementAndGet();
            }

            log.info("Relaying controller response length=" + controllerResponse.getPacket().length);
            clientStream.sendPacket(controllerResponse);
        }
    }

    @NotNull
    private static BinaryProtocolServer.Packet readClientRequest(IncomingDataBuffer in, byte firstByte) throws IOException {
        byte secondByte = in.readByte();
        int length = firstByte * 256 + secondByte;

        return BinaryProtocolServer.readPromisedBytes(in, length);
    }

    private static void sendToTarget(IoStream targetOutputStream, BinaryProtocolServer.Packet packet) throws IOException {
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(packet.getPacket()));
        byte command = (byte) dis.read();

        log.info("Relaying client command " + BinaryProtocol.findCommand(command));
        // sending proxied packet to controller
        targetOutputStream.sendPacket(packet);
    }
}
