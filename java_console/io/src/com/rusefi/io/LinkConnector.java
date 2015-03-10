package com.rusefi.io;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public interface LinkConnector {
    void connect(LinkManager.LinkStateListener listener);

    void send(String command) throws InterruptedException;

    void restart();

    boolean hasError();

    String unpack(String packet);

    String unpackConfirmation(String message);
}
