package com.rusefi.io.tcp;

import com.opensr5.Logger;
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
    public static void createProxy(Logger logger, IoStream targetEcuSocket, int serverProxyPort) {
        Function<Socket, Runnable> clientSocketRunnableFactory = new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket clientSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        try {
                            TcpIoStream clientStream = new TcpIoStream("[[proxy]] ", logger, clientSocket);
                            runProxy(targetEcuSocket, clientStream);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                };
            }
        };
        BinaryProtocolServer.tcpServerSocket(serverProxyPort, "proxy", clientSocketRunnableFactory, Logger.CONSOLE, null);
    }

    public static void runProxy(IoStream targetEcu, IoStream clientStream) throws IOException {
        /*
         * Each client socket is running on it's own thread
         */

        while (true) {
            byte firstByte = clientStream.getDataBuffer().readByte();
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
