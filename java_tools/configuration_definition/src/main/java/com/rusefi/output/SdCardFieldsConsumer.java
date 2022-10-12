package com.rusefi.output;

import static com.rusefi.output.JavaSensorsConsumer.quote;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.util.LazyFile;

import java.io.IOException;

public class SdCardFieldsConsumer implements ConfigurationConsumer {
    private final StringBuilder body = new StringBuilder();
    private final LazyFile output;

    public SdCardFieldsConsumer(String outputFileName) {
        output = new LazyFile(outputFileName);
    }

    @Override
    public void startFile() {
        ConfigurationConsumer.super.startFile();
    }

    @Override
    public void endFile() throws IOException {
        output.write("static constexpr LogField fields[] = {\r\n" +
                "{packedTime, GAUGE_NAME_TIME, \"sec\", 0},\n");
        output.write(getBody());
        output.write("};\r\n");
        output.close();
    }

    @Override
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
        if (configField.getName().startsWith(ConfigStructure.UNUSED_BIT_PREFIX))
            return "";
        if (configField.isBit())
            return "";

        if (configField.isFromIterate()) {
            String name = configField.getIterateOriginalName() + "[" + (configField.getIterateIndex() - 1) + "]";
            return getLine(readerState, configField, prefix, name);
        } else {
            return getLine(readerState, configField, prefix, configField.getName());
        }
    }

    private String getLine(ReaderState readerState, ConfigField configField, String prefix, String name) {
        return "\t{engine->outputChannels." + name +
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
