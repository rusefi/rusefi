package com.rusefi.maintenance.jobs;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import jakarta.xml.bind.JAXBException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;

public class ImportTuneJob extends AsyncJobWithContext<ImportTuneJobContext> {
    private static final Logging log = getLogging(ImportTuneJob.class);
    private final ConnectivityContext connectivityContext;
    private final @Nullable BinaryProtocol binaryProtocol;
    private final @Nullable LinkManager linkManager;

    /** Port-based constructor: opens a fresh connection to {@code port}. */
    public ImportTuneJob(final PortResult port, final Msq tuneToImport, final ConnectivityContext connectivityContext) {
        super("Load Tune", new ImportTuneJobContext(port, tuneToImport));
        this.connectivityContext = connectivityContext;
        this.binaryProtocol = null;
        this.linkManager = null;
    }

    /** Live-connection constructor: reuses the established {@link BinaryProtocol} without closing it. */
    public ImportTuneJob(final BinaryProtocol bp, final LinkManager lm, final Msq tuneToImport,
                         final ConnectivityContext connectivityContext) {
        super("Load Tune", new ImportTuneJobContext(null, tuneToImport));
        this.connectivityContext = connectivityContext;
        this.binaryProtocol = bp;
        this.linkManager = lm;
    }

    /**
     * LM-only constructor: BP is fetched lazily in {@link #doJob} once the handshake completes.
     * Use this when the reconnect is still in progress at click time.
     */
    public ImportTuneJob(final LinkManager lm, final Msq tuneToImport, final ConnectivityContext connectivityContext) {
        super("Load Tune", new ImportTuneJobContext(null, tuneToImport));
        this.connectivityContext = connectivityContext;
        this.binaryProtocol = null;
        this.linkManager = lm;
    }

    public static void importTuneIntoDevice(PortResult port, JComponent parent, ConnectivityContext connectivityContext,
                                            String tuneFileName, SingleAsyncJobExecutor singleAsyncJobExecutor) {
        loadAndStart(tuneFileName, parent, msq ->
            singleAsyncJobExecutor.startJob(new ImportTuneJob(port, msq, connectivityContext), parent));
    }

    /** Imports a tune via the live splash connection without closing/reopening the serial port. */
    public static void importTuneIntoDeviceViaLiveConnection(BinaryProtocol bp, LinkManager lm, JComponent parent,
                                                              ConnectivityContext connectivityContext,
                                                              String tuneFileName,
                                                              SingleAsyncJobExecutor singleAsyncJobExecutor) {
        loadAndStart(tuneFileName, parent, msq ->
            singleAsyncJobExecutor.startJob(new ImportTuneJob(bp, lm, msq, connectivityContext), parent));
    }

    /**
     * Imports a tune via a live {@link LinkManager}, waiting for the BP handshake if it is still
     * in progress (e.g. immediately after a post-flash reconnect).
     */
    public static void importTuneIntoDeviceViaLiveConnection(LinkManager lm, JComponent parent,
                                                              ConnectivityContext connectivityContext,
                                                              String tuneFileName,
                                                              SingleAsyncJobExecutor singleAsyncJobExecutor) {
        loadAndStart(tuneFileName, parent, msq ->
            singleAsyncJobExecutor.startJob(new ImportTuneJob(lm, msq, connectivityContext), parent));
    }

    private static void loadAndStart(String tuneFileName, JComponent parent, java.util.function.Consumer<Msq> starter) {
        try {
            starter.accept(Msq.readTune(tuneFileName));
        } catch (JAXBException e) {
            final String errorMsg = String.format("Failed to load tune to import from file %s", tuneFileName);
            log.error(errorMsg, e);
            JOptionPane.showMessageDialog(parent, errorMsg, "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        if (binaryProtocol != null) {
            JobHelper.doJob(() -> {
                if (CalibrationsHelper.importTune(
                    binaryProtocol, linkManager, context.getTuneToImport(), callbacks, connectivityContext)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            }, onJobFinished);
        } else if (linkManager != null) {
            JobHelper.doJob(() -> {
                BinaryProtocol bp = awaitBinaryProtocol(callbacks);
                if (bp == null) {
                    callbacks.logLine("Timed out waiting for connection.");
                    callbacks.error();
                    return;
                }
                if (CalibrationsHelper.importTune(
                    bp, linkManager, context.getTuneToImport(), callbacks, connectivityContext)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            }, onJobFinished);
        } else {
            JobHelper.doJob(() -> {
                if (CalibrationsHelper.importTune(
                    context.getPort().port, context.getTuneToImport(), callbacks, connectivityContext)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            }, onJobFinished);
        }
    }

    private BinaryProtocol awaitBinaryProtocol(UpdateOperationCallbacks callbacks) {
        BinaryProtocol bp = linkManager.getBinaryProtocol();
        if (bp != null) {
            return bp;
        }
        callbacks.logLine("Waiting for connection to stabilize...");
        CountDownLatch latch = new CountDownLatch(1);
        ConnectionStatusLogic.Listener listener = new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (!isConnected || ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                    latch.countDown();
                }
            }
        };
        ConnectionStatusLogic.INSTANCE.addListener(listener);
        ConnectionStatusValue currentValue = ConnectionStatusLogic.INSTANCE.getValue();
        if (currentValue == ConnectionStatusValue.CONNECTED || currentValue == ConnectionStatusValue.NOT_CONNECTED) {
            latch.countDown();
        }
        try {
            latch.await(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            ConnectionStatusLogic.INSTANCE.removeListener(listener);
        }
        return linkManager.getBinaryProtocol();
    }
}
