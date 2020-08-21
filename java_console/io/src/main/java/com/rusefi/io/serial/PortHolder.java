package com.rusefi.io.serial;

import com.devexperts.logging.Logging;
import com.rusefi.Callable;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStateListener;
import com.opensr5.io.DataListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;

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

    public ConnectionStateListener listener;
    private final Object portLock = new Object();

    @Nullable
    private BinaryProtocol bp;

    protected PortHolder(LinkManager linkManager, Callable<IoStream> ioStreamFactory) {
        this.linkManager = linkManager;
        dataListener = freshData -> linkManager.getEngineState().processNewData(new String(freshData), LinkManager.ENCODER);
        this.ioStreamFactory = ioStreamFactory;
    }

    boolean connectAndReadConfiguration() {
        IoStream stream = ioStreamFactory.call();
        if (stream == null) {
            // error already reported
            return false;
        }
        synchronized (portLock) {
            bp = new BinaryProtocol(linkManager, stream, stream.getDataBuffer());
            portLock.notifyAll();
        }

        boolean result = bp.connectAndReadConfiguration(dataListener);
        if (listener != null) {
            if (result) {
                listener.onConnectionEstablished();
            } else {
                listener.onConnectionFailed();
            }
        }
        return result;
    }

    public void close() {
        synchronized (portLock) {
            if (bp != null) {
                try {
                    bp.close();
                    bp = null;
                } finally {
                    portLock.notifyAll();
                }
            }
        }
    }

    @Nullable
    public BinaryProtocol getBp() {
        return bp;
    }

    /**
     * this method blocks till a connection is available
     */
    public void packAndSend(final String command, boolean fireEvent) throws InterruptedException {
        if (bp == null) {
            Window[] windows = JDialog.getWindows();
            Window window = windows.length == 0 ? null : windows[0];
            JOptionPane.showMessageDialog(window, "No connectivity",
                    "Error", JOptionPane.ERROR_MESSAGE);
            System.exit(-1);
        }

        bp.doSend(command, fireEvent);
    }
}
