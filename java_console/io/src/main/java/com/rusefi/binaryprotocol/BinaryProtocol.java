package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.io.ConfigurationImageFile;
import com.opensr5.io.DataListener;
import com.rusefi.ConfigurationImageDiff;
import com.rusefi.NamedThreadFactory;
import com.rusefi.SignatureHelper;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.test.Bug3923;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Pair;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.*;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.shared.FileUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.livedocs.LiveDocsRegistry;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
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
    public boolean isGoodOutputChannels;

    private final BinaryProtocolState state = new BinaryProtocolState();

    // todo: this ioLock needs better documentation!
    private final Object ioLock = new Object();

    BinaryProtocolLogger binaryProtocolLogger;
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

    public IoStream getStream() {
        return stream;
    }

    public boolean isClosed;

    public CommunicationLoggingListener communicationLoggingListener;

    public BinaryProtocol(LinkManager linkManager, IoStream stream) {
        this.linkManager = linkManager;
        this.stream = stream;

        communicationLoggingListener = linkManager.messageListener::postMessage;

        incomingData = stream.getDataBuffer();
        binaryProtocolLogger = new BinaryProtocolLogger(linkManager);
        binaryProtocolLogger.needCompositeLogger = linkManager.getCompositeLogicEnabled();
    }

    public static void sleep(long millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
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

    public static String getSignature(IoStream stream) throws IOException {
        HelloCommand.send(stream);
        return HelloCommand.getHelloResponse(stream.getDataBuffer());
    }

    /**
     * this method reads configuration snapshot from controller
     *
     * @return true if everything fine
     */
    public String connectAndReadConfiguration(Arguments arguments, DataListener listener) {
        try {
            signature = getSignature(stream);
            log.info("Got " + signature + " signature");
            SignatureHelper.downloadIfNotAvailable(SignatureHelper.getUrl(signature));
        } catch (IOException e) {
            return "Failed to read signature " + e;
        }

        String errorMessage = validateConfigVersion();
        if (errorMessage != null)
            return errorMessage;

        readImage(arguments, Fields.TOTAL_CONFIG_SIZE);
        if (isClosed)
            return "Failed to read calibration";

        startPullThread(listener);
        binaryProtocolLogger.start();
        return null;
    }

    /**
     * @return null if everything is good, error message otherwise
     */
    private String validateConfigVersion() {
        int requestSize = 4;
        byte[] packet = GetOutputsCommand.createRequest(TS_FILE_VERSION_OFFSET, requestSize);

        String msg = "load TS_CONFIG_VERSION";
        byte[] response = executeCommand(Fields.TS_OUTPUT_COMMAND, packet, msg);
        if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK) || response.length != requestSize + 1) {
            close();
            return "Failed to " + msg;
        }
        int actualVersion = FileUtil.littleEndianWrap(response, 1, requestSize).getInt();
        if (actualVersion != TS_FILE_VERSION) {
            log.error("Got TS_CONFIG_VERSION " + actualVersion);
            return "Incompatible firmware format=" + actualVersion + " while format " + TS_FILE_VERSION + " expected";
        }
        return null;
    }

    private void startPullThread(final DataListener textListener) {
        if (!linkManager.COMMUNICATION_QUEUE.isEmpty()) {
            log.info("Current queue size: " + linkManager.COMMUNICATION_QUEUE.size());
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
                                isGoodOutputChannels = requestOutputChannels();
                                if (isGoodOutputChannels)
                                    HeartBeatListeners.onDataArrived();
                                binaryProtocolLogger.compositeLogic(BinaryProtocol.this);
                                if (linkManager.isNeedPullText()) {
                                    String text = requestPendingMessages();
                                    if (text != null)
                                        textListener.onDataArrived((text + "\r\n").getBytes());
                                }
                                if (linkManager.isNeedPullLiveData()) {
                                    LiveDocsRegistry.LiveDataProvider liveDataProvider = LiveDocsRegistry.getLiveDataProvider();
                                    LiveDocsRegistry.INSTANCE.refresh(liveDataProvider);
                                }
                            }
                        });
                    }
                    sleep(Timeouts.TEXT_PULL_PERIOD);
                }
                log.info("Port shutdown: Stopping text pull");
            }
        };
        Thread tr = THREAD_FACTORY.newThread(textPull);
        tr.start();
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

    private byte[] receivePacket(String msg) throws IOException {
        long start = System.currentTimeMillis();
        synchronized (ioLock) {
            return incomingData.getPacket(msg, start);
        }
    }

    /**
     * read complete tune from physical data stream
     */
    public void readImage(Arguments arguments, int size) {
        ConfigurationImage image = getAndValidateLocallyCached();

        if (image == null) {
            image = readFullImageFromController(arguments, size);
            if (image == null)
                return;
        }
        setController(image);
        log.info("Got configuration from controller.");
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
    }

    public static class Arguments {
        final boolean saveFile;

        public Arguments(boolean saveFile) {
            this.saveFile = saveFile;
        }
    }

    @Nullable
    private ConfigurationImage readFullImageFromController(Arguments arguments, int size) {
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

            byte[] packet = new byte[4];
            putShort(packet, 0, swap16(offset));
            putShort(packet, 2, swap16(requestSize));

            byte[] response = executeCommand(Fields.TS_READ_COMMAND, packet, "load image offset=" + offset);

            if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK) || response.length != requestSize + 1) {
                String code = (response == null || response.length == 0) ? "empty" : "ERROR_CODE=" + getCode(response);
                String info = response == null ? "NO RESPONSE" : (code + " length=" + response.length);
                log.info("readImage: ERROR UNEXPECTED Something is wrong, retrying... " + info);
                // todo: looks like forever retry? that's weird
                continue;
            }

            HeartBeatListeners.onDataArrived();
            ConnectionStatusLogic.INSTANCE.markConnected();
            System.arraycopy(response, 1, image.getContent(), offset, requestSize);

            offset += requestSize;
        }
        if (arguments != null && arguments.saveFile) {
            try {
                ConfigurationImageFile.saveToFile(image, CONFIGURATION_RUSEFI_BINARY);
                Msq tune = MsqFactory.valueOf(image);
                tune.writeXmlFile(CONFIGURATION_RUSEFI_XML);
            } catch (Exception e) {
                System.err.println("Ignoring " + e);
            }
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
            case TS_RESPONSE_UNDERRUN:
                return "TS_RESPONSE_UNDERRUN";
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

            int crcFromController = getCrcFromController(localCached.getSize());

            if (crcOfLocallyCachedConfiguration == crcFromController) {
                return localCached;
            }

        }
        return null;
    }

    public int getCrcFromController(int configSize) {
        byte[] packet = createCrcCommand(configSize);
        byte[] response = executeCommand(Fields.TS_CRC_CHECK_COMMAND, packet, "get CRC32");

        if (checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK) && response.length == 5) {
            ByteBuffer bb = ByteBuffer.wrap(response, 1, 4);
            // that's unusual - most of the protocol is LITTLE_ENDIAN
            bb.order(ByteOrder.BIG_ENDIAN);
            int crcFromController = bb.getInt();
            log.info(String.format("rusEFI says tune CRC32 0x%x %d\n", crcFromController, crcFromController));
            short crc16FromController = (short) crcFromController;
            log.info(String.format("rusEFI says tune CRC16 0x%x %d\n", crc16FromController, crc16FromController));
            return crcFromController;
        } else {
            return  -1;
        }
    }

    public static byte[] createCrcCommand(int size) {
        byte[] packet = new byte[4];
        putShort(packet, 0, swap16(/*offset = */ 0));
        putShort(packet, 2, swap16(size));
        return packet;
    }

    public byte[] executeCommand(char opcode, String msg) {
        return executeCommand(opcode, null, msg);
    }

    /**
     * Blocking sending binary packet and waiting for a response
     *
     * @return null in case of IO issues
     */
    public byte[] executeCommand(char opcode, byte[] packet, String msg) {
        if (isClosed)
            return null;

        byte[] fullRequest;

        if (packet != null) {
            fullRequest = new byte[packet.length + 1];
            System.arraycopy(packet, 0, fullRequest, 1, packet.length);
        } else {
            fullRequest = new byte[1];
        }

        fullRequest[0] = (byte)opcode;

        try {
            linkManager.assertCommunicationThread();
            dropPending();
            if (Bug3923.obscene)
                log.info("Sending opcode " + opcode + " payload " + packet.length);
            sendPacket(fullRequest);
            return receivePacket(msg);
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
        binaryProtocolLogger.close();
        stream.close();
    }

    public void writeData(byte[] content, int contentOffset, int ecuOffset, int size) {
        isBurnPending = true;

        byte[] packet = new byte[4 + size];
        putShort(packet, 0, swap16(ecuOffset));
        putShort(packet, 2, swap16(size));

        System.arraycopy(content, contentOffset, packet, 4, size);

        long start = System.currentTimeMillis();
        while (!isClosed && (System.currentTimeMillis() - start < Timeouts.BINARY_IO_TIMEOUT)) {
            byte[] response = executeCommand(Fields.TS_CHUNK_WRITE_COMMAND, packet, "writeImage");
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
            byte[] response = executeCommand(Fields.TS_BURN_COMMAND, "burn");
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
        byte[] command = getTextCommandBytesOnlyText(text);

        long start = System.currentTimeMillis();
        while (!isClosed && (System.currentTimeMillis() - start < Timeouts.BINARY_IO_TIMEOUT)) {
            byte[] response = executeCommand(Fields.TS_EXECUTE, command, "execute");
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

    public static byte[] getTextCommandBytesOnlyText(String text) {
        return text.getBytes();
    }

    private String requestPendingMessages() {
        if (isClosed)
            return null;
        try {
            byte[] response = executeCommand(Fields.TS_GET_TEXT, "text");
            if (response != null && response.length == 1)
                Thread.sleep(100);
            return new String(response, 1, response.length - 1);
        } catch (InterruptedException e) {
            log.error(e.toString());
            return null;
        }
    }

    public boolean requestOutputChannels() {
        if (isClosed)
            return false;

        // TODO: Get rid of the +1.  This adds a byte at the front to tack a fake TS response code on the front
        //  of the reassembled packet.
        byte[] reassemblyBuffer = new byte[TS_TOTAL_OUTPUT_SIZE + 1];
        reassemblyBuffer[0] = Fields.TS_RESPONSE_OK;

        int reassemblyIdx = 0;
        int remaining = TS_TOTAL_OUTPUT_SIZE;

        while (remaining > 0) {
            // If less than one full chunk left, do a smaller read
            int chunkSize = Math.min(remaining, Fields.BLOCKING_FACTOR);

            byte[] response = executeCommand(
                Fields.TS_OUTPUT_COMMAND,
                GetOutputsCommand.createRequest(reassemblyIdx, chunkSize),
                "output channels"
            );

            if (response == null || response.length != (chunkSize + 1) || response[0] != Fields.TS_RESPONSE_OK) {
                return false;
            }

            // Copy this chunk in to the reassembly buffer
            System.arraycopy(response, 1, reassemblyBuffer, reassemblyIdx + 1, chunkSize);
            remaining -= chunkSize;
        }

        state.setCurrentOutputs(reassemblyBuffer);

        SensorCentral.getInstance().grabSensorValues(reassemblyBuffer);
        return true;
    }

    public BinaryProtocolState getBinaryProtocolState() {
        return state;
    }
}
