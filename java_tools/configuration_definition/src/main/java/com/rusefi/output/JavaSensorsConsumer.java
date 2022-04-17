package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.IOException;

public class JavaSensorsConsumer implements ConfigurationConsumer {
    public int sensorTsPosition;

    private final StringBuilder sb = new StringBuilder();

    public JavaSensorsConsumer(int sensorTsPosition) {
        this.sensorTsPosition = sensorTsPosition;
    }

    @Override
    public void startFile() {
    }

    @Override
    public void endFile() throws IOException {
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        FieldsStrategy fieldsStrategy = new FieldsStrategy() {
            public int writeOneField(ConfigField configField, String prefix, int tsPosition, ConfigField next, int i, ConfigField prev) {
                if (configField.isBit()) {
                    // do not support bits for now but need to account for size
                    tsPosition += configField.getSize(next);
                    return tsPosition;
                }
                sb.append(configField.getName()).append("(");

                String string = readerState.variableRegistry.applyVariables(configField.getComment());
                if (string == null || string.isEmpty()) {
                    string = quote(configField.getName());
                } else if (string.charAt(0) != '"') {
                    // huh? weird conditional quoting?
                    string = quote(string);
                }

                sb.append(string).append(", ");

                sb.append("SensorCategory.SENSOR_INPUTS, ");
                sb.append(JavaFieldsConsumer.getJavaType(configField.getElementSize())).append(", ");
                sb.append(tsPosition).append(", ");
                sb.append(configField.autoscaleSpecNumber()).append(", "); // scale
                sb.append(configField.getMin()).append(", ");
                sb.append(configField.getMax()).append(", ");

                sb.append(quote(configField.getUnits())).append(")");


                sb.append(",");
                sb.append("\n");


                tsPosition += configField.getSize(next);
                return tsPosition;
            }
        };
        fieldsStrategy.run(readerState, structure);
    }


    private static String quote(String string) {
        return "\"" + string + "\"";
    }

    public String getContent() {
        return sb.toString();
    }
}
