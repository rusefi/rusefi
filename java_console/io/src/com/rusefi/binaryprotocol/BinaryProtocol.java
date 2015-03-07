package com.rusefi.binaryprotocol;

import etch.util.CircularByteBuffer;
import jssc.SerialPort;
import jssc.SerialPortException;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class BinaryProtocol {
    private final SerialPort serialPort;

    public BinaryProtocol(SerialPort serialPort) {

        this.serialPort = serialPort;
    }

    static void waitForBytes(CircularByteBuffer cbb, int count) throws InterruptedException {
        System.out.println("Waiting for " + count + " byte(s)");
        synchronized (cbb) {
            while (cbb.length() < count)
                cbb.wait();
        }
    }

    public static byte[] makePacket(byte[] command) {
        byte[] packet = new byte[command.length + 6];

        packet[0] = (byte) (command.length / 256);
        packet[1] = (byte) command.length;

        int index = packet.length - 1;
        System.arraycopy(command, 0, packet, 2, command.length);
        int crc = CRC.crc32(command);

        for (int i = 0; i < 4; i++) {
            packet[index--] = (byte) crc;
            crc >>= 8;
        }
        return packet;
    }

    public void sendQueryCommand() throws SerialPortException {
        byte[] command = {'S'};
        sendCrcPacket(command);
    }

    public void sendCrcPacket(byte[] command) throws SerialPortException {
        byte[] packet = makePacket(command);

        serialPort.writeBytes(packet);
    }

    public static int swap16(int x) {
        return (((x & 0xFF) << 8) | ((x) >> 8));
    }

    public static int swap32(int x) {
        return (((x) >> 24) & 0xff) | (((x) << 8) & 0xff0000) | (((x) >> 8) & 0xff00) | (((x) << 24) & 0xff000000);
    }
}
