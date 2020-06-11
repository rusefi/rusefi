package com.rusefi.io;

import com.rusefi.binaryprotocol.BinaryProtocol;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public interface LinkConnector extends LinkDecoder {
    LinkConnector VOID = new LinkConnector() {
        @Override
        public void connectAndReadConfiguration(ConnectionStateListener listener) {
        }

        @Override
        public void send(String command, boolean fireEvent) {
        }

        @Override
        public void restart() {
        }

        @Override
        public BinaryProtocol getBinaryProtocol() {
            return null;
        }
    };

    void connectAndReadConfiguration(ConnectionStateListener listener);

    void send(String command, boolean fireEvent) throws InterruptedException;

    void restart();

    BinaryProtocol getBinaryProtocol();
}
