package com.rusefi.binaryprotocol.test;

import com.rusefi.CompatibleFunction;
import com.rusefi.Listener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.IOException;
import java.net.Socket;

import static com.rusefi.config.generated.Fields.TS_PROTOCOL;
import static com.rusefi.io.tcp.BinaryProtocolServer.TS_OK;

public class TcpServerSandbox {
    public static void main(String[] args) throws IOException {
        BinaryProtocolServer.tcpServerSocket(29001,
                "server",
                new CompatibleFunction<Socket, Runnable>() {
                    @Override
                    public Runnable apply(Socket socket) {
                        System.out.println("apply");

                        return new Runnable() {
                            @Override
                            public void run() {
                                System.out.println("Run server socket: " + socket);

                                try {
                                    IoStream stream = new TcpIoStream("gauge", socket);
                                    IncomingDataBuffer in = stream.getDataBuffer();

                                    while (!socket.isClosed()) {
                                        Integer length = BinaryProtocolServer.getPendingPacketLengthOrHandleProtocolCommand(socket, new BinaryProtocolServer.Context(), in);
                                        if (length == null)
                                            continue;
                                        byte[] payload = BinaryProtocolServer.getPacketContent(in, length);

                                        byte command = payload[0];

                                        if (command == Fields.TS_HELLO_COMMAND) {
                                            new HelloCommand(Fields.TS_SIGNATURE).handle(stream);
                                        } else if (command == Fields.TS_GET_PROTOCOL_VERSION_COMMAND_F) {
                                            stream.sendPacket((TS_OK + TS_PROTOCOL).getBytes());
                                        } else
                                            throw new UnsupportedOperationException("Unsupported command " + command);
                                    }

                                } catch (Exception e) {
                                    System.out.println(e);
                                }
                            }
                        };
                    }
                },
                new Listener() {
                    @Override
                    public void onResult(Object parameter) {
                        System.out.println("onResult");

                    }
                });
    }
}
