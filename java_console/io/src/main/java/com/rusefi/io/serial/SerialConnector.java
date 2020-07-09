package com.rusefi.io.serial;

import com.opensr5.Logger;
import com.rusefi.Callable;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;

/**
 * @author Andrey Belomutskiy
 *         3/3/14
 */
public class SerialConnector implements LinkConnector {

    private final PortHolder portHolder;
    private final Logger logger;
    private final LinkManager linkManager;

    public SerialConnector(LinkManager linkManager, String portName, Logger logger, Callable<IoStream> streamFactory) {
        this.linkManager = linkManager;
        this.logger = logger;

        Callable<IoStream> ioStreamCallable = new Callable<IoStream>() {
            @Override
            public IoStream call() {
                IoStream stream = streamFactory.call();
                if (stream == null) {
                    // error already reported
                    return null;
                }
                IncomingDataBuffer dataBuffer = IncomingDataBuffer.createDataBuffer(stream, logger);
                stream.setDataBuffer(dataBuffer);
                return stream;
            }
        };

        portHolder = new PortHolder(portName, linkManager, logger, ioStreamCallable);
    }

    @Override
    public void connectAndReadConfiguration(ConnectionStateListener listener) {
        logger.info("SerialConnector: connecting");
        portHolder.listener = listener;
        logger.info("scheduleOpening");
        linkManager.execute(new Runnable() {
            @Override
            public void run() {
                logger.info("scheduleOpening>openPort");
                portHolder.connectAndReadConfiguration();
            }
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
    public void restart() {
        linkManager.execute(new Runnable() {
            @Override
            public void run() {
                MessagesCentral.getInstance().postMessage(logger, SerialConnector.this.getClass(), "Restarting serial IO");
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
