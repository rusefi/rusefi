package com.rusefi.binaryprotocol.test;

import com.rusefi.CompatibleFunction;
import com.rusefi.Listener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.ui.StatusConsumer;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;

import static com.rusefi.binaryprotocol.IoHelper.swap16;
import static com.rusefi.config.generated.Fields.TS_PROTOCOL;
import static com.rusefi.io.tcp.BinaryProtocolServer.TS_OK;
import static com.rusefi.io.tcp.BinaryProtocolServer.getOutputCommandResponse;

/**
 * Fully self-contained fake TCP-IP 'ECU' side of TS protocol
 * does not have checkCrc32 command implementation so you have to remove it from .ini if you want to connect to this ECU
 *
 * @see TcpClientSandbox
 */
public class TcpServerSandbox {
    private final static byte [] TOTALLY_EMPTY_CONFIGURATION = new byte[Fields.TOTAL_CONFIG_SIZE];
    private final static byte [] TOTALLY_EMPTY_OUTPUTS = new byte[Fields.TS_TOTAL_OUTPUT_SIZE];

    public static void main(String[] args) throws IOException {
        Listener serverSocketCreationCallback = parameter -> System.out.println("serverSocketCreationCallback");
        CompatibleFunction<Socket, Runnable> socketRunnableFactory = new CompatibleFunction<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket socket) {
                System.out.println("New connection!");

                return new Runnable() {
                    @Override
                    public void run() {
                        System.out.println("Run server socket: " + socket);

                        try {
                            IoStream stream = new TcpIoStream(TcpServerSandbox.class.getSimpleName(), socket);
                            IncomingDataBuffer in = stream.getDataBuffer();

                            while (!socket.isClosed()) {
                                handleCommand(stream, in, socket);

                            }

                        } catch (Exception e) {
                            System.out.println(e);
                        }
                    }
                };
            }
        };
        BinaryProtocolServer.tcpServerSocket(29001, "server", socketRunnableFactory,
                serverSocketCreationCallback, StatusConsumer.ANONYMOUS);
    }

    private static void handleCommand(IoStream stream, IncomingDataBuffer in, Socket socket) throws IOException {
        Integer length = BinaryProtocolServer.getPendingPacketLengthOrHandleProtocolCommand(socket, new BinaryProtocolServer.Context(), in);
        if (length == null)
            return;
        byte[] payload = BinaryProtocolServer.getPacketContent(in, length);

        byte command = payload[0];

        if (command == Fields.TS_HELLO_COMMAND) {
            new HelloCommand(Fields.TS_SIGNATURE).handle(stream);
        } else if (command == Fields.TS_GET_PROTOCOL_VERSION_COMMAND_F) {
            stream.sendPacket((TS_OK + TS_PROTOCOL).getBytes());
        } else if (command == Fields.TS_PAGE_COMMAND) {
            stream.sendPacket(TS_OK.getBytes());
        } else if (command == Fields.TS_CRC_CHECK_COMMAND) {
            stream.sendPacket(BinaryProtocolServer.createCrcResponse(TOTALLY_EMPTY_CONFIGURATION));
        } else if (command == Fields.TS_OUTPUT_COMMAND) {
            byte[] response = getOutputCommandResponse(payload, TOTALLY_EMPTY_OUTPUTS);
            stream.sendPacket(response);
        } else if (command == Fields.TS_READ_COMMAND) {
            DataInputStream dis = new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
            int offset = swap16(dis.readShort());
            int count = swap16(dis.readShort());
            // always all zero response
            byte[] response = new byte[1 + count];
            response[0] = (byte) TS_OK.charAt(0);
            stream.sendPacket(response);
        } else if (command == Fields.TS_GET_FIRMWARE_VERSION) {
            stream.sendPacket((TS_OK + "rusEFI proxy").getBytes());
        } else
            throw new UnsupportedOperationException("Unsupported command " + BinaryProtocol.findCommand(command));
    }
}
