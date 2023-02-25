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
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.util.IoUtils;
import org.jetbrains.annotations.NotNull;

import java.io.Closeable;
import java.io.IOException;
import java.util.Arrays;
import java.util.Objects;
import java.util.TreeSet;
import java.util.concurrent.*;

import static com.devexperts.logging.Logging.getLogging;

/**
 * See TcpCommunicationIntegrationTest
 *
 * @author Andrey Belomutskiy
 * 3/3/14
 */
public class LinkManager implements Closeable {
    private static final Logging log = getLogging(LinkManager.class);

    @NotNull
    public static LogLevel LOG_LEVEL = LogLevel.INFO;

    public static LinkDecoder ENCODER = new LinkDecoder() {
        @Override
        public String unpack(String packedLine) {
            return packedLine;
        }
    };

    private final CommandQueue commandQueue;

    private String lastTriedPort;

    private LinkConnector connector = LinkConnector.VOID;
    private boolean isStarted;
    private boolean needPullData = true;
    private boolean needPullText = true;
    private boolean needPullLiveData = true;
    public final MessagesListener messageListener = (source, message) -> System.out.println(source + ": " + message);
    private Thread communicationThread;

    public LinkManager() {
        Future<?> future = submit(() -> {
            communicationThread = Thread.currentThread();
            System.out.println("communicationThread lookup DONE");
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
    public static IoStream open(String port) throws IOException {
        if (TcpConnector.isTcpPort(port))
            return TcpIoStream.open(port);
        return BufferedSerialIoStream.openPort(port);
    }

    @NotNull
    public CountDownLatch connect(String port) {
        final CountDownLatch connected = new CountDownLatch(1);

        startAndConnect(port, new ConnectionStateListener() {
            @Override
            public void onConnectionFailed(String s) {
                IoUtils.exit("CONNECTION FAILED, did you specify the right port name?", -1);
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

    public static String[] getCommPorts() {
        SerialPort[] ports = SerialPort.getCommPorts();
        // wow sometimes driver returns same port name more than once?!
        TreeSet<String> names = new TreeSet<>();
        for (SerialPort port : ports)
            names.add(port.getSystemPortName());
        return names.toArray(new String[0]);
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
     *
     */
    public final ExecutorService COMMUNICATION_EXECUTOR = new ThreadPoolExecutor(1, 1,
            0L, TimeUnit.MILLISECONDS,
            COMMUNICATION_QUEUE,
            new NamedThreadFactory("communication executor"));

    public void assertCommunicationThread() {
        if (Thread.currentThread() != communicationThread) {
            IllegalStateException e = new IllegalStateException("Communication on wrong thread");
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

    public void startAndConnect(String port, ConnectionStateListener stateListener) {
        Objects.requireNonNull(port, "port");
        start(port, stateListener);
        connector.connectAndReadConfiguration(new BinaryProtocol.Arguments(true), stateListener);
    }

    @NotNull
    public LinkConnector getConnector() {
        return connector;
    }

    public void start(String port, ConnectionFailedListener stateListener) {
        Objects.requireNonNull(port, "port");
        log.info("LinkManager: Starting " + port);
        lastTriedPort = port; // Save port before connection attempt
        if (TcpConnector.isTcpPort(port)) {
            Callable<IoStream> streamFactory = new Callable<IoStream>() {
                @Override
                public IoStream call() {
                    messageListener.postMessage(getClass(), "Opening port: " + port);
                    try {
                        return TcpIoStream.open(port);
                    } catch (Throwable e) {
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

    public void send(String command, boolean fireEvent) throws InterruptedException {
        if (this.connector == null)
            throw new NullPointerException("connector");
        this.connector.send(command, fireEvent);
    }

    public void restart() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        close(); // Explicitly kill the connection (call connectors destructor??????)

        String[] ports = getCommPorts();
        boolean isPortAvailableAgain = Arrays.asList(ports).contains(lastTriedPort);
        log.info("restart isPortAvailableAgain=" + isPortAvailableAgain);
        if (isPortAvailableAgain) {
            connect(lastTriedPort);
        }
    }

    @Override
    public void close() {
        if (connector != null)
            connector.stop();
        isStarted = false; // Connector is dead and cant be in started state (Otherwise the Exception will raised)
    }

    public static String unpackConfirmation(String message) {
        if (message.startsWith(CommandQueue.CONFIRMATION_PREFIX))
            return message.substring(CommandQueue.CONFIRMATION_PREFIX.length());
        return null;
    }

    /**
     * @return null if no port located
     */
    public static String getDefaultPort() {
        String[] ports = getCommPorts();
        if (ports.length == 0) {
            System.out.println("Port not specified and no ports found");
            return null;
        }
        String port = ports[ports.length - 1];
        // todo: reuse 'PortDetector.autoDetectPort' here?
        System.out.println("Using last of " + ports.length + " port(s)");
        System.out.println("All ports: " + Arrays.toString(ports));
        return port;
    }

    public interface MessagesListener {
        void postMessage(Class<?> source, String message);
    }
}
