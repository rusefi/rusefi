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
    public static EngineState engineState = new EngineState(new EngineState.EngineStateListenerImpl() {
        @Override
        public void beforeLine(String fullLine) {
            FileLog.rlog("SerialManager.beforeLine: " + fullLine);
            FileLog.MAIN.logLine(fullLine);
        }
    });
    public static boolean onlyUI = false;
    private static LinkConnector connector;

    public static void start(String port) {
        if (TcpConnector.isTcpPort(port)) {
            connector = new TcpConnector(port);
        } else {
            connector = new SerialConnector(port);
        }
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

    private static String encodeCommand(String command) {
        return "sec!" + command.length() + "!" + command;
    }

    public static void restart() {
        connector.restart();
    }
}
