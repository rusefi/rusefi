package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class SerialConnector implements LinkConnector {
    public SerialConnector(String serialPort) {
        SerialManager.port = serialPort;
    }

    @Override
    public void connect(ConnectionStateListener listener) {
        FileLog.MAIN.logLine("SerialConnector: connecting");
        SerialManager.listener = listener;
        FileLog.MAIN.logLine("scheduleOpening");
        LinkManager.COMMUNICATION_EXECUTOR.execute(new Runnable() {
            @Override
            public void run() {
                FileLog.MAIN.logLine("scheduleOpening>openPort");
                PortHolder.getInstance().openPort(SerialManager.port, SerialManager.dataListener, SerialManager.listener);
            }
        });
    }

    @Override
    public void restart() {
        LinkManager.COMMUNICATION_EXECUTOR.execute(new Runnable() {
            @Override
            public void run() {
                MessagesCentral.getInstance().postMessage(SerialManager.class, "Restarting serial IO");
//                if (closed)
//                    return;
                PortHolder.getInstance().close();
                PortHolder.getInstance().openPort(SerialManager.port, SerialManager.dataListener, SerialManager.listener);
            }
        });
    }

    @Override
    public boolean hasError() {
        return false;
    }

    @Override
    public String unpack(String packet) {
        return packet;
    }

    @Override
    public void send(String text, boolean fireEvent) throws InterruptedException {
        PortHolder.getInstance().packAndSend(text, fireEvent);
    }
}
