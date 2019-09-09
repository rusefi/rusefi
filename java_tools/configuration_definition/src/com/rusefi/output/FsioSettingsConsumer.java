package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ConfigStructure;
import com.rusefi.ReaderState;

import java.io.CharArrayWriter;
import java.util.List;

public abstract class FsioSettingsConsumer implements ConfigurationConsumer {
    private final CharArrayWriter content = new CharArrayWriter();
    private final CharArrayWriter enumDefinition = new CharArrayWriter();
    private final ReaderState state;

    private int currentIndex = 1000;

    public FsioSettingsConsumer(ReaderState state) {
        this.state = state;
    }

    public String getContent() {
        return content.toString();
    }

    public String getEnumDefinition() {
        return enumDefinition.toString();
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) {
        if (state.stack.isEmpty()) {
            handleFields(structure.tsFields, "");
        }
    }

    private void handleFields(List<ConfigField> tsFields, String prefix) {

        for (int i = 0; i < tsFields.size(); i++) {
            ConfigField cf = tsFields.get(i);
            writeOneField(cf, prefix);

        }
    }

    private void writeOneField(ConfigField configField, String prefix) {
        ConfigStructure cs = configField.getState().structures.get(configField.getType());
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? configField.getName() + "_" : "";
            handleFields(cs.tsFields, prefix + extraPrefix);
            return;
        }


        if (configField.isFsioVisible()) {

            String nameWithPrefix = prefix + configField.getName();

            enumDefinition.append("FSIO_SETTING_" + nameWithPrefix.toUpperCase() + " = " + currentIndex++ + ",\n");


            content.append(nameWithPrefix + "\n");
        }


    }
}
