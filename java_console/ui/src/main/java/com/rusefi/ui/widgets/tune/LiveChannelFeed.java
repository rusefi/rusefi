package com.rusefi.ui.widgets.tune;

import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorSubscription;

import javax.swing.*;
import java.awt.event.HierarchyEvent;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.LongSupplier;

/**
 * Live values of the output channels a table or curve axis follows (issue #10670), for marking the
 * operating point. The channels are polled only while the owner is on screen, so a closed or hidden
 * editor stops asking the ECU for them. Frames are coalesced into one EDT update at a time, and
 * values older than {@link #STALE_MILLIS} are dropped so a lost connection leaves no frozen marker.
 */
final class LiveChannelFeed {
    static final long STALE_MILLIS = 1500;

    private final Runnable onChange;
    private final Timer expiry = new Timer(500, e -> expire());
    private final AtomicBoolean refreshPending = new AtomicBoolean();
    private LongSupplier clock = System::currentTimeMillis;
    private SensorCentral.ResponseListenerToken token;
    private boolean active;
    private String[] channels = new String[0];
    private double[] values = new double[0];
    private long updatedMillis;

    /** @param onChange runs on the EDT whenever the values change or are dropped */
    LiveChannelFeed(JComponent owner, Runnable onChange) {
        this.onChange = onChange;
        owner.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0) {
                setActive(owner.isShowing());
            }
        });
    }

    /** A null channel, or none at all, means there is nothing to follow. */
    void setChannels(String... channels) {
        if (!Arrays.equals(this.channels, channels)) {
            this.channels = channels.clone();
            values = nan(channels.length);
            onChange.run();
        }
        updateSubscription();
    }

    String[] getChannels() {
        return channels.clone();
    }

    void setActive(boolean active) {
        this.active = active;
        updateSubscription();
    }

    /** The latest value of the channel at {@code index}, or NaN when there is none. */
    double value(int index) {
        return index < values.length ? values[index] : Double.NaN;
    }

    void setValues(double... values) {
        this.values = values.clone();
        updatedMillis = clock.getAsLong();
        onChange.run();
    }

    void expire() {
        if (hasValues() && clock.getAsLong() - updatedMillis > STALE_MILLIS) {
            clear();
        }
    }

    /** The feed's clock, so callers can age what they keep from it consistently. */
    long nowMillis() {
        return clock.getAsLong();
    }

    void setClockForUnitTest(LongSupplier clock) {
        this.clock = clock;
    }

    private boolean hasChannels() {
        if (channels.length == 0) {
            return false;
        }
        for (String channel : channels) {
            if (channel == null) {
                return false;
            }
        }
        return true;
    }

    private boolean hasValues() {
        for (double value : values) {
            if (!Double.isNaN(value)) {
                return true;
            }
        }
        return false;
    }

    private void clear() {
        if (hasValues()) {
            values = nan(values.length);
            onChange.run();
        }
    }

    private void updateSubscription() {
        if (!active || !hasChannels()) {
            if (token != null) {
                token.remove();
                token = null;
            }
            expiry.stop();
            clear();
            return;
        }
        SensorSubscription subscription = new SensorSubscription(channels);
        if (token == null) {
            token = SensorCentral.getInstance().addListener(this::onFrame, subscription);
        } else {
            token.setSubscription(subscription);
        }
        expiry.start();
    }

    /** Runs on the connection thread for every output-channel frame. */
    private void onFrame() {
        if (!refreshPending.compareAndSet(false, true)) {
            return;
        }
        SwingUtilities.invokeLater(() -> {
            refreshPending.set(false);
            if (token == null) {
                return;
            }
            double[] latest = new double[channels.length];
            for (int i = 0; i < channels.length; i++) {
                latest[i] = SensorCentral.getInstance().getValue(channels[i]);
            }
            setValues(latest);
        });
    }

    private static double[] nan(int size) {
        double[] values = new double[size];
        Arrays.fill(values, Double.NaN);
        return values;
    }
}
