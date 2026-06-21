package com.rusefi.core;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.LowercaseHashMap;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.ByteBufferUtil.littleEndianWrap;

public interface ISensorHolder {
    Logging log = getLogging(ISensorHolder.class);

    /**
     * Process output channels and evaluate gauge expressions.
     *
     * @param response the output channels response bytes
     * @param ini the INI file model with gauge definitions
     * @param configImage optional configuration image for resolving config values in expressions
     */
    /**
     * Returns a map to accumulate output channel values during {@link #grabSensorValues}.
     * The default creates a fresh map each call; override to return a cached instance
     * (the map will be cleared at the start of each call).
     */
    default Map<String, Double> getOutputChannelMap() {
        return new LowercaseHashMap<>();
    }

    default void grabSensorValues(byte[] response, @NotNull IniFileModel ini, @Nullable ConfigurationImage configImage) {
        // Use case-insensitive map so that gauge channel names like "CLTValue" match
        // output channel keys regardless of capitalisation differences.
        Map<String, Double> outputChannelValues = getOutputChannelMap();
        outputChannelValues.clear();

        // Pass 1: read every direct output channel defined in the ini.
        // This single pass covers what was previously spread across gauge, indicator,
        // gauge-label, and datalog-only channels.
        for (String name : ini.getAllOutputChannels().keySet()) {
            Double value = tryReadOutputChannel(response, name, ini, name);
            if (value != null) {
                setValue(value, name);
                outputChannelValues.put(name, value);
            }
        }

        // Pass 2: evaluate expression-based gauge channels (e.g. "{ coolant * 1.8 + 32 }").
        // These are not in allOutputChannels, so they were not handled by pass 1.
        for (Map.Entry<String, GaugeModel> e : ini.getGauges().entrySet()) {
            String channel = e.getValue().getChannel();
            if (outputChannelValues.containsKey(channel))
                continue;

            String expression = resolveExpression(ini, channel);
            if (expression == null) {
                log.warn("Member not found for gauge " + e.getKey() + ": " + channel);
                continue;
            }

            resolveExpressionVariables(response, ini, configImage, expression, outputChannelValues, outputChannelValues);

            Double result = ExpressionEvaluator.tryEvaluateWithContext(expression, outputChannelValues);
            if (result != null) {
                setValue(result, channel);
                outputChannelValues.put(channel, result);
            } else {
                log.warn("Could not evaluate expression for gauge " + e.getKey() + ": " + expression);
            }
        }

        // Pass 3: resolve string-valued gauge labels (bitStringValue, stringValue).
        onGaugeLabelsResolved(resolveGaugeLabels(ini, configImage, outputChannelValues));
    }

    /**
     * Called after gauge labels have been resolved. Override to store or react to resolved labels.
     */
    default void onGaugeLabelsResolved(Map<String, ResolvedGaugeLabels> labels) {
    }

    /**
     * Try to read a named output channel from the response bytes.
     * @return the value, or null if the channel doesn't exist, has an unsupported type, or is out of bounds
     */
    @Nullable
    static Double tryReadOutputChannel(byte[] response, String label, IniFileModel ini, String channelName) {
        try {
            IniField field = ini.getOutputChannel(channelName);
            return readFieldValue(response, label, field);
        } catch (IniMemberNotFound e) {
            return null;
        } catch (IllegalArgumentException e) {
            log.warn("Out of bounds reading output channel " + channelName + ": " + e.getMessage());
            return null;
        }
    }

    /**
     * Resolve a gauge channel reference to its expression string.
     * The channel may already be an expression like "{ coolant * 1.8 + 32 }",
     * or it may be the name of an expression output channel.
     */
    @Nullable
    static String resolveExpression(IniFileModel ini, String channel) {
        if (ExpressionEvaluator.looksLikeExpression(channel))
            return channel;
        return ini.getExpressionOutputChannel(channel);
    }

    /**
     * Populate the expression context with any missing variables needed for evaluation,
     * reading from output channels first, then falling back to config values.
     */
    default void resolveExpressionVariables(byte[] response, IniFileModel ini,
            @Nullable ConfigurationImage configImage, String expression,
            Map<String, Double> context, Map<String, Double> outputChannelValues) {
        for (String varName : ExpressionEvaluator.extractVariables(expression)) {
            if (context.containsKey(varName))
                continue;

            Double value = tryReadOutputChannel(response, varName, ini, varName);
            if (value != null) {
                context.put(varName, value);
                setValue(value, varName);
                outputChannelValues.put(varName, value);
                continue;
            }

            if (configImage != null) {
                Double configValue = getConfigValue(varName, ini, configImage);
                if (configValue != null) {
                    context.put(varName, configValue);
                }
            }
        }
    }

    /**
     * Try to get a configuration value by field name.
     *
     * @param fieldName the name of the configuration field
     * @param ini the INI file model
     * @param configImage the configuration image with actual values
     * @return the value, or null if not found
     */
    @Nullable
    static Double getConfigValue(String fieldName, IniFileModel ini, ConfigurationImage configImage) {
        Optional<IniField> optField = ini.findIniField(fieldName);
        if (!optField.isPresent()) {
            return null;
        }
        return configImage.readNumericValue(optField.get());
    }

    /**
     * Read the numeric value of an output channel field from the response bytes.
     * Handles both ScalarIniField and EnumIniField (bits).
     *
     * @return the scaled value, or null if the field type is not supported
     */
    @Nullable
    static Double readFieldValue(byte[] response, String label, IniField field) {
        ByteBuffer bb = getByteBuffer(response, label, field.getOffset());
        if (field instanceof ScalarIniField) {
            ScalarIniField scalarField = (ScalarIniField) field;
            double rawValue = scalarField.getType().readRawValue(bb);
            return rawValue * scalarField.getMultiplier();
        } else if (field instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) field;
            int rawValue = (int) enumField.getType().readRawValue(bb);
            int bitCount = enumField.getBitSize0() + 1;
            int mask = (1 << bitCount) - 1;
            return (double) ((rawValue >> enumField.getBitPosition()) & mask);
        }
        return null;
    }

    // Cached ByteBuffer per thread, re-wrapped whenever the response array changes.
    // This avoids allocating a new HeapByteBuffer for every output channel read per ECU frame.
    ThreadLocal<ByteBuffer[]> RESPONSE_BB_HOLDER = ThreadLocal.withInitial(() -> new ByteBuffer[1]);

    static @NotNull ByteBuffer getByteBuffer(byte[] response, String message, int fieldOffset) {
        int offset = fieldOffset + 1; // first byte is response code
        int size = 4;
        if (offset + size > response.length) {
            throw new IllegalArgumentException(message + String.format(" but %d+%d in %d", offset, size, response.length));
        }
        // Re-use a single ByteBuffer per thread per response array; wrap the full array once.
        ByteBuffer[] holder = RESPONSE_BB_HOLDER.get();
        ByteBuffer bb = holder[0];
        if (bb == null || bb.array() != response) {
            bb = littleEndianWrap(response, 0, response.length);
            holder[0] = bb;
        }
        bb.limit(offset + size);
        bb.position(offset);
        return bb;
    }

    double getValue(Sensor sensor);

    double getValue(String sensorName);

    boolean setValue(double value, String sensorName);

    /**
     * Resolve string-valued gauge labels (title, units) that contain bitStringValue() or stringValue() calls.
     *
     * @param ini the INI file model
     * @param configImage the configuration image for reading string/enum fields
     * @param outputChannelValues accumulated output channel numeric values
     * @return map of gauge name to resolved labels
     */
    static Map<String, ResolvedGaugeLabels> resolveGaugeLabels(IniFileModel ini,
            @Nullable ConfigurationImage configImage, Map<String, Double> outputChannelValues) {
        Map<String, ResolvedGaugeLabels> result = new HashMap<>();

        for (Map.Entry<String, GaugeModel> e : ini.getGauges().entrySet()) {
            GaugeModel gauge = e.getValue();
            String resolvedTitle = null;
            String resolvedUnits = null;

            if (gauge.getTitleValue().isExpression() && TsStringFunction.containsStringFunction(gauge.getTitle())) {
                resolvedTitle = TsStringFunction.resolve(gauge.getTitle(), ini, configImage, outputChannelValues);
            }
            if (gauge.getUnitsValue().isExpression() && TsStringFunction.containsStringFunction(gauge.getUnits())) {
                resolvedUnits = TsStringFunction.resolve(gauge.getUnits(), ini, configImage, outputChannelValues);
            }

            if (resolvedTitle != null || resolvedUnits != null) {
                result.put(e.getKey(), new ResolvedGaugeLabels(
                    resolvedTitle != null ? resolvedTitle : gauge.getTitle(),
                    resolvedUnits != null ? resolvedUnits : gauge.getUnits()
                ));
            }
        }
        return result;
    }

    class ResolvedGaugeLabels {
        private final String title;
        private final String units;

        public ResolvedGaugeLabels(String title, String units) {
            this.title = title;
            this.units = units;
        }

        public String getTitle() {
            return title;
        }

        public String getUnits() {
            return units;
        }
    }
}
