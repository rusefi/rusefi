package com.rusefi.io;

import com.rusefi.FileLog;
import com.rusefi.NamedThreadFactory;
import com.rusefi.core.EngineState;
import com.rusefi.io.serial.SerialConnector;
import com.rusefi.io.tcp.TcpConnector;
import jssc.SerialPortList;
import org.jetbrains.annotations.NotNull;

import java.util.Arrays;
import java.util.concurrent.*;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class LinkManager {
    @NotNull
    public static CountDownLatch connect(String port) {
        final CountDownLatch connected = new CountDownLatch(1);
        startAndConnect(port, new ConnectionStateListener() {
            @Override
            public void onConnectionFailed() {
                System.out.println("CONNECTION FAILED, did you specify the right port name?");
                System.exit(-1);
            }

            @Override
            public void onConnectionEstablished() {
                connected.countDown();
            }
        });
        try {
            connected.await(60, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
        return connected;
    }

    public static void execute(Runnable runnable) {
        COMMUNICATION_EXECUTOR.execute(runnable);
    }

    public static Future submit(Runnable runnable) {
        return COMMUNICATION_EXECUTOR.submit(runnable);
    }

    public enum LogLevel {
        INFO,
        DEBUG,
        TRACE;

        public boolean isDebugEnabled() {
            return this == DEBUG || this == TRACE;
        }
    }

    @NotNull
    public static LogLevel LOG_LEVEL = LogLevel.INFO;

    public static LinkDecoder ENCODER = new LinkDecoder() {
        @Override
        public String unpack(String packedLine) {
            return LinkManager.unpack(packedLine);
        }
    };

    /**
     * Threading of the whole input/output does not look healthy at all!
     *
     * @see #COMMUNICATION_EXECUTOR
     */
    public final static Executor TCP_READ_EXECUTOR = Executors.newSingleThreadExecutor(new ThreadFactory() {
        @Override
        public Thread newThread(@NotNull Runnable r) {
            Thread t = new Thread(r);
            t.setName("IO executor thread");
            t.setDaemon(true);  // need daemon thread so that COM thread is also daemon
            return t;
        }
    });
    public static final String LOG_VIEWER = "log viewer";
    public static final LinkedBlockingQueue<Runnable> COMMUNICATION_QUEUE = new LinkedBlockingQueue<>();
    /**
     * All request/responses to underlying controller are happening on this single-threaded executor in a FIFO manner
     * @see #TCP_READ_EXECUTOR
     */
    public static final ExecutorService COMMUNICATION_EXECUTOR = new ThreadPoolExecutor(1, 1,
            0L, TimeUnit.MILLISECONDS,
            COMMUNICATION_QUEUE,
            new NamedThreadFactory("communication executor"));
    private static Thread COMMUNICATION_THREAD;

    static {
/*
        Future future = submit(new Runnable() {
            @Override
            public void run() {
            // WAT? this is hanging?!
                COMMUNICATION_THREAD = Thread.currentThread();
                System.out.println("Done");
            }
        });
        try {
            // let's wait for the above trivial task to finish
            future.get();
            System.out.println("Done2");
        } catch (InterruptedException | ExecutionException e) {
            throw new IllegalStateException(e);
        }
 */
    }

    public static void assertCommunicationThread() {
//        if (Thread.currentThread() != COMMUNICATION_THREAD)
//            throw new IllegalStateException("Communication on wrong thread");
    }

    public static EngineState engineState = new EngineState(new EngineState.EngineStateListenerImpl() {
        @Override
        public void beforeLine(String fullLine) {
            FileLog.MAIN.logLine(fullLine);
            ConnectionWatchdog.onDataArrived();
        }
    });

    public static LinkConnector connector;

    /**
     * This flag controls if mock controls are needed
     */
    public static boolean isSimulationMode;

    public static void startAndConnect(String port, ConnectionStateListener stateListener) {
        FileLog.MAIN.logLine("LinkManager: Starting " + port);
        start(port);
        connect(stateListener);
    }

    public static void start(String port) {
        if (isLogViewerMode(port)) {
            connector = LinkConnector.VOID;
        } else if (TcpConnector.isTcpPort(port)) {
            connector = new TcpConnector(port);
            isSimulationMode = true;
        } else {
            connector = new SerialConnector(port);
        }
    }

    public static boolean isLogViewerMode(String port) {
        return port.equals(LOG_VIEWER);
    }

    public static boolean isLogViewer() {
        return connector == LinkConnector.VOID;
    }

    public static void connect(ConnectionStateListener listener) {
        if (connector == null)
            throw new NullPointerException("connector");
        connector.connectAndReadConfiguration(listener);
    }

    public static void send(String command, boolean fireEvent) throws InterruptedException {
        if (connector == null)
            throw new NullPointerException("connector");
        connector.send(command, fireEvent);
    }

    public static void restart() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        connector.restart();
    }

    public static String unpack(String packet) {
        return connector.unpack(packet);
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
        String[] ports = SerialPortList.getPortNames();
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
}
