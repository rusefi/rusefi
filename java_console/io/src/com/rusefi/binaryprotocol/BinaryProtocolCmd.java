package com.rusefi.binaryprotocol;

import com.rusefi.io.DataListener;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.io.serial.SerialPortReader;
import etch.util.CircularByteBuffer;
import jssc.SerialPort;
import jssc.SerialPortException;

import java.io.EOFException;
import java.util.Arrays;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class BinaryProtocolCmd {
    private static final int BUFFER_SIZE = 10000;
    private static SerialPort serialPort;

    public static void main(String[] args) throws SerialPortException, InterruptedException, EOFException {
        if (args.length != 1) {
            System.out.println("Exactly one parameter expected");
            return;
        }

        String port = args[0];


        serialPort = new SerialPort(port);
        boolean opened = serialPort.openPort();
        if (!opened) {
            System.out.println("failed to open " + port);
        }
        BinaryProtocol bp = new BinaryProtocol(serialPort);


        final CircularByteBuffer cbb = new CircularByteBuffer(BUFFER_SIZE);
        DataListener listener = new DataListener() {
            @Override
            public void onDataArrived(byte[] freshData) {
                System.out.println(freshData.length + " byte(s) arrived");
                synchronized (cbb) {
                    if (cbb.size() - cbb.length() < freshData.length) {
                        System.out.println("buffer overflow not expected");
                        cbb.clear();
                    }
                    cbb.put(freshData);
                    cbb.notifyAll();
                }
            }
        };
        serialPort.addEventListener(new SerialPortReader(serialPort, listener));
        PortHolder.setupPort(serialPort, 38400);
        System.out.println("Looks good");
        bp.sendQueryCommand();


        synchronized (cbb) {
            BinaryProtocol.waitForBytes(cbb, 2);

            int packetSize = BinaryProtocol.swap16(cbb.getShort());
            System.out.println("Got packet size " + packetSize);
            BinaryProtocol.waitForBytes(cbb, packetSize + 4);
            byte[] packet = new byte[packetSize];
            int packetCrc;
            synchronized (cbb) {
                cbb.get(packet);
                packetCrc = BinaryProtocol.swap32(cbb.getInt());
            }
            int actualCrc = CRC.crc32(packet);

            boolean isCrcOk = actualCrc == packetCrc;
            if(!isCrcOk) {
                System.out.println(String.format("%x", actualCrc) + " vs " + String.format("%x", packetCrc));
            }

            System.out.println("packet " + Arrays.toString(packet) + ": crc ok=" + isCrcOk);
        }


    }

}
