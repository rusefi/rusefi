package com.rusefi.io.serial;

import com.rusefi.FileLog;
import com.rusefi.io.LinkConnector;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class SerialConnector implements LinkConnector {
    public SerialConnector(String serialPort) {
        SerialManager.port = serialPort;
    }

    @Override
    public void connect() {
        FileLog.MAIN.logLine("SerialConnector: connecting");
        SerialManager.scheduleOpening();
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
    public void send(String command) throws InterruptedException {
        PortHolder.getInstance().packAndSend(command);
    }
}
