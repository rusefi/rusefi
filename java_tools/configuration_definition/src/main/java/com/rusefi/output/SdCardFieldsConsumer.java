package com.rusefi.output;

import static com.rusefi.output.JavaSensorsConsumer.quote;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.io.IOException;

public class SdCardFieldsConsumer implements ConfigurationConsumer {
    private final StringBuilder body = new StringBuilder();
    private final String outputFileName;

    public SdCardFieldsConsumer(String outputFileName) {
        this.outputFileName = outputFileName;
    }

    @Override
    public void startFile() {
        ConfigurationConsumer.super.startFile();
    }

    @Override
    public void endFile() throws IOException {
        ConfigurationConsumer.super.endFile();
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


        return "\t{engine->outputChannels." + configField.getName() +
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
