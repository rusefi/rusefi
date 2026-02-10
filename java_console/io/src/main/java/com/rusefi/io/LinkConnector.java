package com.rusefi.io;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import org.jetbrains.annotations.NotNull;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public interface LinkConnector extends LinkDecoder {
    LinkConnector VOID = new LinkConnector() {
        @Override
        public void connectAndReadConfiguration(BinaryProtocol.Arguments arguments, ConnectionStatusLogic.Listener listener) {
        }

        @Override
        public void send(String command, boolean fireEvent) {
        }

        @Override
        public BinaryProtocol getBinaryProtocol() {
            return null;
        }
    };

    void connectAndReadConfiguration(BinaryProtocol.Arguments arguments, ConnectionStatusLogic.Listener listener);

    void send(String command, boolean fireEvent) throws InterruptedException;

    BinaryProtocol getBinaryProtocol();

    default BinaryProtocolState getBinaryProtocolState() {
        return getBinaryProtocol().getBinaryProtocolState();
    }

    default void stop() {
    }
}
