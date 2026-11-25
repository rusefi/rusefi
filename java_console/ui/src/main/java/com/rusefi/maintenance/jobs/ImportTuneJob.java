package com.rusefi.maintenance.jobs;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import jakarta.xml.bind.JAXBException;
import java.lang.reflect.InvocationTargetException;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

public class ImportTuneJob extends AsyncJobWithContext<ImportTuneJobContext> {
    private static final Logging log = getLogging(ImportTuneJob.class);
    private final ConnectivityContext connectivityContext;
    private final @Nullable BinaryProtocol binaryProtocol;
    private final LinkManager linkManager;

    /** Live-connection constructor: reuses the established {@link BinaryProtocol} without closing it. */
    public ImportTuneJob(final BinaryProtocol bp, final LinkManager lm, final Msq tuneToImport,
                         final ConnectivityContext connectivityContext) {
        super("Load Tune", new ImportTuneJobContext(tuneToImport));
        this.connectivityContext = connectivityContext;
        this.binaryProtocol = bp;
        this.linkManager = lm;
    }

    /**
     * LM-only constructor: BP is fetched lazily in {@link #doJob} once the handshake completes.
     * Use this when the reconnect is still in progress at click time.
     */
    public ImportTuneJob(final LinkManager lm, final Msq tuneToImport, final ConnectivityContext connectivityContext) {
        super("Load Tune", new ImportTuneJobContext(tuneToImport));
        this.connectivityContext = connectivityContext;
        this.binaryProtocol = null;
        this.linkManager = lm;
    }

    /** Imports a tune via the live splash connection without closing/reopening the serial port. */
    public static void importTuneIntoDeviceViaLiveConnection(BinaryProtocol bp, LinkManager lm, JComponent parent,
                                                               ConnectivityContext connectivityContext,
                                                               String tuneFileName,
                                                               SingleAsyncJobExecutor singleAsyncJobExecutor,
                                                               Consumer<String> errorHandler) {
        loadAndStart(tuneFileName, parent, msq ->
            singleAsyncJobExecutor.startJob(
                new ImportTuneJob(bp, lm, msq, connectivityContext), parent, errorHandler), errorHandler);
    }

    /**
     * Imports a tune via a live {@link LinkManager}, waiting for the BP handshake if it is still
     * in progress (e.g. immediately after a post-flash reconnect).
     */
    public static void importTuneIntoDeviceViaLiveConnection(LinkManager lm, JComponent parent,
                                                               ConnectivityContext connectivityContext,
                                                               String tuneFileName,
                                                               SingleAsyncJobExecutor singleAsyncJobExecutor,
                                                               Consumer<String> errorHandler) {
        loadAndStart(tuneFileName, parent, msq ->
            singleAsyncJobExecutor.startJob(
                new ImportTuneJob(lm, msq, connectivityContext), parent, errorHandler), errorHandler);
    }

    private static void loadAndStart(String tuneFileName, JComponent parent,
                                     Consumer<Msq> starter, Consumer<String> errorHandler) {
        try {
            Msq tune = Msq.readTune(tuneFileName);
            runOnEdtAndWait(() -> {
                Object[] options = {"Import and Burn", "Cancel"};
                int choice = JOptionPane.showOptionDialog(
                    parent,
                    "Importing this tune writes settings to the ECU immediately and burns them to ECU flash. Continue?",
                    "Import and Burn",
                    JOptionPane.DEFAULT_OPTION,
                    JOptionPane.WARNING_MESSAGE,
                    null,
                    options,
                    options[1]
                );
                startIfApproved(choice == 0, tune, starter);
            });
        } catch (JAXBException e) {
            final String errorMsg = String.format("Failed to load tune to import from file %s: %s",
                tuneFileName, e.getMessage());
            log.error(errorMsg, e);
            errorHandler.accept(errorMsg);
        }
    }

    static void startIfApproved(boolean approved, Msq tune, Consumer<Msq> starter) {
        if (approved) {
            starter.accept(tune);
        }
    }

    private static void runOnEdtAndWait(Runnable action) {
        if (SwingUtilities.isEventDispatchThread()) {
            action.run();
            return;
        }
        try {
            SwingUtilities.invokeAndWait(action);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } catch (InvocationTargetException e) {
            log.error("Failed to confirm tune import", e.getCause());
        }
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        if (binaryProtocol != null) {
            JobHelper.doJob(() -> {
                if (!isEstablishedConnectionCurrent(binaryProtocol, linkManager)) {
                    callbacks.logLine("The ECU connection changed; start Import and Burn again.");
                    callbacks.error();
                    return;
                }
                if (CalibrationsHelper.importTune(
                    binaryProtocol, linkManager, context.getTuneToImport(), callbacks, connectivityContext)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            }, onJobFinished);
        } else {
            JobHelper.doJob(() -> {
                BinaryProtocol bp = JobHelper.awaitBinaryProtocol(linkManager, callbacks);
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
        }
    }

    static boolean isEstablishedConnectionCurrent(BinaryProtocol bp, LinkManager lm) {
        return lm.getBinaryProtocol() == bp && !bp.isClosed();
    }

}
