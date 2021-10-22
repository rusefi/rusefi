package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.opensr5.io.ConfigurationImageFile;
import com.opensr5.io.DataListener;
import com.rusefi.ConfigurationImageDiff;
import com.rusefi.NamedThreadFactory;
import com.rusefi.SignatureHelper;
import com.rusefi.Timeouts;
import com.rusefi.composite.CompositeEvent;
import com.rusefi.composite.CompositeParser;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Pair;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.*;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.stream.LogicdataStreamFile;
import com.rusefi.stream.StreamFile;
import com.rusefi.stream.TSHighSpeedLog;
import com.rusefi.stream.VcdStreamFile;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.livedocs.LiveDocsRegistry;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.EOFException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.*;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.IoHelper.*;
import static com.rusefi.config.generated.Fields.*;

/**
 * This object represents logical state of physical connection.
 * <p>
 * Instance is connected until we experience issues. Once we decide to close the connection there is no restart -
 * new instance of this class would need to be created once we establish a new physical connection.
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/6/2015
 */
public class BinaryProtocol {
    private static final Logging log = getLogging(BinaryProtocol.class);
    private static final ThreadFactory THREAD_FACTORY = new NamedThreadFactory("text pull");

    private static final String USE_PLAIN_PROTOCOL_PROPERTY = "protocol.plain";
    private static final String CONFIGURATION_RUSEFI_BINARY = "current_configuration.rusefi_binary";
    private static final String CONFIGURATION_RUSEFI_XML = "current_configuration.msq";
    private static final int HIGH_RPM_DELAY = Integer.getInteger("high_speed_logger_time", 10);
    /**
     * This properly allows to switch to non-CRC32 mode
     * todo: finish this feature, assuming we even need it.
     */
    public static boolean PLAIN_PROTOCOL = Boolean.getBoolean(USE_PLAIN_PROTOCOL_PROPERTY);

    private final LinkManager linkManager;
    private final IoStream stream;
    private final IncomingDataBuffer incomingData;
    private boolean isBurnPending;
    public String signature;

    private BinaryProtocolState state = new BinaryProtocolState();

    // todo: this ioLock needs better documentation!
    private final Object ioLock = new Object();

    public static final int COMPOSITE_OFF_RPM = Integer.getInteger("high_speed_logger_rpm", 300);

    /**
     * Composite logging turns off after 10 seconds of RPM above 300
     */
    private boolean needCompositeLogger;
    private boolean isCompositeLoggerEnabled;
    private long lastLowRpmTime = System.currentTimeMillis();

    private List<StreamFile> compositeLogs = new ArrayList<>();
    public static boolean DISABLE_LOCAL_CACHE;

    public static String findCommand(byte command) {
        switch (command) {
            case Fields.TS_PAGE_COMMAND:
                return "PAGE";
            case Fields.TS_COMMAND_F:
                return "PROTOCOL";
            case Fields.TS_CRC_CHECK_COMMAND:
                return "CRC_CHECK";
            case Fields.TS_BURN_COMMAND:
                return "BURN";
            case Fields.TS_HELLO_COMMAND:
                return "HELLO";
            case Fields.TS_READ_COMMAND:
                return "READ";
            case Fields.TS_GET_TEXT:
                return "TS_GET_TEXT";
            case Fields.TS_GET_FIRMWARE_VERSION:
                return "GET_FW_VERSION";
            case Fields.TS_CHUNK_WRITE_COMMAND:
                return "WRITE_CHUNK";
            case Fields.TS_OUTPUT_COMMAND:
                return "TS_OUTPUT_COMMAND";
            case Fields.TS_RESPONSE_OK:
                return "TS_RESPONSE_OK";
            default:
                return "command " + (char) command + "/" + command;
        }
    }

    private void createCompositesIfNeeded() {
        if (!compositeLogs.isEmpty())
            return;
        compositeLogs.addAll(Arrays.asList(
                new VcdStreamFile(getFileName("rusEFI_trigger_log_", ".vcd")),
                new LogicdataStreamFile(getFileName("rusEFI_trigger_log_", ".logicdata")),
                new TSHighSpeedLog(getFileName("rusEFI_trigger_log_"))
        ));
    }

    public IoStream getStream() {
        return stream;
    }

    public boolean isClosed;

    public CommunicationLoggingListener communicationLoggingListener = CommunicationLoggingListener.VOID;

    public byte[] getCurrentOutputs() {
        return state.getCurrentOutputs();
    }

    public void setCurrentOutputs(byte[] currentOutputs) {
        state.setCurrentOutputs(currentOutputs);
    }

    private SensorCentral.SensorListener rpmListener;

    private final Thread hook = new Thread(() -> closeComposites(), "BinaryProtocol::hook");

    public BinaryProtocol(LinkManager linkManager, IoStream stream, IncomingDataBuffer dataBuffer) {
        this.linkManager = linkManager;
        this.stream = stream;

        communicationLoggingListener = new CommunicationLoggingListener() {
            @Override
            public void onPortHolderMessage(Class clazz, String message) {
                linkManager.messageListener.postMessage(clazz, message);
            }
        };

        incomingData = dataBuffer;
        Runtime.getRuntime().addShutdownHook(hook);
        needCompositeLogger = linkManager.getCompositeLogicEnabled();
        rpmListener = value -> {
            if (value <= COMPOSITE_OFF_RPM) {
                needCompositeLogger = linkManager.getCompositeLogicEnabled();
                lastLowRpmTime = System.currentTimeMillis();
            } else if (System.currentTimeMillis() - lastLowRpmTime > HIGH_RPM_DELAY * Timeouts.SECOND) {
                needCompositeLogger = false;
            }
        };
    }

    public static void sleep(long millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    @NotNull
    public static String getFileName(String prefix) {
        return getFileName(prefix, ".csv");
    }

    @NotNull
    public static String getFileName(String prefix, String fileType) {
        return Logger.DIR + prefix + Logger.getDate() + fileType;
    }

    public void doSend(final String command, boolean fireEvent) throws InterruptedException {
        log.info("Sending [" + command + "]");
        if (fireEvent && LinkManager.LOG_LEVEL.isDebugEnabled()) {
            communicationLoggingListener.onPortHolderMessage(BinaryProtocol.class, "Sending [" + command + "]");
        }

        Future f = linkManager.submit(new Runnable() {
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
            log.error("timeout sending [" + command + "] giving up: " + e);
            return;
        }
        /**
         * this here to make CommandQueue happy
         */
        linkManager.getCommandQueue().handleConfirmationMessage(CommandQueue.CONFIRMATION_PREFIX + command);
    }

    public String getSignature() throws IOException {
        HelloCommand.send(stream);
        return HelloCommand.getHelloResponse(incomingData);
    }

    /**
     * this method reads configuration snapshot from controller
     *
     * @return true if everything fine
     */
    public boolean connectAndReadConfiguration(DataListener listener) {
        try {
            signature = getSignature();
            System.out.println("Got " + signature);
            SignatureHelper.downloadIfNotAvailable(SignatureHelper.getUrl(signature));
        } catch (IOException e) {
            return false;
        }
        readImage(Fields.TOTAL_CONFIG_SIZE);
        if (isClosed)
            return false;

        startTextPullThread(listener);
        SensorCentral.getInstance().addListener(Sensor.RPM, rpmListener);
        return true;
    }

    private void startTextPullThread(final DataListener listener) {
        if (!linkManager.COMMUNICATION_QUEUE.isEmpty()) {
            log.info("Current queue: " + linkManager.COMMUNICATION_QUEUE.size());
        }
        Runnable textPull = new Runnable() {
            @Override
            public void run() {
                while (!isClosed) {
//                    FileLog.rlog("queue: " + LinkManager.COMMUNICATION_QUEUE.toString());
                    if (linkManager.COMMUNICATION_QUEUE.isEmpty() && linkManager.getNeedPullData()) {
                        linkManager.submit(new Runnable() {
                            @Override
                            public void run() {
                                if (requestOutputChannels())
                                    HeartBeatListeners.onDataArrived();
                                compositeLogic();
                                String text = requestPendingMessages();
                                if (text != null)
                                    listener.onDataArrived((text + "\r\n").getBytes());
                                LiveDocsRegistry.INSTANCE.refresh(BinaryProtocol.this);
                            }
                        });
                    }
                    sleep(Timeouts.TEXT_PULL_PERIOD);
                }
                log.info("Stopping text pull");
            }
        };
        Thread tr = THREAD_FACTORY.newThread(textPull);
        tr.start();
    }

    private void compositeLogic() {
        if (needCompositeLogger) {
            getComposite();
        } else if (isCompositeLoggerEnabled) {
            byte packet[] = new byte[2];
            packet[0] = Fields.TS_SET_LOGGER_SWITCH;
            packet[1] = Fields.TS_COMPOSITE_DISABLE;
            executeCommand(packet, "disable composite");
            isCompositeLoggerEnabled = false;
            closeComposites();
        }
    }

    private void closeComposites() {
        for (StreamFile composite : compositeLogs) {
            composite.close();
        }
        compositeLogs.clear();
    }

    private void dropPending() {
        synchronized (ioLock) {
            if (isClosed)
                return;
            incomingData.dropPending();
        }
    }

    public void uploadChanges(ConfigurationImage newVersion) {
        ConfigurationImage current = getControllerConfiguration();
        // let's have our own copy which no one would be able to change
        newVersion = newVersion.clone();
        int offset = 0;
        while (offset < current.getSize()) {
            Pair<Integer, Integer> range = ConfigurationImageDiff.findDifferences(current, newVersion, offset);
            if (range == null)
                break;
            int size = range.second - range.first;
            log.info("Need to patch: " + range + ", size=" + size);
            byte[] oldBytes = current.getRange(range.first, size);
            log.info("old " + Arrays.toString(oldBytes));

            byte[] newBytes = newVersion.getRange(range.first, size);
            log.info("new " + Arrays.toString(newBytes));

            writeData(newVersion.getContent(), 0, range.first, size);

            offset = range.second;
        }
        burn();
        setController(newVersion);
    }

    private byte[] receivePacket(String msg, boolean allowLongResponse) throws IOException {
        long start = System.currentTimeMillis();
        synchronized (ioLock) {
            return incomingData.getPacket(msg, allowLongResponse, start);
        }
    }

    /**
     * read complete tune from physical data stream
     */
    public void readImage(int size) {
        ConfigurationImage image = getAndValidateLocallyCached();

        if (image == null) {
            image = readFullImageFromController(size);
            if (image == null)
                return;
        }
        setController(image);
        log.info("Got configuration from controller.");
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
    }

    @Nullable
    private ConfigurationImage readFullImageFromController(int size) {
        ConfigurationImage image;
        image = new ConfigurationImage(size);

        int offset = 0;

        long start = System.currentTimeMillis();
        log.info("Reading from controller...");

        while (offset < image.getSize() && (System.currentTimeMillis() - start < Timeouts.READ_IMAGE_TIMEOUT)) {
            if (isClosed)
                return null;

            int remainingSize = image.getSize() - offset;
            int requestSize = Math.min(remainingSize, Fields.BLOCKING_FACTOR);

            byte packet[] = new byte[5];
            packet[0] = Fields.TS_READ_COMMAND;
            putShort(packet, 1, swap16(offset));
            putShort(packet, 3, swap16(requestSize));

            byte[] response = executeCommand(packet, "load image offset=" + offset);

            if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK) || response.length != requestSize + 1) {
                String code = (response == null || response.length == 0) ? "empty" : "code " + getCode(response);
                String info = response == null ? "NO RESPONSE" : (code + " size " + response.length);
                log.info("readImage: ERROR UNEXPECTED Something is wrong, retrying... " + info);
                continue;
            }

            HeartBeatListeners.onDataArrived();
            ConnectionStatusLogic.INSTANCE.markConnected();
            System.arraycopy(response, 1, image.getContent(), offset, requestSize);

            offset += requestSize;
        }
        try {
            ConfigurationImageFile.saveToFile(image, CONFIGURATION_RUSEFI_BINARY);
            Msq tune = MsqFactory.valueOf(image);
            tune.writeXmlFile(CONFIGURATION_RUSEFI_XML);
        } catch (Exception e) {
            System.err.println("Ignoring " + e);
        }
        return image;
    }

    private static String getCode(byte[] response) {
        int b = response[0] & 0xff;
        switch (b) {
            case TS_RESPONSE_CRC_FAILURE:
                return "CRC_FAILURE";
            case TS_RESPONSE_UNRECOGNIZED_COMMAND:
                return "UNRECOGNIZED_COMMAND";
            case TS_RESPONSE_OUT_OF_RANGE:
                return "OUT_OF_RANGE";
            case TS_RESPONSE_FRAMING_ERROR:
                return "FRAMING_ERROR";
        }
        return Integer.toString(b);
    }

    private ConfigurationImage getAndValidateLocallyCached() {
        if (DISABLE_LOCAL_CACHE)
            return null;
        ConfigurationImage localCached;
        try {
            localCached = ConfigurationImageFile.readFromFile(CONFIGURATION_RUSEFI_BINARY);
        } catch (IOException e) {
            System.err.println("Error reading " + CONFIGURATION_RUSEFI_BINARY + ": no worries " + e);
            return null;
        }

        if (localCached != null) {
            int crcOfLocallyCachedConfiguration = IoHelper.getCrc32(localCached.getContent());
            log.info(String.format(CONFIGURATION_RUSEFI_BINARY + " Local cache CRC %x\n", crcOfLocallyCachedConfiguration));

            byte packet[] = new byte[5];
            packet[0] = Fields.TS_CRC_CHECK_COMMAND;
            putShort(packet, 1, swap16(/*offset = */ 0));
            putShort(packet, 3, swap16(localCached.getSize()));
            byte[] response = executeCommand(packet, "get CRC32");

            if (checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK) && response.length == 5) {
                ByteBuffer bb = ByteBuffer.wrap(response, 1, 4);
                // that's unusual - most of the protocol is LITTLE_ENDIAN
                bb.order(ByteOrder.BIG_ENDIAN);
                int crcFromController = bb.getInt();
                log.info(String.format("From rusEFI tune CRC32 0x%x %d\n", crcFromController, crcFromController));
                short crc16FromController = (short) crcFromController;
                log.info(String.format("From rusEFI tune CRC16 0x%x %d\n", crc16FromController, crc16FromController));
                if (crcOfLocallyCachedConfiguration == crcFromController) {
                    return localCached;
                }
            }
        }
        return null;
    }

    public byte[] executeCommand(byte[] packet, String msg) {
        return executeCommand(packet, msg, false);
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
            linkManager.assertCommunicationThread();
            dropPending();

            sendPacket(packet);
            return receivePacket(msg, allowLongResponse);
        } catch (IOException e) {
            log.error(msg + ": executeCommand failed: " + e);
            close();
            return null;
        }
    }

    public void close() {
        if (isClosed)
            return;
        isClosed = true;
        SensorCentral.getInstance().removeListener(Sensor.RPM, rpmListener);
        stream.close();
        closeComposites();
        Runtime.getRuntime().removeShutdownHook(hook);
    }

    public void writeData(byte[] content, int contentOffset, int ecuOffset, int size) {
        isBurnPending = true;

        byte[] packet = new byte[5 + size];
        packet[0] = Fields.TS_CHUNK_WRITE_COMMAND;
        putShort(packet, 1, swap16(ecuOffset));
        putShort(packet, 3, swap16(size));

        System.arraycopy(content, contentOffset, packet, 5, size);

        long start = System.currentTimeMillis();
        while (!isClosed && (System.currentTimeMillis() - start < Timeouts.BINARY_IO_TIMEOUT)) {
            byte[] response = executeCommand(packet, "writeImage");
            if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK) || response.length != 1) {
                log.error("writeData: Something is wrong, retrying...");
                continue;
            }
            break;
        }
    }

    public void burn() {
        if (!isBurnPending)
            return;
        log.info("Need to burn");

        while (true) {
            if (isClosed)
                return;
            byte[] response = executeCommand(new byte[]{Fields.TS_BURN_COMMAND}, "burn");
            if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_BURN_OK) || response.length != 1) {
                continue;
            }
            break;
        }
        log.info("DONE");
        isBurnPending = false;
    }

    public void setController(ConfigurationImage controller) {
        state.setController(controller);
    }

    /**
     * Configuration as it is in the controller to the best of our knowledge
     */
    public ConfigurationImage getControllerConfiguration() {
        return state.getControllerConfiguration();
    }

    private void sendPacket(byte[] command) throws IOException {
        stream.sendPacket(command);
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
            if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_COMMAND_OK) || response.length != 1) {
                continue;
            }
            return false;
        }
        return true;
    }

    public static byte[] getTextCommandBytes(String text) {
        byte[] asBytes = text.getBytes();
        byte[] command = new byte[asBytes.length + 1];
        command[0] = Fields.TS_EXECUTE;
        System.arraycopy(asBytes, 0, command, 1, asBytes.length);
        return command;
    }

    private String requestPendingMessages() {
        if (isClosed)
            return null;
        try {
            byte[] response = executeCommand(new byte[]{Fields.TS_GET_TEXT}, "text", true);
            if (response != null && response.length == 1)
                Thread.sleep(100);
            return new String(response, 1, response.length - 1);
        } catch (InterruptedException e) {
            log.error(e.toString());
            return null;
        }
    }

    public void getComposite() {
        if (isClosed)
            return;

        byte packet[] = new byte[1];
        packet[0] = Fields.TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY;
        // get command would enable composite logging in controller but we need to turn it off from our end
        // todo: actually if console gets disconnected composite logging might end up enabled in controller?
        isCompositeLoggerEnabled = true;

        byte[] response = executeCommand(packet, "composite log", true);
        if (checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK)) {
            List<CompositeEvent> events = CompositeParser.parse(response);
            createCompositesIfNeeded();
            for (StreamFile composite : compositeLogs)
                composite.append(events);
        }
    }

    public boolean requestOutputChannels() {
        if (isClosed)
            return false;

        byte[] packet = GetOutputsCommand.createRequest();

        byte[] response = executeCommand(packet, "output channels", false);
        if (response == null || response.length != (Fields.TS_OUTPUT_SIZE + 1) || response[0] != Fields.TS_RESPONSE_OK)
            return false;

        state.setCurrentOutputs(response);

        SensorCentral.getInstance().grabSensorValues(response);
        return true;
    }

    public void setRange(byte[] src, int scrPos, int offset, int count) {
        state.setRange(src, scrPos, offset, count);
    }

    public BinaryProtocolState getBinaryProtocolState() {
        return state;
    }
}
