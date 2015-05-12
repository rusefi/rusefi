package com.rusefi.binaryprotocol;

import com.rusefi.*;
import com.rusefi.core.Pair;
import com.rusefi.io.DataListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.serial.SerialIoStream;
import etch.util.CircularByteBuffer;
import jssc.SerialPort;
import jssc.SerialPortException;

import java.io.EOFException;
import java.io.IOException;
import java.util.Arrays;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class BinaryProtocol {
    private static final int BLOCKING_FACTOR = 256;
    private static final byte RESPONSE_OK = 0;
    private static final byte RESPONSE_BURN_OK = 0x04;
    private static final byte RESPONSE_COMMAND_OK = 0x07;
    private static final int SWITCH_TO_BINARY_RESPONSE = 0xA7E;

    private final Logger logger;
    private final IoStream stream;
    private static final int BUFFER_SIZE = 10000;
    private final CircularByteBuffer cbb;
    private boolean isBurnPending;

    private final Object lock = new Object();
    private ConfigurationImage controller;

    // todo: fix this, this is HORRIBLE!
    @Deprecated
    public static BinaryProtocol instance;
    public boolean isClosed;

    public BinaryProtocol(final Logger logger, SerialIoStream stream) {
        this.logger = logger;
        this.stream = stream;

        instance = this;

        cbb = new CircularByteBuffer(BUFFER_SIZE);
        DataListener listener = new DataListener() {
            @Override
            public void onDataArrived(byte[] freshData) {
                logger.trace(freshData.length + " byte(s) arrived");
                synchronized (cbb) {
                    if (cbb.size() - cbb.length() < freshData.length) {
                        logger.error("buffer overflow not expected");
                        cbb.clear();
                    }
                    cbb.put(freshData);
                    cbb.notifyAll();
                }
            }
        };
        this.stream.addEventListener(listener);
    }

    public BinaryProtocol(Logger logger, SerialPort serialPort) {
        this(logger, new SerialIoStream(serialPort, logger));
    }

    public Logger getLogger() {
        return logger;
    }

    public void switchToBinaryProtocol() {
        // we do not have reliable implementation yet :(
        for (int i = 0; i < 15; i++)
            doSwitchToBinary();
    }

    private void doSwitchToBinary() {
        long start = System.currentTimeMillis();

        while (true) {
            dropPending();

            try {
                stream.write("~\n".getBytes());
                synchronized (cbb) {
                    boolean isTimeout = waitForBytes(2, start, "switch to binary");
                    if (isTimeout) {
                        close();
                        System.out.println("Timeout waiting for switch response");
                        return;
                    }
                    int response = cbb.getShort();
                    if (response != SWITCH_TO_BINARY_RESPONSE) {
                        logger.error(String.format("Unexpected response [%x], re-trying", response));
                        continue;
                    }
                    logger.info("Switched to binary protocol");
                }
            } catch (IOException e) {
                close();
                FileLog.MAIN.logLine("exception: " + e);
                return;
            } catch (InterruptedException e) {
                throw new IllegalStateException(e);
            }
            break;
        }
    }

    private void dropPending() {
        synchronized (cbb) {
            if (isClosed)
                return;
            int pending = cbb.length();
            if (pending > 0) {
                logger.error("Unexpected pending data: " + pending + " byte(s)");
                cbb.get(new byte[pending]);
            }
            stream.purge();
        }
    }

    public void burnChanges(ConfigurationImage newVersion, Logger logger) throws InterruptedException, EOFException, SerialPortException {
        ConfigurationImage current = getController();
        // let's have our own copy which no one would be able to change
        newVersion = newVersion.clone();
        int offset = 0;
        while (offset < current.getSize()) {
            Pair<Integer, Integer> range = ConfigurationImageDiff.findDifferences(current, newVersion, offset);
            if (range == null)
                break;
            int size = range.second - range.first;
            logger.info("Need to patch: " + range + ", size=" + size);
            byte[] oldBytes = current.getRange(range.first, size);
            logger.info("old " + Arrays.toString(oldBytes));

            byte[] newBytes = newVersion.getRange(range.first, size);
            logger.info("new " + Arrays.toString(newBytes));

            writeData(newVersion.getContent(), range.first, size, logger);

            offset = range.second;
        }
        burn(logger);
        setController(newVersion);
    }

    /**
     * this method adds two bytes for packet size before and four bytes for CRC after
     */
    public static byte[] makePacket(byte[] command) {
        byte[] packet = new byte[command.length + 6];

        packet[0] = (byte) (command.length / 256);
        packet[1] = (byte) command.length;

        System.arraycopy(command, 0, packet, 2, command.length);
        int crc = CRC.crc32(command);

        putInt(packet, packet.length - 4, crc);
        return packet;
    }

    public static int swap16(int x) {
        return (((x & 0xFF) << 8) | ((x) >> 8));
    }

    public static int swap32(int x) {
        return (((x) >> 24) & 0xff) | (((x) << 8) & 0xff0000) | (((x) >> 8) & 0xff00) | (((x) << 24) & 0xff000000);
    }

    private byte[] receivePacket(String msg, boolean allowLongResponse) throws InterruptedException, EOFException {
        long start = System.currentTimeMillis();
        synchronized (cbb) {
            boolean isTimeout = waitForBytes(2, start, msg + " header");
            if (isTimeout)
                return null;

            int packetSize = BinaryProtocol.swap16(cbb.getShort());
            logger.trace("Got packet size " + packetSize);
            if (packetSize < 0)
                return null;
            if (!allowLongResponse && packetSize > BLOCKING_FACTOR + 10)
                return null;

            isTimeout = waitForBytes(packetSize + 4, start, msg + " body");
            if (isTimeout)
                return null;

            byte[] packet = new byte[packetSize];
            int packetCrc;
            synchronized (cbb) {
                cbb.get(packet);
                packetCrc = BinaryProtocol.swap32(cbb.getInt());
            }
            int actualCrc = CRC.crc32(packet);

            boolean isCrcOk = actualCrc == packetCrc;
            if (!isCrcOk) {
                logger.trace(String.format("%x", actualCrc) + " vs " + String.format("%x", packetCrc));
                return null;
            }
            logger.trace("packet " + Arrays.toString(packet) + ": crc OK");

            return packet;
        }
    }

    public void readImage(int size) {
        ConfigurationImage image = new ConfigurationImage(size);

        int offset = 0;

        long start = System.currentTimeMillis();

        while (offset < image.getSize() && (System.currentTimeMillis() - start < Timeouts.READ_IMAGE_TIMEOUT)) {
            if (isClosed)
                return;

            int remainingSize = image.getSize() - offset;
            int requestSize = Math.min(remainingSize, BLOCKING_FACTOR);

            byte packet[] = new byte[7];
            packet[0] = 'R';
            putShort(packet, 1, 0); // page
            putShort(packet, 3, swap16(offset));
            putShort(packet, 5, swap16(requestSize));

            byte[] response = exchange(packet, "load image", false);

            if (!checkResponseCode(response, RESPONSE_OK) || response.length != requestSize + 1) {
                logger.error("readImage: Something is wrong, retrying...");
                continue;
            }


            System.arraycopy(response, 1, image.getContent(), offset, requestSize);

            offset += requestSize;
        }
        logger.info("Got image!");
        setController(image);
    }

    /**
     * Blocking sending binary packet and waiting for a response
     *
     * @return null in case of IO issues
     */
    public byte[] exchange(byte[] packet, String msg, boolean allowLongResponse) {
        if (isClosed)
            return null;
        dropPending();
        try {
            sendCrcPacket(packet);
            return receivePacket(msg, allowLongResponse);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        } catch (IOException e) {
            logger.error(msg + ": exchange failed: " + e);
            close();
            return null;
        }
    }

    public void close() {
        if (isClosed)
            return;
        isClosed = true;
        stream.close();
    }

    public void writeData(byte[] content, Integer offset, int size, Logger logger) throws SerialPortException, EOFException, InterruptedException {
        if (size > BLOCKING_FACTOR) {
            writeData(content, offset, BLOCKING_FACTOR, logger);
            writeData(content, offset + BLOCKING_FACTOR, size - BLOCKING_FACTOR, logger);
            return;
        }

        isBurnPending = true;

        byte packet[] = new byte[7 + size];
        packet[0] = 'C';
        putShort(packet, 1, 0); // page
        putShort(packet, 3, swap16(offset));
        putShort(packet, 5, swap16(size));

        System.arraycopy(content, offset, packet, 7, size);

        long start = System.currentTimeMillis();
        while (!isClosed && (System.currentTimeMillis() - start < Timeouts.BINARY_IO_TIMEOUT)) {
            byte[] response = exchange(packet, "writeImage", false);
            if (!checkResponseCode(response, RESPONSE_OK) || response.length != 1) {
                logger.error("writeData: Something is wrong, retrying...");
                continue;
            }
            break;
        }
    }

    private void burn(Logger logger) throws InterruptedException, EOFException, SerialPortException {
        if (!isBurnPending)
            return;
        logger.info("Need to burn");

        while (true) {
            if (isClosed)
                return;
            byte[] response = exchange(new byte[]{'B'}, "burn", false);
            if (!checkResponseCode(response, RESPONSE_BURN_OK) || response.length != 1) {
                continue;
            }
            break;
        }
        logger.info("DONE");
        isBurnPending = false;
    }

    public void setController(ConfigurationImage controller) {
        synchronized (lock) {
            this.controller = controller.clone();
        }
    }

    public ConfigurationImage getController() {
        synchronized (lock) {
            if (controller == null)
                return null;
            return controller.clone();
        }
    }

    /**
     * @return true in case of timeout, false if everything is fine
     */
    private boolean waitForBytes(int count, long start, String msg) throws InterruptedException {
        logger.info("Waiting for " + count + " byte(s): " + msg);
        synchronized (cbb) {
            while (cbb.length() < count) {
                int timeout = (int) (start + Timeouts.BINARY_IO_TIMEOUT - System.currentTimeMillis());
                if (timeout <= 0) {
                    return true; // timeout. Sad face.
                }
                cbb.wait(timeout);
            }
        }
        return false; // looks good!
    }

    private boolean checkResponseCode(byte[] response, byte code) {
        return response != null && response.length > 0 && response[0] == code;
    }

    private static void putInt(byte[] packet, int offset, int value) {
        int index = offset + 3;
        for (int i = 0; i < 4; i++) {
            packet[index--] = (byte) value;
            value >>= 8;
        }
    }

    private static void putShort(byte[] packet, int offset, int value) {
        int index = offset + 1;
        for (int i = 0; i < 2; i++) {
            packet[index--] = (byte) value;
            value >>= 8;
        }
    }

    private void sendCrcPacket(byte[] command) throws IOException {
        byte[] packet = makePacket(command);
        logger.info("Sending " + Arrays.toString(packet));
        stream.write(packet);
    }

    /**
     * This method blocks until a confirmation is received
     *
     * @return true in case of timeout, false if got proper confirmation
     */
    public boolean sendTextCommand(String text) {
        byte[] asBytes = text.getBytes();
        byte[] command = new byte[asBytes.length + 1];
        command[0] = 'E';
        System.arraycopy(asBytes, 0, command, 1, asBytes.length);

        long start = System.currentTimeMillis();
        while (!isClosed && (System.currentTimeMillis() - start < Timeouts.BINARY_IO_TIMEOUT)) {
            byte[] response = exchange(command, "execute", false);
            if (!checkResponseCode(response, RESPONSE_COMMAND_OK) || response.length != 1) {
                continue;
            }
            return false;
        }
        return true;
    }

    public String requestText() {
        if (isClosed)
            return null;
        try {
            byte[] response = exchange(new byte[]{'G'}, "text", true);
            if (response != null && response.length == 1)
                Thread.sleep(100);
            //        System.out.println(result);
            return new String(response, 1, response.length - 1);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }
}
