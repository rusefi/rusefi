package com.rusefi.binaryprotocol.test;

import com.opensr5.ini.IniFileModeSingleton;

/**
 * Fully self-contained fake TCP-IP 'ECU' side of TS protocol
 * does not have checkCrc32 command implementation so you have to remove it from .ini if you want to connect to this ECU
 *
 * @see TcpClientSandbox
 * @see BinaryProtocolServerSandbox what's the difference?
 */
public class TcpServerSandbox {
    private final static byte[] TOTALLY_EMPTY_CONFIGURATION = new byte[IniFileModeSingleton.getInstance().getMetaInfo().getPageSize(0)];
/*
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


                        EcuState ecuState = new EcuState();

                        try {
                            IoStream stream = new TcpIoStream(TcpServerSandbox.class.getSimpleName(), socket);
                            IncomingDataBuffer in = stream.getDataBuffer();

                            while (!socket.isClosed()) {
                                handleCommand(ecuState, stream, in, socket);

                                ecuState.onCommand();

                            }

                        } catch (Exception e) {
                            System.out.println("huh exception: " + e);
                            e.printStackTrace();
                        }
                    }
                };
            }
        };
        BinaryProtocolServer.tcpServerSocket(29001, "server", socketRunnableFactory,
                serverSocketCreationCallback, StatusConsumer.ANONYMOUS);
    }

    static class EcuState {
        private final byte[] outputs = new byte[VariableRegistryValues.TS_TOTAL_OUTPUT_SIZE];

        final long startUpTime = System.currentTimeMillis();

        public void onCommand() {
            int seconds = (int) ((System.currentTimeMillis() - startUpTime) / 1000);

            TsOutputs.SECONDS.setValueU32(outputs, seconds);

        }
    }


    private static void handleCommand(EcuState ecuState, IoStream stream, IncomingDataBuffer in, Socket socket) throws IOException {
        Integer length = BinaryProtocolServer.getPendingPacketLengthOrHandleProtocolCommand(socket, new BinaryProtocolServer.Context(), in);
        if (length == null)
            return;
        byte[] payload = BinaryProtocolServer.getPacketContent(in, length);

        byte command = payload[0];

        if (command == Integration.TS_HELLO_COMMAND) {
            new HelloCommand(VariableRegistryValues.TS_SIGNATURE).handle(stream);
        } else if (command == Integration.TS_GET_PROTOCOL_VERSION_COMMAND_F) {
            stream.sendPacket((TS_OK + TS_PROTOCOL).getBytes());
        } else if (command == Integration.TS_CRC_CHECK_COMMAND) {
            stream.sendPacket(BinaryProtocolServer.createCrcResponse(TOTALLY_EMPTY_CONFIGURATION));
        } else if (command == Integration.TS_SET_LOGGER_SWITCH) {
            if (payload[1] == Integration.TS_COMPOSITE_READ) {
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                baos.write(TS_OK.charAt(0));
                LittleEndianOutputStream dout = new LittleEndianOutputStream(baos);
                int count = 256;
                dout.writeShort(count * 5);

                for (int i = 0; i < count; i++) {
                    baos.write(i);
                    baos.write(0);
                    baos.write(0);
                    baos.write(0);
                    baos.write(100);
                }
                stream.sendPacket(baos.toByteArray());
            } else {
                stream.sendPacket(TS_OK.getBytes());
            }
        } else if (command == Integration.TS_OUTPUT_COMMAND) {
            byte[] response = getOutputCommandResponse(payload, ecuState.outputs);
            stream.sendPacket(response);
//        } else if (command == Integration.TS_GET_SCATTERED_GET_COMMAND) {
////            System.out.println("Cool TS_GET_SCATTERED_GET_COMMAND");
//            int startOffset = HIGHSPEEDOFFSETS.getOffset();
//            int totalResponseSize = 0;
//            for (int i = 0; i < HIGH_SPEED_COUNT; i++) {
//                int higherByte = getByte(startOffset + 1);
//                int type = higherByte >> 5;
//                int size = getSize(type);
//
//                totalResponseSize += size;
//
//                int twoBytes = getByte(startOffset) + (higherByte & 0x1F) * 256;
////                System.out.println("TS_GET_SCATTERED_GET_COMMAND index=" + i + " type=" + type + " offset=" + twoBytes);
//                startOffset += 2;
//            }
//
//            byte[] response = new byte[1 + totalResponseSize];
//            response[0] = (byte) TS_OK.charAt(0);
//            stream.sendPacket(response);
//
        } else if (command == Integration.TS_CHUNK_WRITE_COMMAND) {
            ByteRange byteRange = ByteRange.valueOf(payload);
            System.out.println("TS_CHUNK_WRITE_COMMAND " + byteRange + " payload " + payload.length);
            System.arraycopy(payload, WriteChunkCommand.SCR_POS_WITH, TOTALLY_EMPTY_CONFIGURATION, byteRange.getOffset(), byteRange.getCount());
            stream.sendPacket(TS_OK.getBytes());
        } else if (command == Integration.TS_BURN_COMMAND) {
            stream.sendPacket(new byte[]{Integration.TS_RESPONSE_BURN_OK});
        } else if (command == Integration.TS_READ_COMMAND) {
            ByteRange byteRange = ByteRange.valueOf(payload);
            int count = byteRange.getCount();
            // always all zero response
            byte[] response = new byte[1 + count];
            response[0] = (byte) TS_OK.charAt(0);
            stream.sendPacket(response);
        } else if (command == Integration.TS_GET_FIRMWARE_VERSION) {
            stream.sendPacket((TS_OK + "rusEFI proxy").getBytes());
        } else
            throw new UnsupportedOperationException("Unsupported command " + BinaryProtocol.findCommand(command));
    }

    private static int getSize(int type) {
        if (type == 0)
            return 0;
        return 1 << (type - 1);
    }

    private static int getByte(int startOffset) {
        return TOTALLY_EMPTY_CONFIGURATION[startOffset] & 0xFF;
    }
 */
}
