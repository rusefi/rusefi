package com.rusefi.io.tcp;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.function.Function;

import static com.rusefi.binaryprotocol.BinaryProtocolCommands.COMMAND_PROTOCOL;

public class BinaryProtocolProxy {
    public static void createProxy(IoStream targetEcuSocket, int serverProxyPort) {
        Function<Socket, Runnable> clientSocketRunnableFactory = new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket clientSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        runProxy(targetEcuSocket, clientSocket);
                    }
                };
            }
        };
        BinaryProtocolServer.tcpServerSocket(serverProxyPort, "proxy", clientSocketRunnableFactory, Logger.CONSOLE, null);
    }

    private static void runProxy(IoStream targetEcu, Socket clientSocket) {
        /*
         * Each client socket is running on it's own thread
         */
        try {
            DataInputStream clientInputStream = new DataInputStream(clientSocket.getInputStream());
            DataOutputStream clientOutputStream = new DataOutputStream(clientSocket.getOutputStream());

            while (true) {
                byte firstByte = clientInputStream.readByte();
                if (firstByte == COMMAND_PROTOCOL) {
                    BinaryProtocolServer.handleProtocolCommand(clientSocket);
                    continue;
                }
                proxyClientRequestToController(clientInputStream, firstByte, targetEcu);

                proxyControllerResponseToClient(targetEcu, clientOutputStream);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void proxyControllerResponseToClient(IoStream targetInputStream, DataOutputStream clientOutputStream) throws IOException {
        short length = targetInputStream.readShort();
        BinaryProtocolServer.Packet packet = BinaryProtocolServer.readPromisedBytes(targetInputStream.getDataBuffer(), length);

        System.out.println("Relaying controller response length=" + length);
        clientOutputStream.writeShort(length);
        clientOutputStream.write(packet.getPacket());
        clientOutputStream.writeInt(packet.getCrc());
        clientOutputStream.flush();
    }

    private static void proxyClientRequestToController(DataInputStream in, byte firstByte, IoStream targetOutputStream) throws IOException {
        byte secondByte = in.readByte();
        int length = firstByte * 256 + secondByte;

        BinaryProtocolServer.Packet packet = BinaryProtocolServer.readPromisedBytes(in, length);

        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(packet.getPacket()));
        byte command = (byte) dis.read();

        System.out.println("Relaying client command" + (char) command + "/" + command + " length=" + length);
        // sending proxies packet to controller
        targetOutputStream.write(firstByte);
        targetOutputStream.write(secondByte);
        targetOutputStream.write(packet.getPacket());
        targetOutputStream.writeInt(packet.getCrc());
    }
}
