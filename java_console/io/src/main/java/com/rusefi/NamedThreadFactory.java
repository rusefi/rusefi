package com.rusefi;

import org.jetbrains.annotations.NotNull;

import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicInteger;

public class NamedThreadFactory implements ThreadFactory {
    private final AtomicInteger counter = new AtomicInteger();
    private final String name;
    private final boolean isDaemon;

    public NamedThreadFactory(String name) {
        this(name, false);
    }

    public NamedThreadFactory(String name, boolean isDaemon) {
        this.name = name;
        this.isDaemon = isDaemon;
    }

    @Override
    public Thread newThread(@NotNull Runnable r) {
        Thread t = Executors.defaultThreadFactory().newThread(r);
        t.setName(name + counter.incrementAndGet());
        t.setDaemon(isDaemon);
        return t;
    }
}
