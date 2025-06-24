package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.NamedThreadFactory;
import org.jetbrains.annotations.NotNull;

import java.util.concurrent.atomic.AtomicReference;

import static com.devexperts.logging.Logging.getLogging;

class CommunicationThreadFactory extends NamedThreadFactory {
    private static final Logging log = getLogging(CommunicationThreadFactory.class);

    private final AtomicReference<Thread> communicationThread = new AtomicReference<>();

    CommunicationThreadFactory() {
        super("ECU Communication Executor", true);
    }

    public boolean isInCommunicationThread() {
        return communicationThread.get() == Thread.currentThread();
    }

    @Override
    public Thread newThread(@NotNull Runnable r) {
        final Thread newCommunicationThread = super.newThread(r);
        final Thread prevCommunicationThread = communicationThread.getAndSet(newCommunicationThread);
        if (prevCommunicationThread != null) {
            log.warn("Multiple communication threads.");
        }
        return newCommunicationThread;
    }
};
