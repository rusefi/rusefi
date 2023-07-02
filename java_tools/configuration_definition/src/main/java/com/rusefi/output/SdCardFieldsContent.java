package com.rusefi.output;

import com.rusefi.core.SensorCategory;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.IOException;

import static com.rusefi.output.JavaSensorsConsumer.quote;

public class SdCardFieldsContent {
    private final StringBuilder body = new StringBuilder();

    public String home = "engine->outputChannels";
    public Boolean isPtr = false;

    public void handleEndStruct(ReaderState state, ConfigStructure structure) throws IOException {
        if (state.isStackEmpty()) {
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(state, structure.getTsFields(), "",
                    (configField, prefix, prefix2) -> processOutput(prefix, prefix2), ".");
            iterator.loop();
            String content = iterator.getContent();
            body.append(content);
        }
    }

    private String processOutput(ConfigField configField, String prefix) {
        if (configField.getName().startsWith(ConfigStructureImpl.ALIGNMENT_FILL_AT))
            return "";
        if (configField.getName().startsWith(ConfigStructure.UNUSED_ANYTHING_PREFIX))
            return "";
        if (configField.isBit())
            return "";

        if (configField.isFromIterate()) {
            String name = configField.getIterateOriginalName() + "[" + (configField.getIterateIndex() - 1) + "]";
            return getLine(configField, prefix, prefix + name);
        } else {
            return getLine(configField, prefix, prefix + configField.getName());
        }
    }

    private String getLine(ConfigField configField, String prefix, String name) {
        String categoryStr = configField.getCategory();

        if(categoryStr == null) {
            categoryStr = "";
        } else {
            categoryStr = ", " + categoryStr;
        }

        return "\t{" + home + (isPtr ? "->" : ".") + name +
                ", "
                + DataLogConsumer.getHumanGaugeName(prefix, configField) +
                ", " +
                quote(configField.getUnits()) +
                ", " +
                configField.getDigits() +
                categoryStr +
                "},\n";
    }

    public String getBody() {
        return body.toString();
    }
}
