package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.rusefi.Callable;
import com.rusefi.NamedThreadFactory;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.core.EngineState;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.io.serial.StreamConnector;
import com.rusefi.io.can.PCanIoStream;
import com.rusefi.io.can.SocketCANIoStream;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.util.IoUtils;
import org.jetbrains.annotations.NotNull;

import java.io.Closeable;
import java.util.Arrays;
import java.util.Objects;
import java.util.Set;
import java.util.TreeSet;
import java.util.concurrent.*;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

/**
 * See TcpCommunicationIntegrationTest
 *
 * @author Andrey Belomutskiy
 * 3/3/14
 */
public class LinkManager implements Closeable {
    private static final Logging log = getLogging(LinkManager.class);
    public static final String PCAN = "PCAN";
    public static final String SOCKET_CAN = "SocketCAN";

    @NotNull
    public static final LogLevel LOG_LEVEL = LogLevel.INFO;

    public static final LinkDecoder ENCODER = new LinkDecoder() {
        @Override
        public String unpack(String packedLine) {
            return packedLine;
        }
    };

    public static final String LOG_VIEWER = "log viewer";
    private final CommandQueue commandQueue;

    private String lastTriedPort;

    private LinkConnector connector = LinkConnector.VOID;
    private boolean isStarted;
    private boolean compositeLogicEnabled = true;
    private boolean needPullData = true;
    private boolean needPullText = true;
    private boolean needPullLiveData = true;
    public final MessagesListener messageListener = (source, message) -> log.info(source + ": " + message);
    private Thread communicationThread;
    private boolean isDisconnectedByUser;

    public LinkManager() {
        Future<?> future = submit(() -> {
            communicationThread = Thread.currentThread();
            log.info("communicationThread lookup DONE");
        });
        try {
            // let's wait for the above trivial task to finish
            future.get();
        } catch (InterruptedException | ExecutionException e) {
            throw new IllegalStateException(e);
        }

        engineState = new EngineState(new EngineState.EngineStateListenerImpl() {
            @Override
            public void beforeLine(String fullLine) {
                //log.info(fullLine);
                HeartBeatListeners.onDataArrived();
            }
        });
        commandQueue = new CommandQueue(this);
    }

    @NotNull
    public CountDownLatch connect(String port, boolean isScanningForEcu) {
        final CountDownLatch connected = new CountDownLatch(1);

        startAndConnect(port, new ConnectionStateListener() {
            @Override
            public void onConnectionFailed(String s) {
                if (!isScanningForEcu)
                    IoUtils.exit("TERMINATING: CONNECTION FAILED, did you specify the right port name? " + s, -1);
            }

            @Override
            public void onConnectionEstablished() {
                connected.countDown();
            }
        });

        return connected;
    }

    public void execute(Runnable runnable) {
        COMMUNICATION_EXECUTOR.execute(runnable);
    }

    public Future submit(Runnable runnable) {
        return COMMUNICATION_EXECUTOR.submit(runnable);
    }

    public static Set<String> getCommPorts() {
        SerialPort[] ports = SerialPort.getCommPorts();
        // wow sometimes driver returns same port name more than once?!
        return Arrays.stream(ports).map(SerialPort::getSystemPortName).collect(Collectors.toCollection(TreeSet::new));
    }

    public BinaryProtocol getBinaryProtocol() {
        return getCurrentStreamState();
    }

    public BinaryProtocol getCurrentStreamState() {
        Objects.requireNonNull(connector, "connector");
        return connector.getBinaryProtocol();
    }

    public BinaryProtocolState getBinaryProtocolState() {
        return connector.getBinaryProtocolState();
    }

    public CommandQueue getCommandQueue() {
        return commandQueue;
    }

    public LinkManager setCompositeLogicEnabled(boolean compositeLogicEnabled) {
        this.compositeLogicEnabled = compositeLogicEnabled;
        return this;
    }

    public boolean getCompositeLogicEnabled() {
        return compositeLogicEnabled;
    }

    public boolean getNeedPullData() {
        return needPullData;
    }

    public boolean isNeedPullText() {
        return needPullText;
    }

    public boolean isNeedPullLiveData() {
        return needPullLiveData;
    }

    public LinkManager setNeedPullLiveData(boolean needPullLiveData) {
        this.needPullLiveData = needPullLiveData;
        return this;
    }

    public LinkManager setNeedPullData(boolean needPullData) {
        this.needPullData = needPullData;
        return this;
    }

    public LinkManager setNeedPullText(boolean needPullText) {
        this.needPullText = needPullText;
        return this;
    }

    public void disconnect() {
        log.info("disconnect");
        isDisconnectedByUser = true;
        close();
    }

    public void reconnect() {
        log.info("reconnect");
        isDisconnectedByUser = false;
        restart();
    }

    public enum LogLevel {
        INFO,
        DEBUG,
        TRACE;

        public boolean isDebugEnabled() {
            return this == DEBUG || this == TRACE;
        }
    }

    public final LinkedBlockingQueue<Runnable> COMMUNICATION_QUEUE = new LinkedBlockingQueue<>();
    /**
     * All request/responses to underlying controller are happening on this single-threaded executor in a FIFO manner
     */
    public final ExecutorService COMMUNICATION_EXECUTOR = new ThreadPoolExecutor(1, 1,
            0L, TimeUnit.MILLISECONDS,
            COMMUNICATION_QUEUE,
            new NamedThreadFactory("ECU Communication Executor", true));

    public void assertCommunicationThread() {
        if (Thread.currentThread() != communicationThread) {
            IllegalStateException e = new IllegalStateException("Communication on wrong thread. Use linkManager.execute or linkManager.submit");
            e.printStackTrace();
            log.error(e.getMessage(), e);
            throw e;
        }
    }

    private final EngineState engineState;

    public EngineState getEngineState() {
        return engineState;
    }

    /**
     * This flag controls if mock controls are needed
     * todo: decouple from TcpConnector since not really related
     */
    public static boolean isSimulationMode;

    public void startAndConnect(
        final String port,
        final ConnectionStateListener stateListener
    ) {
        Objects.requireNonNull(port, "port");
        start(port, stateListener);
        connector.connectAndReadConfiguration(
            new BinaryProtocol.Arguments(true),
            stateListener
        );
    }

    @NotNull
    public LinkConnector getConnector() {
        return connector;
    }

    public void start(String port, ConnectionFailedListener stateListener) {
        Objects.requireNonNull(port, "port");
        log.info("LinkManager: Starting " + port);
        lastTriedPort = port; // Save port before connection attempt
        if (isLogViewerMode(port)) {
            setConnector(LinkConnector.VOID);
        } else if (PCAN.equals(port)) {
            Callable<IoStream> streamFactory = PCanIoStream::createStream;
            setConnector(new StreamConnector(this, streamFactory));
        } else if (SOCKET_CAN.equals(port)) {
            Callable<IoStream> streamFactory = SocketCANIoStream::createStream;
            setConnector(new StreamConnector(this, streamFactory));
        } else if (TcpConnector.isTcpPort(port)) {
            Callable<IoStream> streamFactory = new Callable<IoStream>() {
                @Override
                public IoStream call() {
                    messageListener.postMessage(getClass(), "Opening TCP port: " + port);
                    try {
                        return TcpIoStream.open(port);
                    } catch (Throwable e) {
                        log.error("TCP error " + e);
                        stateListener.onConnectionFailed("Error " + e);
                        return null;
                    }
                }
            };

            setConnector(new StreamConnector(this, streamFactory));
            isSimulationMode = true;
        } else {
            Callable<IoStream> ioStreamCallable = new Callable<IoStream>() {
                @Override
                public IoStream call() {
                    messageListener.postMessage(getClass(), "Opening port: " + port);
                    IoStream stream = BufferedSerialIoStream.openPort(port);
                    if (stream == null) {
                        // error already reported
                        return null;
                    }
                    return stream;
                }
            };
            setConnector(new StreamConnector(this, ioStreamCallable));
        }
    }

    public void setConnector(LinkConnector connector) {
        if (isStarted) {
            throw new IllegalStateException("Already started");
        }
        isStarted = true;
        this.connector = connector;
    }

    public static boolean isLogViewerMode(String port) {
        Objects.requireNonNull(port, "port");
        return port.equals(LOG_VIEWER);
    }

    public boolean isLogViewer() {
        return connector == LinkConnector.VOID;
    }

    public void send(String command, boolean fireEvent) throws InterruptedException {
        if (this.connector == null)
            throw new NullPointerException("connector");
        this.connector.send(command, fireEvent);
    }

    public void restart() {
        if (isDisconnectedByUser)
            return;
        close(); // Explicitly kill the connection (call connectors destructor??????)

        final Set<String> ports = getCommPorts();
        final boolean isPortAvailableAgain = ports.contains(lastTriedPort);
        log.info("restart isPortAvailableAgain=" + isPortAvailableAgain);
        if (isPortAvailableAgain) {
            connect(lastTriedPort, false);
        }
    }

    @Override
    public void close() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        if (connector != null) {
            connector.stop();
        }
        isStarted = false; // Connector is dead and cant be in started state (Otherwise the Exception will raised)
    }

    public static String unpackConfirmation(String message) {
        if (message.startsWith(CommandQueue.CONFIRMATION_PREFIX))
            return message.substring(CommandQueue.CONFIRMATION_PREFIX.length());
        return null;
    }

    public interface MessagesListener {
        void postMessage(Class<?> source, String message);
    }
}
