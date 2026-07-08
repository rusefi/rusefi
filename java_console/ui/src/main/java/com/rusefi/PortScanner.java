package com.rusefi;

import java.util.concurrent.CountDownLatch;

/**
 * The narrow hardware-discovery surface consumers use via {@link ConnectivityContext}: current
 * hardware snapshot, change notifications, and the suspend/resume/cache choreography flash jobs
 * perform to take exclusive ownership of a port. {@link SerialPortScanner} is the production
 * implementation; tests substitute a fake to drive the flashing/session state machines with
 * scripted hardware.
 * <p>
 * See docs/java-connectivity-context-review.md for the dependency-injection/testability plan.
 */
public interface PortScanner {
    interface Listener {
        void onChange(AvailableHardware currentHardware);
    }

    AvailableHardware getCurrentHardware();

    /**
     * Subscribe to hardware-list changes. The production scanner starts its scan thread on the
     * first subscription.
     */
    void addListener(Listener listener);

    /**
     * Suspend scanning and wait for in-flight port probes to release their port handles, so a
     * flash job can take exclusive access to the port. The returned latch counts down when the
     * current scan cycle has fully stopped.
     */
    CountDownLatch suspend();

    void resume();

    void restartTimer();

    /**
     * Pre-populate the port cache with a known result so the scanner does not re-inspect (open)
     * an actively-used port on the next scan cycle.
     */
    void cachePort(PortResult port);

    /**
     * Remove a port from the cache so the scanner re-inspects it on the next scan cycle, e.g.
     * after a reboot-to-bootloader command changes what is on the other end of the port.
     */
    void invalidatePort(String portName);
}
