package com.rusefi.io.serial;

import com.devexperts.logging.Logging;
import com.rusefi.Callable;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;

import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class StreamConnector implements LinkConnector {
    private static final Logging log = getLogging(StreamConnector.class);

    private final PortHolder portHolder;
    private final LinkManager linkManager;

    public StreamConnector(LinkManager linkManager, Callable<IoStream> ioStreamCallable) {
        this.linkManager = linkManager;

        portHolder = new PortHolder(linkManager, ioStreamCallable);
    }

    @Override
    public void connectAndReadConfiguration(BinaryProtocol.Arguments arguments, ConnectionStatusLogic.Listener listener) {
        Objects.requireNonNull(arguments);
        log.info("StreamConnector: connecting");
        portHolder.listener = listener;
        log.info("scheduleOpening");
        linkManager.execute(() -> {
            log.info("scheduleOpening>openPort");
            portHolder.connectAndReadConfiguration(arguments);
        });
    }

    @Override
    public BinaryProtocol getBinaryProtocol() {
        return portHolder.getBp();
    }

    @Override
    public void stop() {
        portHolder.close();
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
