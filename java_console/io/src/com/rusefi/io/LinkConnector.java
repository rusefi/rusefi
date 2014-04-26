package com.rusefi.io;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public interface LinkConnector {
    void connect();

    void send(String command) throws InterruptedException;

    void restart();

    boolean hasError();
}
