package com.rusefi.io;

import com.irnems.FileLog;
import com.irnems.core.EngineState;
import com.rusefi.io.serial.SerialConnector;
import com.rusefi.io.tcp.TcpConnector;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class LinkManager {
    public final static Executor IO_EXECUTOR = Executors.newSingleThreadExecutor(new ThreadFactory() {
        @Override
        public Thread newThread(Runnable r) {
            Thread t = new Thread(r);
            t.setName("IO executor thread");
            t.setDaemon(true);  // need daemon thread so that COM thread is also daemon
            return t;
        }
    });
    public static final String LOG_VIEWER = "log viewer";
    private static final LinkConnector VOID = new LinkConnector() {
        @Override
        public void connect() {
        }

        @Override
        public void send(String command) throws InterruptedException {
        }

        @Override
        public void restart() {
        }

        @Override
        public boolean hasError() {
            return false;
        }
    };
    public static EngineState engineState = new EngineState(new EngineState.EngineStateListenerImpl() {
        @Override
        public void beforeLine(String fullLine) {
            FileLog.MAIN.logLine(fullLine);
        }
    });
    public static boolean onlyUI = false;
    private static LinkConnector connector;

    /**
     * This flag controls if mock controls are needed
     */
    public static boolean isStimulationMode;

    public static void start(String port) {
        if (port.equals(LOG_VIEWER)) {
            connector = LinkManager.VOID;
        } else if (TcpConnector.isTcpPort(port)) {
            connector = new TcpConnector(port);
            isStimulationMode = true;
        } else {
            connector = new SerialConnector(port);
        }
    }

    public static boolean isLogViewer() {
        return connector == LinkManager.VOID;
    }

    public static void open() {
        if (connector == null)
            throw new NullPointerException("connector");
        connector.connect();
    }

    public static void stop() {
//        connector.stop();
    }

    public static void send(String command) throws InterruptedException {
        if (connector == null)
            throw new NullPointerException("connector");
        connector.send(encodeCommand(command));
    }

    public static String encodeCommand(String command) {
        return "sec!" + command.length() + "!" + command;
    }

    public static void restart() {
        connector.restart();
    }

    public static boolean hasError() {
        return connector.hasError();
    }
}
