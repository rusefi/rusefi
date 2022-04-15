package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.IOException;
import java.util.List;

public class JavaSensorsConsumer implements ConfigurationConsumer {
    private final ReaderState state;

    private final StringBuilder sb = new StringBuilder();

    public JavaSensorsConsumer(ReaderState state) {

        this.state = state;
    }

    @Override
    public void startFile() {
    }

    @Override
    public void endFile() throws IOException {
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            writeJavaFields(structure.tsFields, "", 0);
        }
    }

    private void writeJavaFields(List<ConfigField> tsFields, String prefix, int tsPosition) {
        FieldIterator iterator = new FieldIterator(tsFields);
        for (int i = 0; i < tsFields.size(); i++) {
            iterator.start(i);
            tsPosition = writeOneField(iterator.cf, prefix, tsPosition, iterator.next,
                    iterator.bitState.get(),
                    iterator.getPrev());

            iterator.end();
        }
    }

    private int writeOneField(ConfigField configField, String prefix, int tsPosition, ConfigField next, int i, ConfigField prev) {
        if (configField.isBit()) {
            // do not support bits for now but need to account for size
            tsPosition += configField.getSize(next);
            return tsPosition;
        }
        sb.append(configField.getName()).append("(");

        sb.append(configField.getComment()).append(", ");

        sb.append("SensorCategory.SENSOR_INPUTS, ");
        sb.append(JavaFieldsConsumer.getJavaType(configField.getElementSize())).append(", ");
        sb.append(tsPosition).append(", ");
        sb.append("1").append(", "); // scale
        sb.append(configField.getMin()).append(", ");
        sb.append(configField.getMax()).append(", ");

        sb.append(quote(configField.getUnits())).append(")");


        sb.append(",");
        sb.append("\n");


        tsPosition += configField.getSize(next);
        return tsPosition;
    }

    private static String quote(String string) {
        return "\"" + string + "\"";
    }

    public String getContent() {
        return sb.toString();
    }
}
