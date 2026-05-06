package com.rusefi.core;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import com.opensr5.ini.LowercaseHashMap;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

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
    private final Map<String, List<SensorListener>> sensorListeners = new HashMap<>();
    private final List<ResponseListener> listeners = new CopyOnWriteArrayList<>();
    private volatile Map<String, ResolvedGaugeLabels> resolvedGaugeLabels = Collections.emptyMap();
    private byte[] response;

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
        this.response = response;
        ISensorCentral.super.grabSensorValues(response, ini, configImage);
        updateRuntimeDataRate();
        for (ResponseListener listener : listeners)
            listener.onSensorUpdate();
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
        this.resolvedGaugeLabels = labels;
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
        return resolvedGaugeLabels.get(gaugeName);
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
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName.toLowerCase(Locale.US));
        }

        if (listeners == null)
            return true;
        for (SensorListener listener : listeners)
            listener.onSensorUpdate(value);
        return true;
    }

    public void addListener(ResponseListener listener) {
        listeners.add(listener);
    }

    public void removeListener(ResponseListener listener) {
        listeners.remove(listener);
    }

    @Override
    public ListenerToken addListener(String sensorName, SensorListener listener) {
        String key = sensorName.toLowerCase(Locale.US);
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(key);
            if (listeners == null)
                listeners = new CopyOnWriteArrayList<>();
            sensorListeners.put(key, listeners);
        }
        listeners.add(listener);

        return new SensorCentral.ListenerToken(this, sensorName, listener);
    }

    @Override
    public void removeListener(String sensorName, SensorListener listener) {
        List<SensorListener> listeners;
        synchronized (sensorListeners) {
            listeners = sensorListeners.get(sensorName.toLowerCase(Locale.US));
        }
        if (listeners != null)
            listeners.remove(listener);
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
}
