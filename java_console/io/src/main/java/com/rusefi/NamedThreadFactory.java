package com.rusefi;

import org.jetbrains.annotations.NotNull;

import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicInteger;

public class NamedThreadFactory implements ThreadFactory {
    private final AtomicInteger counter = new AtomicInteger();
    private String name;

    public NamedThreadFactory(String name) {
        this.name = name;
    }

    @Override
    public Thread newThread(@NotNull Runnable r) {
        Thread t = Executors.defaultThreadFactory().newThread(r);
        t.setName(name + counter.incrementAndGet());
        return t;
    }
}
