package com.rusefi.core;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
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
    default void grabSensorValues(byte[] response, @NotNull IniFileModel ini, @Nullable ConfigurationImage configImage) {
        Map<String, Double> outputChannelValues = new HashMap<>();
        List<Map.Entry<String, GaugeModel>> expressionGauges = new ArrayList<>();

        // First pass: resolve direct output channel references
        for (Map.Entry<String, GaugeModel> e : ini.getGauges().entrySet()) {
            String gaugeName = e.getKey();
            String channel = e.getValue().getChannel();

            // Try the channel name directly
            Double value = tryReadOutputChannel(response, gaugeName, ini, channel);
            if (value != null) {
                setValue(value, channel);
                outputChannelValues.put(channel, value);
                continue;
            }

            // For simple wrapped expressions like "{ intake }", try the inner variable
            if (ExpressionEvaluator.looksLikeExpression(channel)) {
                String simpleVar = ExpressionEvaluator.extractSimpleVariableName(channel);
                if (simpleVar != null) {
                    value = tryReadOutputChannel(response, gaugeName, ini, simpleVar);
                    if (value != null) {
                        setValue(value, simpleVar);
                        outputChannelValues.put(simpleVar, value);
                        continue;
                    }
                }
                expressionGauges.add(e);
            } else if (ini.getExpressionOutputChannel(channel) != null) {
                expressionGauges.add(e);
            } else {
                log.warn("Member not found for " + e);
            }
        }

        // Second pass: evaluate expression-based gauges
        for (Map.Entry<String, GaugeModel> e : expressionGauges) {
            String gaugeName = e.getKey();
            String channel = e.getValue().getChannel();

            String expression = resolveExpression(ini, channel);
            if (expression == null) {
                log.warn("No expression found for gauge " + gaugeName + ": " + channel);
                continue;
            }

            Map<String, Double> context = new HashMap<>(outputChannelValues);
            resolveExpressionVariables(response, ini, configImage, expression, context, outputChannelValues);

            Double result = ExpressionEvaluator.tryEvaluateWithContext(expression, context);
            if (result != null) {
                setValue(result, channel);
            } else {
                log.warn("Could not evaluate expression for gauge " + gaugeName + ": " + expression);
            }
        }
    }

    /**
     * Try to read a named output channel from the response bytes.
     * @return the value, or null if the channel doesn't exist or has an unsupported type
     */
    @Nullable
    static Double tryReadOutputChannel(byte[] response, String label, IniFileModel ini, String channelName) {
        try {
            IniField field = ini.getOutputChannel(channelName);
            return readFieldValue(response, label, field);
        } catch (IniMemberNotFound e) {
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
        IniField iniField = optField.get();

        if (iniField instanceof ScalarIniField) {
            ScalarIniField scalarField = (ScalarIniField) iniField;
            ByteBuffer bb = configImage.getByteBuffer(scalarField.getOffset(), scalarField.getType().getStorageSize());
            double rawValue = getRawValue(bb, scalarField.getType());
            return rawValue * scalarField.getMultiplier();
        }

        if (iniField instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) iniField;
            ByteBuffer bb = configImage.getByteBuffer(enumField.getOffset(), enumField.getType().getStorageSize());
            int rawValue = (int) getRawValue(bb, enumField.getType());
            int bitCount = enumField.getBitSize0() + 1;
            int mask = (1 << bitCount) - 1;
            return (double) ((rawValue >> enumField.getBitPosition()) & mask);
        }

        return null;
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
            double rawValue = getRawValue(bb, scalarField.getType());
            return rawValue * scalarField.getMultiplier();
        } else if (field instanceof EnumIniField) {
            EnumIniField enumField = (EnumIniField) field;
            int rawValue = (int) getRawValue(bb, enumField.getType());
            int bitCount = enumField.getBitSize0() + 1;
            int mask = (1 << bitCount) - 1;
            return (double) ((rawValue >> enumField.getBitPosition()) & mask);
        }
        return null;
    }

    static double getRawValue(ByteBuffer bb, com.rusefi.config.FieldType type) {
        switch (type) {
            case FLOAT:
                return bb.getFloat();
            case INT:
                return bb.getInt();
            case UINT16:
                return bb.getInt() & 0xFFFF;
            case INT16:
                return (short) (bb.getInt() & 0xFFFF);
            case UINT8:
                return bb.getInt() & 0xFF;
            case INT8:
                return (byte) (bb.getInt() & 0xFF);
            default:
                throw new UnsupportedOperationException("type " + type);
        }
    }

    static @NotNull ByteBuffer getByteBuffer(byte[] response, String message, int fieldOffset) {
        int offset = fieldOffset + 1; // first byte is response code
        int size = 4;
        if (offset + size > response.length) {
            throw new IllegalArgumentException(message + String.format(" but %d+%d in %d", offset, size, response.length));
        }
        return littleEndianWrap(response, offset, size);
    }

    double getValue(Sensor sensor);

    double getValue(String sensorName);

    boolean setValue(double value, Sensor sensor);

    boolean setValue(double value, String sensorName);
}
