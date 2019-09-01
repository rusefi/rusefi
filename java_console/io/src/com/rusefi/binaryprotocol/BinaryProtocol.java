package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.rusefi.ConfigurationImageDiff;
import com.rusefi.FileLog;
import com.rusefi.Timeouts;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Pair;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.*;
import com.rusefi.ui.livedocs.LiveDocsRegistry;
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
 * @see BinaryProtocolHolder
 */
public class BinaryProtocol implements BinaryProtocolCommands {

    private static final String USE_PLAIN_PROTOCOL_PROPERTY = "protocol.plain";
    private static final int TEXT_PULL_PERIOD = 100;
    /**
     * This properly allows to switch to non-CRC32 mode
     * todo: finish this feature, assuming we even need it.
     */
    public static boolean PLAIN_PROTOCOL = Boolean.getBoolean(USE_PLAIN_PROTOCOL_PROPERTY);
    static {
        FileLog.MAIN.logLine(USE_PLAIN_PROTOCOL_PROPERTY + ": " + PLAIN_PROTOCOL);
    }

    private final Logger logger;
    private final IoStream stream;
    private final IncomingDataBuffer incomingData;
    private boolean isBurnPending;

    // todo: this ioLock needs better documentation!
    private final Object ioLock = new Object();
    private final Object imageLock = new Object();
    private ConfigurationImage controller;

    public boolean isClosed;
    /**
     * Snapshot of current gauges status
     * @see BinaryProtocolCommands#COMMAND_OUTPUTS
     */
    public byte[] currentOutputs;

    protected BinaryProtocol(final Logger logger, IoStream stream) {
        this.logger = logger;
        this.stream = stream;

        incomingData = new IncomingDataBuffer(logger);
        stream.setInputListener(incomingData::addData);
    }

    private static void sleep(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    public void doSend(final String command, boolean fireEvent) throws InterruptedException {
        FileLog.MAIN.logLine("Sending [" + command + "]");
        if (fireEvent && LinkManager.LOG_LEVEL.isDebugEnabled()) {
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
            getLogger().error("timeout sending [" + command + "] giving up: " + e);
            return;
        }
        /**
         * this here to make CommandQueue happy
         */
        CommandQueue.getInstance().handleConfirmationMessage(CommandQueue.CONFIRMATION_PREFIX + command);
    }

    /**
     * this method would switch controller to binary protocol and read configuration snapshot from controller
     *
     * @return true if everything fine
     */
    public boolean connectAndReadConfiguration(DataListener listener) {
//        switchToBinaryProtocol();
        readImage(Fields.TOTAL_CONFIG_SIZE);
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
                                if (requestOutputChannels())
                                	ConnectionWatchdog.onDataArrived();
                                String text = requestPendingMessages();
                                if (text != null)
                                    listener.onDataArrived((text + "\r\n").getBytes());
                                LiveDocsRegistry.INSTANCE.refresh(BinaryProtocol.this);
                            }
                        });
                    }
                    sleep(TEXT_PULL_PERIOD);
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

    private void dropPending() {
        synchronized (ioLock) {
            if (isClosed)
                return;
            incomingData.dropPending();
            stream.purge();
        }
    }

    public void uploadChanges(ConfigurationImage newVersion, Logger logger) throws InterruptedException, EOFException, SerialPortException {
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
            return incomingData.getPacket(logger, msg, allowLongResponse, start);
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
            packet[0] = COMMAND_READ;
            putShort(packet, 1, 0); // page
            putShort(packet, 3, swap16(offset));
            putShort(packet, 5, swap16(requestSize));

            byte[] response = executeCommand(packet, "load image offset=" + offset, false);

            if (!checkResponseCode(response, RESPONSE_OK) || response.length != requestSize + 1) {
                String code = (response == null || response.length == 0) ? "empty" : "code " + response[0];
                String info = response == null ? "null" : (code + " size " + response.length);
                logger.error("readImage: Something is wrong, retrying... " + info);
                continue;
            }

            ConnectionStatus.INSTANCE.markConnected();
            System.arraycopy(response, 1, image.getContent(), offset, requestSize);

            offset += requestSize;
        }
        setController(image);
        logger.info("Got configuration from controller.");
        ConnectionStatus.INSTANCE.setValue(ConnectionStatus.Value.CONNECTED);
    }

    /**
     * Blocking sending binary packet and waiting for a response
     *
     * @return null in case of IO issues
     */
    public byte[] executeCommand(byte[] packet, String msg, boolean allowLongResponse) {
        if (isClosed)
            return null;
        try {
            LinkManager.assertCommunicationThread();
            dropPending();

            sendPacket(packet);
            return receivePacket(msg, allowLongResponse);
        } catch (InterruptedException | IOException e) {
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
        packet[0] = COMMAND_CHUNK_WRITE;
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
            byte[] response = executeCommand(new byte[]{COMMAND_BURN}, "burn", false);
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

    private void sendPacket(byte[] command) throws IOException {
        stream.sendPacket(command, logger);
    }


    /**
     * This method blocks until a confirmation is received or {@link Timeouts#BINARY_IO_TIMEOUT} is reached
     *
     * @return true in case of timeout, false if got proper confirmation
     */
    private boolean sendTextCommand(String text) {
        byte[] command = getTextCommandBytes(text);

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

    public static byte[] getTextCommandBytes(String text) {
        byte[] asBytes = text.getBytes();
        byte[] command = new byte[asBytes.length + 1];
        command[0] = 'E';
        System.arraycopy(asBytes, 0, command, 1, asBytes.length);
        return command;
    }

    private String requestPendingMessages() {
        if (isClosed)
            return null;
        try {
            byte[] response = executeCommand(new byte[]{'G'}, "text", true);
            if (response != null && response.length == 1)
                Thread.sleep(100);
            //        System.out.println(result);
            return new String(response, 1, response.length - 1);
        } catch (InterruptedException e) {
            FileLog.MAIN.log(e);
            return null;
        }
    }

    public boolean requestOutputChannels() {
        if (isClosed)
            return false;

        byte packet[] = new byte[5];
        packet[0] = COMMAND_OUTPUTS;
        putShort(packet, 1, 0); // offset
        putShort(packet, 3, swap16(Fields.TS_OUTPUT_SIZE));

        byte[] response = executeCommand(packet, "output channels", false);
        if (response == null || response.length != (Fields.TS_OUTPUT_SIZE + 1) || response[0] != RESPONSE_OK)
            return false;

        currentOutputs = response;

        for (Sensor sensor : Sensor.values()) {
            ByteBuffer bb = ByteBuffer.wrap(response, 1 + sensor.getOffset(), 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);

            if (sensor.getType() == FieldType.FLOAT) {
                double value = bb.getFloat();
                SensorCentral.getInstance().setValue(value, sensor);
            } else if (sensor.getType() == FieldType.INT) {
                int value = bb.getInt();
                SensorCentral.getInstance().setValue(value, sensor);
            } else if (sensor.getType() == FieldType.INT16) {
                short value = (short) (bb.getInt() & 0xFFFF);
                SensorCentral.getInstance().setValue(value, sensor);
            } else if (sensor.getType() == null) {
                // do nothing for old text sensors which I am suprised are still in the code
            } else
                throw new UnsupportedOperationException("type " + sensor.getType());
        }
        return true;
    }
}
