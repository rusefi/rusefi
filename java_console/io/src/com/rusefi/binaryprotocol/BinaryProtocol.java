package com.rusefi.binaryprotocol;

import com.rusefi.*;
import com.rusefi.config.FieldType;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Pair;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.DataListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.CommunicationLoggingHolder;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.io.serial.SerialIoStream;
import jssc.SerialPort;
import jssc.SerialPortException;

import java.io.EOFException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import static com.rusefi.binaryprotocol.IoHelper.*;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class BinaryProtocol {
    private static final int OUTPUT_CHANNELS_SIZE = 196;

    private static final int BLOCKING_FACTOR = 256;
    private static final byte RESPONSE_OK = 0;
    private static final byte RESPONSE_BURN_OK = 0x04;
    private static final byte RESPONSE_COMMAND_OK = 0x07;
    /**
     * that's hex for "~\n", see BINARY_SWITCH_TAG in firmware source code
     */
    private static final int SWITCH_TO_BINARY_RESPONSE = 0x7e0a;
    /**
     * See SWITCH_TO_BINARY_COMMAND in firmware source code
     */
    private static final String SWITCH_TO_BINARY_COMMAND = "~";
    public static final char COMMAND_OUTPUTS = 'O';
    public static final char COMMAND_HELLO = 'S';
    public static final char COMMAND_PROTOCOL = 'F';
    public static final char COMMAND_CRC_CHECK_COMMAND = 'k';

    private final Logger logger;
    private final IoStream stream;
    private final IncomingDataBuffer incomingData;
    private boolean isBurnPending;

    private final Object ioLock = new Object();
    private final Object imageLock = new Object();
    private ConfigurationImage controller;

    // todo: fix this, this is HORRIBLE!
    @Deprecated
    public static BinaryProtocol instance;
    public boolean isClosed;

    public BinaryProtocol(final Logger logger, IoStream stream) {
        this.logger = logger;
        this.stream = stream;

        instance = this;

        incomingData = new IncomingDataBuffer(logger);
        DataListener streamDataListener = new DataListener() {
            @Override
            public void onDataArrived(byte[] freshData) {
                incomingData.addData(freshData);
            }
        };
        stream.setDataListener(streamDataListener);
    }

    public BinaryProtocol(Logger logger, SerialPort serialPort) {
        this(logger, new SerialIoStream(serialPort, logger));
    }

    private static void sleep() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    public void doSend(final String command, boolean fireEvent) throws InterruptedException {
        FileLog.MAIN.logLine("Sending [" + command + "]");
        if (fireEvent) {
            CommunicationLoggingHolder.communicationLoggingListener.onPortHolderMessage(BinaryProtocol.class, "Sending [" + command + "]");
        }

        Future f = LinkManager.COMMUNICATION_EXECUTOR.submit(new Runnable() {
            @Override
            public void run() {
                sendTextCommand(command);
            }

            @Override
            public String toString() {
                return "Runnable for " + command;
            }
        });

        try {
            f.get(Timeouts.COMMAND_TIMEOUT_SEC, TimeUnit.SECONDS);
        } catch (ExecutionException e) {
            throw new IllegalStateException(e);
        } catch (TimeoutException e) {
            getLogger().error("timeout, giving up: " + e);
            return;
        }
        /**
         * this here to make CommandQueue happy
         */
        MessagesCentral.getInstance().postMessage(PortHolder.class, CommandQueue.CONFIRMATION_PREFIX + command);
    }

    /**
     * this method would switch controller to binary protocol and read configuration snapshot from controller
     *
     * @return true if everything fine
     */
    public boolean connectAndReadConfiguration(DataListener listener) {
        switchToBinaryProtocol();
        readImage(TsPageSize.IMAGE_SIZE);
        if (isClosed)
            return false;

        startTextPullThread(listener);
        return true;
    }

    private void startTextPullThread(final DataListener listener) {
        if (!LinkManager.COMMUNICATION_QUEUE.isEmpty()) {
            System.out.println("Current queue: " + LinkManager.COMMUNICATION_QUEUE.size());
        }
        Runnable textPull = new Runnable() {
            @Override
            public void run() {
                while (!isClosed) {
//                    FileLog.rlog("queue: " + LinkManager.COMMUNICATION_QUEUE.toString());
                    if (LinkManager.COMMUNICATION_QUEUE.isEmpty()) {
                        LinkManager.COMMUNICATION_EXECUTOR.submit(new Runnable() {
                            @Override
                            public void run() {
                                requestOutputChannels();
                                String text = requestPendingMessages();
                                if (text != null)
                                    listener.onDataArrived((text + "\r\n").getBytes());
                            }
                        });
                    }
                    sleep();
                }
                FileLog.MAIN.logLine("Stopping text pull");
            }
        };
        Thread tr = new Thread(textPull);
        tr.setName("text pull");
        tr.start();
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
            try {
                dropPending();

                stream.write((SWITCH_TO_BINARY_COMMAND + "\n").getBytes());
                synchronized (ioLock) {
                    boolean isTimeout = incomingData.waitForBytes(2, start, "switch to binary");
                    if (isTimeout) {
                        close();
                        System.out.println("Timeout waiting for switch response");
                        return;
                    }
                    int response = incomingData.getShort();
                    if (response != swap16(SWITCH_TO_BINARY_RESPONSE)) {
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

    private void dropPending() throws IOException {
        synchronized (ioLock) {
            if (isClosed)
                return;
            incomingData.dropPending();
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

    private byte[] receivePacket(String msg, boolean allowLongResponse) throws InterruptedException, EOFException {
        long start = System.currentTimeMillis();
        synchronized (ioLock) {
            boolean isTimeout = incomingData.waitForBytes(2, start, msg + " header");
            if (isTimeout)
                return null;

            int packetSize = swap16(incomingData.getShort());
            logger.trace("Got packet size " + packetSize);
            if (packetSize < 0)
                return null;
            if (!allowLongResponse && packetSize > BLOCKING_FACTOR + 10)
                return null;

            isTimeout = incomingData.waitForBytes(packetSize + 4, start, msg + " body");
            if (isTimeout)
                return null;

            byte[] packet = new byte[packetSize];
                incomingData.getData(packet);
            int packetCrc = swap32(incomingData.getInt());
            int actualCrc = crc32(packet);

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
        logger.info("Reading from controller...");

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

            byte[] response = executeCommand(packet, "load image", false);

            if (!checkResponseCode(response, RESPONSE_OK) || response.length != requestSize + 1) {
                logger.error("readImage: Something is wrong, retrying...");
                continue;
            }

            System.arraycopy(response, 1, image.getContent(), offset, requestSize);

            offset += requestSize;
        }
        logger.info("Got configuration from controller.");
        setController(image);
    }

    /**
     * Blocking sending binary packet and waiting for a response
     *
     * @return null in case of IO issues
     */
    private byte[] executeCommand(byte[] packet, String msg, boolean allowLongResponse) {
        if (isClosed)
            return null;
        try {
            dropPending();

            sendCrcPacket(packet);
            return receivePacket(msg, allowLongResponse);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        } catch (IOException e) {
            logger.error(msg + ": executeCommand failed: " + e);
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
            byte[] response = executeCommand(packet, "writeImage", false);
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
            byte[] response = executeCommand(new byte[]{'B'}, "burn", false);
            if (!checkResponseCode(response, RESPONSE_BURN_OK) || response.length != 1) {
                continue;
            }
            break;
        }
        logger.info("DONE");
        isBurnPending = false;
    }

    public void setController(ConfigurationImage controller) {
        synchronized (imageLock) {
            this.controller = controller.clone();
        }
    }

    public ConfigurationImage getController() {
        synchronized (imageLock) {
            if (controller == null)
                return null;
            return controller.clone();
        }
    }

    private void sendCrcPacket(byte[] command) throws IOException {
        sendCrcPacket(command, logger, stream);
    }

    public static void sendCrcPacket(byte[] command, Logger logger, IoStream stream) throws IOException {
        byte[] packet = IoHelper.makeCrc32Packet(command);
        logger.info("Sending " + Arrays.toString(packet));
        stream.write(packet);
    }

    /**
     * This method blocks until a confirmation is received
     *
     * @return true in case of timeout, false if got proper confirmation
     */
    private boolean sendTextCommand(String text) {
        byte[] asBytes = text.getBytes();
        byte[] command = new byte[asBytes.length + 1];
        command[0] = 'E';
        System.arraycopy(asBytes, 0, command, 1, asBytes.length);

        long start = System.currentTimeMillis();
        while (!isClosed && (System.currentTimeMillis() - start < Timeouts.BINARY_IO_TIMEOUT)) {
            byte[] response = executeCommand(command, "execute", false);
            if (!checkResponseCode(response, RESPONSE_COMMAND_OK) || response.length != 1) {
                continue;
            }
            return false;
        }
        return true;
    }

    public String requestPendingMessages() {
        if (isClosed)
            return null;
        try {
            byte[] response = executeCommand(new byte[]{'G'}, "text", true);
            if (response != null && response.length == 1)
                Thread.sleep(100);
            //        System.out.println(result);
            return new String(response, 1, response.length - 1);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    public void requestOutputChannels() {
        if (isClosed)
            return;
//        try {
        byte[] response = executeCommand(new byte[]{COMMAND_OUTPUTS}, "output channels", false);
        if (response == null || response.length != (OUTPUT_CHANNELS_SIZE + 1) || response[0] != RESPONSE_OK)
            return;

        for (Sensor sensor : Sensor.values()) {
            if (sensor.getType() == FieldType.FLOAT) {

                ByteBuffer bb = ByteBuffer.wrap(response, 1 + sensor.getOffset(), 4);
                bb.order(ByteOrder.LITTLE_ENDIAN);

                double value = bb.getFloat();

                SensorCentral.getInstance().setValue(value, sensor);
            }
        }

//        } catch (InterruptedException e) {
//            throw new IllegalStateException(e);
//        }
    }
}
