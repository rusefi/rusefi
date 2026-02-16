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
import com.rusefi.config.Field;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

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
        // Context for expression evaluation - maps output channel names to their values
        Map<String, Double> outputChannelValues = new HashMap<>();
        // Gauges that need expression evaluation (deferred to second pass)
        List<Map.Entry<String, GaugeModel>> expressionGauges = new ArrayList<>();

        // First pass: process direct output channel references and collect values
        for (Map.Entry<String, GaugeModel> e : ini.getGauges().entrySet()) {
            String gaugeName = e.getKey();
            GaugeModel gaugeModel = e.getValue();
            String channel = gaugeModel.getChannel();

            try {
                IniField sensorField = ini.getOutputChannel(channel);
                Double value = readFieldValue(response, gaugeName, sensorField);
                if (value != null) {
                    setValue(value, channel);
                    outputChannelValues.put(channel, value);
                } else {
                    log.warn("Unsupported field type for " + e);
                }
            } catch (IniMemberNotFound ex) {
                // Channel might be:
                // 1. An expression like "{ intake }" or "{ coolant * 1.8 + 32 }"
                // 2. An expression output channel name like "coolantTemperature" which is defined as an expression
                if (ExpressionEvaluator.looksLikeExpression(channel)) {
                    // Check if it's a simple wrapped variable like "{ intake }"
                    String simpleVar = ExpressionEvaluator.extractSimpleVariableName(channel);
                    if (simpleVar != null) {
                        // Try to look up the unwrapped variable name
                        try {
                            IniField sensorField = ini.getOutputChannel(simpleVar);
                            Double value = readFieldValue(response, gaugeName, sensorField);
                            if (value != null) {
                                setValue(value, simpleVar);
                                outputChannelValues.put(simpleVar, value);
                                continue;
                            }
                        } catch (IniMemberNotFound ignored) {
                            // Will be handled in expression pass
                        }
                    }
                    // Defer to second pass for complex expressions
                    expressionGauges.add(e);
                } else {
                    if (ini.getExpressionOutputChannel(channel) != null) {
                        // It's an expression output channel - defer to second pass
                        expressionGauges.add(e);
                    } else {
                        log.warn("Member not found for " + e);
                    }
                }
            }
        }

        // Second pass: evaluate expression-based gauges using collected output channel values
        // plus any configuration values needed for ternary expressions
        for (Map.Entry<String, GaugeModel> e : expressionGauges) {
            String gaugeName = e.getKey();
            GaugeModel gaugeModel = e.getValue();
            String channel = gaugeModel.getChannel();

            // Determine the actual expression to evaluate
            String expression;
            if (ExpressionEvaluator.looksLikeExpression(channel)) {
                expression = channel;
            } else {
                // Channel might reference an expression output channel
                String exprChannel = ini.getExpressionOutputChannel(channel);
                if (exprChannel != null) {
                    expression = exprChannel;
                } else {
                    log.warn("No expression found for gauge " + gaugeName + ": " + channel);
                    continue;
                }
            }

            // Build context with output channel values and any needed config values
            Map<String, Double> context = new HashMap<>(outputChannelValues);

            // Extract variables needed for this expression
            Set<String> neededVars = ExpressionEvaluator.extractVariables(expression);
            for (String varName : neededVars) {
                if (!context.containsKey(varName)) {
                    // First try to read from output channels (for raw values like "coolant", "intake")
                    try {
                        IniField sensorField = ini.getOutputChannel(varName);
                        Double value = readFieldValue(response, varName, sensorField);
                        if (value != null) {
                            context.put(varName, value);
                            setValue(value, varName);
                            outputChannelValues.put(varName, value);
                        }
                    } catch (IniMemberNotFound ignored) {
                        // Not a direct output channel, try config
                    }

                    // If still not found, and we have a config image, try configuration (for example, useMetricOnInterface)
                    if (!context.containsKey(varName) && configImage != null) {
                        Double configValue = getConfigValue(varName, ini, configImage);
                        if (configValue != null) {
                            context.put(varName, configValue);
                        }
                    }
                }
            }

            Double result = ExpressionEvaluator.tryEvaluateWithContext(expression, context);
            if (result != null) {
                setValue(result, channel);
            } else {
                log.warn("Could not evaluate expression for gauge " + gaugeName + ": " + expression);
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
        IniField iniField = ini.getIniField(fieldName);
        if (iniField == null) {
            return null;
        }

        if (iniField instanceof ScalarIniField) {
            ScalarIniField scalarField = (ScalarIniField) iniField;
            ByteBuffer bb = configImage.getByteBuffer(scalarField.getOffset(), scalarField.getType().getStorageSize());
            double rawValue = getRawValue(bb, scalarField.getType());
            return rawValue * scalarField.getMultiplier();
        }

        // For bit fields, we need to extract the bit value
        // The IniField may have bit information
        try {
            Field field = new Field(fieldName, iniField.getOffset(), com.rusefi.config.FieldType.INT);
            return field.getValue(configImage);
        } catch (Exception e) {
            log.debug("Failed to get config value for " + fieldName + ": " + e.getMessage());
            return null;
        }
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
