package com.rusefi.io.serial;

import com.devexperts.logging.Logging;
import com.rusefi.Callable;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.Nullable;
import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;

/**
 * This class holds the reference to the actual Serial port object
 * <p/>
 * 7/25/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class PortHolder {
    private static final Logging log = getLogging(PortHolder.class);

    private final DataListener dataListener;
    private final Callable<IoStream> ioStreamFactory;
    private final LinkManager linkManager;

    private final Object portLock = new Object();
    private long generation;

    @Nullable
    private volatile BinaryProtocol bp;

    protected PortHolder(LinkManager linkManager, Callable<IoStream> ioStreamFactory) {
        this.linkManager = linkManager;
        dataListener = freshData -> linkManager.getEngineState().processNewData(new String(freshData), LinkManager.ENCODER);
        this.ioStreamFactory = ioStreamFactory;
    }

    /** Capture cancellation before the open is queued on the communication executor. */
    Runnable prepareConnection(BinaryProtocol.Arguments arguments, ConnectionStatusLogic.Listener listener) {
        Objects.requireNonNull(arguments);
        synchronized (portLock) {
            close();
            long attemptGeneration = generation;
            return () -> connectAndReadConfiguration(arguments, listener, attemptGeneration);
        }
    }

    private void connectAndReadConfiguration(BinaryProtocol.Arguments arguments,
                                             ConnectionStatusLogic.Listener listener, long attemptGeneration) {
        synchronized (portLock) {
            if (attemptGeneration != generation) {
                return;
            }
        }
        // Opening and configuration I/O must not block cancellation on portLock.
        IoStream stream;
        try {
            stream = ioStreamFactory.call();
        } catch (RuntimeException e) {
            log.error("Exception opening port: " + e.getMessage());
            completeConnection(attemptGeneration, listener, "Exception opening port: " + e.getMessage());
            return;
        }
        if (stream == null) {
            completeConnection(attemptGeneration, listener, "Failed to open port");
            return;
        }
        BinaryProtocol protocol;
        synchronized (portLock) {
            if (attemptGeneration != generation) {
                stream.close();
                return;
            }
            protocol = new BinaryProtocol(linkManager, stream);
            bp = protocol;
        }

        String errorMessage = protocol.connectAndReadConfiguration(arguments, dataListener);
        completeConnection(attemptGeneration, listener, errorMessage);
    }

    private void completeConnection(long attemptGeneration, ConnectionStatusLogic.Listener listener,
                                    String errorMessage) {
        synchronized (portLock) {
            if (attemptGeneration != generation) {
                return;
            }
            if (errorMessage == null && bp != null && bp.isClosed()) {
                errorMessage = "Connection closed during startup";
            }
            if (errorMessage != null && bp != null) {
                BinaryProtocol protocol = bp;
                bp = null;
                protocol.close();
            }
            // Serialize notification with close, including cancellation during cleanup callbacks.
            if (attemptGeneration == generation && listener != null) {
                if (errorMessage == null) {
                    listener.onConnectionEstablished();
                } else {
                    listener.onConnectionFailed(errorMessage);
                }
            }
        }
    }

    public void close() {
        synchronized (portLock) {
            generation++;
            BinaryProtocol protocol = bp;
            bp = null;
            if (protocol != null) {
                protocol.close();
            }
        }
    }

    @Nullable
    public BinaryProtocol getBp() {
        return bp;
    }

    public void packAndSend(final String command, boolean fireEvent) throws InterruptedException {
        BinaryProtocol protocol = bp;
        if (protocol == null) {
            log.warn("packAndSend: no active connection, dropping command '" + command + "'");
            throw new InterruptedException("No connectivity");
        }

        protocol.doSend(command, fireEvent);
    }
}
