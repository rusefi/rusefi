package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.NamedThreadFactory;
import org.jetbrains.annotations.NotNull;

import static com.devexperts.logging.Logging.getLogging;

class CommunicationThreadFactory extends NamedThreadFactory {
    private static final Logging log = getLogging(CommunicationThreadFactory.class);

    private Thread communicationThread = null;

    CommunicationThreadFactory() {
        super("ECU Communication Executor", true);
    }

    public void assertCommunicationThread() {
        if (Thread.currentThread() != communicationThread) {
            IllegalStateException e = new IllegalStateException("Communication on wrong thread. Use linkManager.execute or linkManager.submit");
            e.printStackTrace();
            log.error(e.getMessage(), e);
            throw e;
        }
    }

    @Override
    public synchronized Thread newThread(@NotNull Runnable r) {
        if (communicationThread != null) {
            log.warn("We're creating second communication thread");
        }
        communicationThread = super.newThread(r);
        return communicationThread;
    }
};
