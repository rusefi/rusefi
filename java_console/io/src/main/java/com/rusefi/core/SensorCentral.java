package com.rusefi.core;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import com.opensr5.ini.LowercaseHashMap;

import java.util.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.TimeUnit;

/**
 * This class keeps track of {@link Sensor} current values and {@link SensorCentral.SensorListener}.
 *
 * <h3>Data flow into SensorCentral</h3>
 *
 * <p>SensorCentral is the singleton sink for all real-time sensor values decoded from the
 * controller's "output channels" frame. The flow per ECU frame is:</p>
 *
 * <ol>
 *   <li><b>Frame arrival.</b> {@link com.rusefi.binaryprotocol.BinaryProtocol} (the only
 *       production producer) reassembles a raw output-channels byte buffer from the link and
 *       calls {@link #grabSensorValues(byte[], IniFileModel, ConfigurationImage)} once per
 *       frame. Test sandboxes (e.g. {@code TableTracerSandbox}, {@code CurveTracerSandbox})
 *       and the proxy server's {@code ControllerConnectionState} feed synthetic frames the
 *       same way.</li>
 *
 *   <li><b>Decode.</b> {@link #grabSensorValues} stores the raw {@code response} bytes for
 *       later retrieval via {@link #getResponse()} and delegates decoding to
 *       {@link ISensorCentral#grabSensorValues} (default implementation in
 *       {@link ISensorHolder#grabSensorValues}). That default runs three passes over the
 *       {@link IniFileModel}:
 *       <ul>
 *         <li>Pass 1 — direct output channels: every channel in
 *             {@link IniFileModel#getAllOutputChannels()} is read from the bytes and pushed
 *             via {@link #setValue(double, String)}.</li>
 *         <li>Pass 2 — expression gauges: gauge channels that are runtime expressions
 *             (e.g. {@code { coolant * 1.8 + 32 }}) are evaluated against the just-decoded
 *             values plus optional config image and also pushed via {@link #setValue}.</li>
 *         <li>Pass 3 — string-valued gauge labels (bitStringValue / stringValue) are resolved
 *             and delivered through {@link #onGaugeLabelsResolved(Map)}, which caches them
 *             into {@link #resolvedGaugeLabels} for {@link #getResolvedLabels(String)}.</li>
 *       </ul>
 *   </li>
 *
 *   <li><b>Storage.</b> Each {@link #setValue(double, String)} call writes through to
 *       {@link SensorsHolder} (case-insensitive, lock-free per-channel {@code AtomicDouble})
 *       and reports whether the value actually changed.</li>
 *
 *   <li><b>Per-sensor fan-out.</b> When a value changed, {@link #setValue} synchronously
 *       notifies every registered {@link SensorListener} for that channel name (registered
 *       via {@link #addListener(String, SensorListener)}). Listener lists are keyed by the
 *       lower-cased sensor name.</li>
 *
 *   <li><b>Per-frame fan-out.</b> After the entire frame is decoded, {@link #grabSensorValues}
 *       notifies every {@link ResponseListener} registered via
 *       {@link #addListener(ResponseListener)} exactly once. These listeners typically refresh
 *       UI panels that need a "frame complete" signal rather than per-channel deltas.</li>
 * </ol>
 *
 * <p>Outside the per-frame pipeline, {@link #setValue} can also be called directly (e.g. from
 * tests or simulated sensors); the same fan-out rules apply.</p>
 *
 * <p>Date: 1/6/13<br/>
 * Andrey Belomutskiy, (c) 2013-2020</p>
 */
public class SensorCentral implements ISensorCentral {
    private static final SensorCentral INSTANCE = new SensorCentral();

    private final SensorsHolder sensorsHolder = new SensorsHolder();
    // Reused every grabSensorValues call to avoid allocating a fresh map each ECU frame.
    // LowercaseHashMap gives case-insensitive semantics with O(1) lookup vs TreeMap's O(log N).
    private final Map<String, Double> outputChannelCache = new LowercaseHashMap<>();

    // Keys normalized to lower-case (Locale.US) for O(1) HashMap lookup.
    // "coolant", "COOLANT", "Coolant" all resolve to the same listener list.
    private final Map<String, List<SensorListenerHolder>> sensorListeners = new HashMap<>();
    private final List<ResponseListenerHolder> listeners = new CopyOnWriteArrayList<>();
    private final List<SnapshotListener> snapshotListeners = new CopyOnWriteArrayList<>();
    private final AtomicInteger fullOutputLeases = new AtomicInteger();
    private final AtomicLong outputDemandGeneration = new AtomicLong();
    private final Object snapshotMonitor = new Object();

    private static class SensorListenerHolder {
        private final String sensorName;
        private final SensorListener listener;
        private final boolean contributesDemand;
        private volatile boolean active = true;

        private SensorListenerHolder(String sensorName, SensorListener listener, boolean contributesDemand) {
            this.sensorName = sensorName;
            this.listener = listener;
            this.contributesDemand = contributesDemand;
        }
    }

    private static class ResponseListenerHolder {
        private final ResponseListener listener;
        private volatile SensorSubscription subscription;
        private volatile boolean active = true;

        public ResponseListenerHolder(ResponseListener listener, SensorSubscription subscription) {
            this.listener = listener;
            this.subscription = subscription;
        }
    }
    private volatile Map<String, ResolvedGaugeLabels> resolvedGaugeLabels = Collections.emptyMap();
    private volatile byte[] response;
    private volatile OutputChannelSnapshot currentSnapshot;

    // Sliding window of recent frame arrival timestamps (System.nanoTime), used to compute
    // the synthetic 'runtimeDataRateGauge' value (frames-per-second) once per ECU frame.
    private static final int RATE_WINDOW_SIZE = 16;
    private static final long RATE_WINDOW_NANOS = 2_000_000_000L; // 2 seconds
    private final java.util.ArrayDeque<Long> frameTimestampsNanos = new java.util.ArrayDeque<>(RATE_WINDOW_SIZE + 1);

    public static SensorCentral getInstance() {
        return INSTANCE;
    }

    private SensorCentral() {
    }

    @Override
    public Map<String, Double> getOutputChannelMap() {
        return outputChannelCache;
    }

    @Override
    public void grabSensorValues(byte[] response, @NotNull IniFileModel ini, @Nullable ConfigurationImage configImage) {
        grabSensorValues(OutputChannelSnapshot.full(response), ini, configImage);
        // Preserve the legacy full-frame API's identity contract for existing direct callers.
        this.response = response;
    }

    public void grabSensorValues(OutputChannelSnapshot snapshot, @NotNull IniFileModel ini,
                                 @Nullable ConfigurationImage configImage) {
        synchronized (snapshotMonitor) {
            currentSnapshot = snapshot;
            snapshotMonitor.notifyAll();
        }
        response = snapshot.isFull() ? snapshot.getResponse() : null;
        ISensorCentral.super.grabSensorValues(snapshot, ini, configImage);
        updateRuntimeDataRate();
        for (ResponseListenerHolder holder : listeners) {
            if (holder.active && (snapshot.isFull()
                    || holder.subscription == null
                    || holder.subscription.isInterestedInAny(snapshot.getRequestedChannels()))) {
                holder.listener.onSensorUpdate();
            }
        }
        for (SnapshotListener listener : snapshotListeners) {
            listener.onSnapshot(snapshot);
        }
    }

    /**
     * Computes the runtime data rate (Hz) over a sliding window of recent frame timestamps
     * and publishes it to the synthetic {@code runtimeDataRateGauge} channel injected by
     * {@link com.opensr5.ini.ImmutableIniFileModel}.
     */
    private void updateRuntimeDataRate() {
        long now = System.nanoTime();
        synchronized (frameTimestampsNanos) {
            frameTimestampsNanos.addLast(now);
            // Bound the window by both count and time so an idle period drops the rate to ~0.
            while (frameTimestampsNanos.size() > RATE_WINDOW_SIZE
                    || (!frameTimestampsNanos.isEmpty() && now - frameTimestampsNanos.peekFirst() > RATE_WINDOW_NANOS)) {
                frameTimestampsNanos.pollFirst();
            }
            int n = frameTimestampsNanos.size();
            double hz = 0.0;
            if (n >= 2) {
                long spanNanos = now - frameTimestampsNanos.peekFirst();
                if (spanNanos > 0) {
                    // (n - 1) intervals over spanNanos
                    hz = (n - 1) * 1_000_000_000.0 / spanNanos;
                }
            }
            setValue(hz, com.opensr5.ini.ImmutableIniFileModel.RUNTIME_DATA_RATE_GAUGE);
        }
    }

    @Override
    public void onGaugeLabelsResolved(Map<String, ResolvedGaugeLabels> labels) {
        Map<String, ResolvedGaugeLabels> normalizedLabels = new LowercaseHashMap<>();
        normalizedLabels.putAll(labels);
        this.resolvedGaugeLabels = normalizedLabels;
    }

    /**
     * Get the resolved gauge labels from the most recent update cycle.
     * @return map of gauge name to resolved title/units
     */
    public Map<String, ResolvedGaugeLabels> getResolvedGaugeLabels() {
        return resolvedGaugeLabels;
    }

    /**
     * Get resolved labels for a specific gauge.
     * @return resolved labels, or null if no string functions were resolved for this gauge
     */
    @Nullable
    public ResolvedGaugeLabels getResolvedLabels(String gaugeName) {
        return resolvedGaugeLabels.get(gaugeName.toLowerCase(Locale.US));
    }

    public byte[] getResponse() {
        return response;
    }

    @Override
    public double getValue(Sensor sensor) {
        return getValue(sensor.getNativeName());
    }

    @Override
    public double getValue(String sensorName) {
        return sensorsHolder.getValue(sensorName);
    }

    @Override
    public boolean setValue(double value, String sensorName) {
        boolean isUpdated = sensorsHolder.setValue(value, sensorName);
        if (!isUpdated)
            return false;
        List<SensorListenerHolder> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName.toLowerCase(Locale.US));
        }

        if (listeners == null)
            return true;
        for (SensorListenerHolder holder : listeners) {
            if (holder.active) {
                holder.listener.onSensorUpdate(value);
            }
        }
        return true;
    }

    public ResponseListenerToken addListener(ResponseListener listener) {
        return addListener(listener, new SensorSubscription());
    }

    public ResponseListenerToken addListener(ResponseListener listener, SensorSubscription subscription) {
        ResponseListenerHolder holder = new ResponseListenerHolder(listener, subscription);
        listeners.add(holder);
        outputDemandGeneration.incrementAndGet();
        return new ResponseListenerToken(this, holder);
    }

    public SensorSubscription getSubscription(ResponseListener listener) {
        for (ResponseListenerHolder holder : listeners) {
            if (holder.listener == listener) {
                return holder.subscription;
            }
        }
        return null;
    }

    public void removeListener(ResponseListener listener) {
        boolean removed = listeners.removeIf(holder -> holder.listener == listener);
        if (removed) {
            outputDemandGeneration.incrementAndGet();
        }
    }

    @Override
    public ListenerToken addListener(String sensorName, SensorListener listener) {
        return addListener(sensorName, listener, true);
    }

    public ListenerToken addPassiveListener(String sensorName, SensorListener listener) {
        return addListener(sensorName, listener, false);
    }

    private ListenerToken addListener(String sensorName, SensorListener listener, boolean contributesDemand) {
        String key = sensorName.toLowerCase(Locale.US);
        SensorListenerHolder holder = new SensorListenerHolder(key, listener, contributesDemand);
        List<SensorListenerHolder> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(key);
            if (listeners == null)
                listeners = new CopyOnWriteArrayList<>();
            sensorListeners.put(key, listeners);
        }
        listeners.add(holder);
        if (contributesDemand) {
            outputDemandGeneration.incrementAndGet();
        }

        return new SensorCentral.ListenerToken(
            () -> removeListener(holder),
            active -> setListenerActive(holder, active));
    }

    @Override
    public void removeListener(String sensorName, SensorListener listener) {
        List<SensorListenerHolder> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName.toLowerCase(Locale.US));
        }
        if (listeners != null) {
            for (SensorListenerHolder holder : listeners) {
                if (holder.listener == listener) {
                    removeListener(holder);
                    return;
                }
            }
        }
    }

    private void removeListener(SensorListenerHolder holder) {
        synchronized (sensorListeners) {
            List<SensorListenerHolder> holders = sensorListeners.get(holder.sensorName);
            if (holders != null && holders.remove(holder)) {
                if (holders.isEmpty()) {
                    sensorListeners.remove(holder.sensorName);
                }
                if (holder.contributesDemand) {
                    outputDemandGeneration.incrementAndGet();
                }
            }
        }
    }

    private void setListenerActive(SensorListenerHolder holder, boolean active) {
        if (holder.active != active) {
            holder.active = active;
            if (holder.contributesDemand) {
                outputDemandGeneration.incrementAndGet();
            }
        }
    }

    public OutputChannelDemand getOutputChannelDemand() {
        Set<String> channels = new LinkedHashSet<>();
        synchronized (sensorListeners) {
            for (List<SensorListenerHolder> holders : sensorListeners.values()) {
                for (SensorListenerHolder holder : holders) {
                    if (holder.active && holder.contributesDemand) {
                        channels.add(holder.sensorName);
                        break;
                    }
                }
            }
        }

        boolean full = fullOutputLeases.get() > 0;
        for (ResponseListenerHolder holder : listeners) {
            if (!holder.active) {
                continue;
            }
            SensorSubscription subscription = holder.subscription;
            if (subscription == null || subscription.getSensorNames().isEmpty()) {
                full = true;
            } else {
                channels.addAll(subscription.getSensorNames());
            }
        }
        return new OutputChannelDemand(channels, full, outputDemandGeneration.get());
    }

    public FullOutputLease acquireFullOutput() {
        fullOutputLeases.incrementAndGet();
        long generation = outputDemandGeneration.incrementAndGet();
        return new FullOutputLease(this, generation);
    }

    private void releaseFullOutput() {
        int remaining = fullOutputLeases.decrementAndGet();
        if (remaining < 0) {
            fullOutputLeases.incrementAndGet();
            throw new IllegalStateException("Unbalanced full-output lease release");
        }
        outputDemandGeneration.incrementAndGet();
    }

    public SnapshotListenerToken addSnapshotListener(SnapshotListener listener) {
        snapshotListeners.add(listener);
        return new SnapshotListenerToken(snapshotListeners, listener);
    }

    public OutputChannelSnapshot getCurrentSnapshot() {
        return currentSnapshot;
    }

    @Nullable
    public OutputChannelSnapshot awaitFullSnapshot(long generation, long timeoutMillis) throws InterruptedException {
        long remainingNanos = TimeUnit.MILLISECONDS.toNanos(timeoutMillis);
        long deadline = System.nanoTime() + remainingNanos;
        synchronized (snapshotMonitor) {
            while (currentSnapshot == null || !currentSnapshot.isFull()
                    || currentSnapshot.getGeneration() < generation) {
                if (remainingNanos <= 0) {
                    return null;
                }
                TimeUnit.NANOSECONDS.timedWait(snapshotMonitor, remainingNanos);
                remainingNanos = deadline - System.nanoTime();
            }
            return currentSnapshot;
        }
    }

    /**
     * Clears all accumulated sensor state. Called on ECU disconnect so stale values from the
     * old board do not linger into the next connection (especially critical when ECU A and B
     * have different output-channel layouts / firmware signatures).
     * <p>
     * Thread-safety: this method may race with the pull thread's {@link #grabSensorValues} call.
     * {@code sensorsHolder} uses its own internal lock so {@code reset()} and
     * {@code getValue}/{@code setValue} are mutually exclusive.
     * {@code response} and {@code resolvedGaugeLabels} are {@code volatile}, so their
     * null/empty assignments are immediately visible to any concurrent reader.
     * {@code frameTimestampsNanos} is guarded by its own {@code synchronized} block here and
     * in {@link #updateRuntimeDataRate}.
     */
    public void reset() {
        sensorsHolder.reset();
        response = null;
        synchronized (snapshotMonitor) {
            currentSnapshot = null;
            snapshotMonitor.notifyAll();
        }
        resolvedGaugeLabels = Collections.emptyMap();
        synchronized (frameTimestampsNanos) {
            frameTimestampsNanos.clear();
        }
    }

    @Override
    public ValueSource getValueSource(Sensor sensor) {
        return getValueSource(sensor.name());
    }

    @Override
    public ValueSource getValueSource(String sensorName) {
        return () -> SensorCentral.this.getValue(sensorName);
    }

    public interface SensorListener {
        void onSensorUpdate(double value);
    }

    public interface ResponseListener {
        void onSensorUpdate();
    }

    public interface SnapshotListener {
        void onSnapshot(OutputChannelSnapshot snapshot);
    }

    public static final class ResponseListenerToken {
        private final SensorCentral central;
        private final ResponseListenerHolder holder;
        private final AtomicBoolean removed = new AtomicBoolean();

        private ResponseListenerToken(SensorCentral central, ResponseListenerHolder holder) {
            this.central = central;
            this.holder = holder;
        }

        public void setActive(boolean active) {
            if (!removed.get() && holder.active != active) {
                holder.active = active;
                central.outputDemandGeneration.incrementAndGet();
            }
        }

        public void setSubscription(SensorSubscription subscription) {
            if (!removed.get()) {
                holder.subscription = subscription;
                central.outputDemandGeneration.incrementAndGet();
            }
        }

        public void remove() {
            if (removed.compareAndSet(false, true) && central.listeners.remove(holder)) {
                central.outputDemandGeneration.incrementAndGet();
            }
        }
    }

    public static final class SnapshotListenerToken {
        private final List<SnapshotListener> listeners;
        private final SnapshotListener listener;
        private final AtomicBoolean removed = new AtomicBoolean();

        private SnapshotListenerToken(List<SnapshotListener> listeners, SnapshotListener listener) {
            this.listeners = listeners;
            this.listener = listener;
        }

        public void remove() {
            if (removed.compareAndSet(false, true)) {
                listeners.remove(listener);
            }
        }
    }

    public static final class FullOutputLease implements AutoCloseable {
        private final SensorCentral central;
        private final long generation;
        private final AtomicBoolean closed = new AtomicBoolean();

        private FullOutputLease(SensorCentral central, long generation) {
            this.central = central;
            this.generation = generation;
        }

        public long getGeneration() {
            return generation;
        }

        @Override
        public void close() {
            if (closed.compareAndSet(false, true)) {
                central.releaseFullOutput();
            }
        }
    }
}
