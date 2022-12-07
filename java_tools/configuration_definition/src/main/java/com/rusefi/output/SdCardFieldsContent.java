package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.IOException;

import static com.rusefi.output.JavaSensorsConsumer.quote;

public class SdCardFieldsContent {
    private final StringBuilder body = new StringBuilder();

    public String home = "engine->outputChannels";

    public void handleEndStruct(ReaderState state, ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(state, structure.tsFields, "",
                    this::processOutput, ".");
            iterator.loop();
            String content = iterator.getContent();
            body.append(content);
        }
    }

    private String processOutput(ReaderState readerState, ConfigField configField, String prefix) {
        if (configField.getName().startsWith(ConfigStructure.ALIGNMENT_FILL_AT))
            return "";
        if (configField.getName().startsWith(ConfigStructure.UNUSED_ANYTHING_PREFIX))
            return "";
        if (configField.isBit())
            return "";

        if (configField.isFromIterate()) {
            String name = configField.getIterateOriginalName() + "[" + (configField.getIterateIndex() - 1) + "]";
            return getLine(readerState, configField, prefix, prefix + name);
        } else {
            return getLine(readerState, configField, prefix, prefix + configField.getName());
        }
    }

    private String getLine(ReaderState readerState, ConfigField configField, String prefix, String name) {
        return "\t{" + home + "." + name +
                ", "
                + DataLogConsumer.getComment(prefix, configField, readerState.variableRegistry) +
                ", " +
                quote(configField.getUnits()) +
                ", " +
                configField.getDigits() +

                "},\n";
    }

    public String getBody() {
        return body.toString();
    }
}
