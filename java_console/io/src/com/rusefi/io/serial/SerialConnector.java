package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.core.EngineState;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class SerialConnector implements LinkConnector {
    public SerialConnector(String serialPort) {
        SerialManager.port = serialPort;
    }

    @Override
    public void connect(LinkManager.LinkStateListener listener) {
        FileLog.MAIN.logLine("SerialConnector: connecting");
        SerialManager.scheduleOpening(listener);
    }

    @Override
    public void restart() {
        SerialManager.restart();
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
    public void send(String text) throws InterruptedException {
        PortHolder.getInstance().packAndSend(text);
    }

    @Override
    public String unpackConfirmation(String message) {
        return message.substring(CommandQueue.CONFIRMATION_PREFIX.length());
    }
}
