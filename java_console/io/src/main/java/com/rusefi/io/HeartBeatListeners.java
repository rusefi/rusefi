package com.rusefi.io;

import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Singleton which allows listeners to be notified of controller data arrival.
 */
public enum HeartBeatListeners {
    INSTANCE;

    private final CopyOnWriteArrayList<Listener> listeners = new CopyOnWriteArrayList<>();

    public synchronized void addListener(Listener listener) {
        listeners.add(listener);
        if (listeners.size() > 16)
            throw new IllegalStateException("Very unexpected " + listeners);
    }

    public static void onDataArrived() {
        for (Listener listener : INSTANCE.listeners)
            listener.onDataArrival();
    }

    public interface Listener {
        void onDataArrival();
    }
}
