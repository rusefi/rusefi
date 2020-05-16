package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class SerialConnector implements LinkConnector {

    private final PortHolder portHolder = new PortHolder();

    public SerialConnector(String serialPort) {
        portHolder.port = serialPort;
    }

    @Override
    public void connect(ConnectionStateListener listener) {
        FileLog.MAIN.logLine("SerialConnector: connecting");
        portHolder.listener = listener;
        FileLog.MAIN.logLine("scheduleOpening");
        LinkManager.execute(new Runnable() {
            @Override
            public void run() {
                FileLog.MAIN.logLine("scheduleOpening>openPort");
                portHolder.connectAndReadConfiguration();
            }
        });
    }

    @Override
    public BinaryProtocol getBinaryProtocol() {
        return portHolder.getBp();
    }

    @Override
    public void restart() {
        LinkManager.execute(new Runnable() {
            @Override
            public void run() {
                MessagesCentral.getInstance().postMessage(getClass(), "Restarting serial IO");
                portHolder.close();
                portHolder.connectAndReadConfiguration();
            }
        });
    }

    @Override
    public String unpack(String packet) {
        return packet;
    }

    @Override
    public void send(String text, boolean fireEvent) throws InterruptedException {
        portHolder.packAndSend(text, fireEvent);
    }
}
