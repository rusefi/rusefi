package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.IOException;

import static com.rusefi.output.JavaSensorsConsumer.quote;

public class SdCardFieldsContent {
    private final StringBuilder body = new StringBuilder();

    public String home = "engine->outputChannels";
    public String conditional;
    public Boolean isPtr = false;

    public void handleEndStruct(ReaderState state, ConfigStructure structure) throws IOException {
        if (state.isStackEmpty()) {
            PerFieldWithStructuresIterator.Strategy strategy = new PerFieldWithStructuresIterator.Strategy() {
                @Override
                public String process(ReaderState state, ConfigField configField, String prefix) {
                    return processOutput(configField, prefix);
                }

                @Override
                public String getArrayElementName(ConfigField cf) {
                    return cf.getOriginalArrayName();
                }
            };
            PerFieldWithStructuresIterator iterator = new PerFieldWithStructuresIterator(state, structure.getTsFields(), "",
                    strategy, ".");
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

        String name = configField.getOriginalArrayName();
        return getLine(configField, prefix, prefix + name);
    }

    private String getLine(ConfigField configField, String prefix, String name) {
        String categoryStr = configField.getCategory();

        if(categoryStr == null) {
            categoryStr = "";
        } else {
            categoryStr = ", " + categoryStr;
        }

        String before = conditional == null ? "" : "#if " + conditional + "\n";
        String after = conditional == null ? "" : "#endif\n";

        return before
                + "\t{" + home + (isPtr ? "->" : ".") + name +
                ", "
                + DataLogConsumer.getHumanGaugeName(prefix, configField) +
                ", " +
                quote(configField.getUnits()) +
                ", " +
                configField.getDigits() +
                categoryStr +
                "},\n" +
                after;
    }

    public String getBody() {
        return body.toString();
    }
}
