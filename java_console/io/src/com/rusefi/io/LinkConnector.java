package com.rusefi.io;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public interface LinkConnector extends LinkDecoder {
    LinkConnector VOID = new LinkConnector() {
        @Override
        public void connect(ConnectionStateListener listener) {
        }

        @Override
        public void send(String command, boolean fireEvent) throws InterruptedException {
        }

        @Override
        public void restart() {
        }

        @Override
        public boolean hasError() {
            return false;
        }

        @Override
        public String unpack(String packet) {
            return LinkDecoder.TEXT_PROTOCOL_DECODER.unpack(packet);
        }
    };

    void connect(ConnectionStateListener listener);

    void send(String command, boolean fireEvent) throws InterruptedException;

    void restart();

    boolean hasError();
}
