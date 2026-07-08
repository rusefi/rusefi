package com.rusefi;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Hand-written {@link PortScanner} fake: tests script hardware changes via
 * {@link #fireHardwareChange} and assert on the recorded suspend/resume/cache calls.
 * See docs/java-connectivity-context-review.md.
 */
public class FakePortScanner implements PortScanner {
    private volatile AvailableHardware currentHardware = new AvailableHardware(Collections.emptyList(), false, false, false);
    private final List<Listener> listeners = new CopyOnWriteArrayList<>();

    public final List<PortResult> cachedPorts = Collections.synchronizedList(new ArrayList<>());
    public final List<String> invalidatedPorts = Collections.synchronizedList(new ArrayList<>());
    public int suspendCount;
    public int resumeCount;
    public int restartTimerCount;

    @Override
    public AvailableHardware getCurrentHardware() {
        return currentHardware;
    }

    @Override
    public void addListener(Listener listener) {
        listeners.add(listener);
    }

    /**
     * Simulate a scan cycle detecting a changed hardware list: update the snapshot and notify
     * all listeners, like the production scanner's scan thread does.
     */
    public void fireHardwareChange(AvailableHardware hardware) {
        this.currentHardware = hardware;
        for (Listener listener : listeners) {
            listener.onChange(hardware);
        }
    }

    @Override
    public CountDownLatch suspend() {
        suspendCount++;
        return new CountDownLatch(0);
    }

    @Override
    public void resume() {
        resumeCount++;
    }

    @Override
    public void restartTimer() {
        restartTimerCount++;
    }

    @Override
    public void cachePort(PortResult port) {
        cachedPorts.add(port);
    }

    @Override
    public void invalidatePort(String portName) {
        invalidatedPorts.add(portName);
    }
}
