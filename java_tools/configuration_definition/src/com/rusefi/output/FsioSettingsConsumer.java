package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ConfigStructure;
import com.rusefi.ReaderState;

import java.io.CharArrayWriter;
import java.util.List;

/**
 * Some configuration settings are exposed to FSIO
 * Here we generate relevant FSIO constants and lookup code
 */

public abstract class FsioSettingsConsumer implements ConfigurationConsumer {
    private final CharArrayWriter content = new CharArrayWriter();
    private final CharArrayWriter enumDefinition = new CharArrayWriter();
    private final CharArrayWriter names = new CharArrayWriter();
    private final CharArrayWriter strings = new CharArrayWriter();
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
            handleFields(structure.tsFields, "", "");
        }
    }

    private void handleFields(List<ConfigField> tsFields, String prefix, String cFieldPrefix) {

        for (int i = 0; i < tsFields.size(); i++) {
            ConfigField cf = tsFields.get(i);
            writeOneField(cf, prefix, cFieldPrefix);
        }
    }

    private void writeOneField(ConfigField configField, String prefix, String cNamePrefix) {
        ConfigStructure cs = configField.getState().structures.get(configField.getType());
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? configField.getName() + "." : "";
            String cFieldPrefix = cs.withPrefix ? configField.getCFieldName() + "." : "";
            handleFields(cs.tsFields, prefix + extraPrefix, prefix + cFieldPrefix);
            return;
        }

        if (configField.isFsioVisible()) {

            String nameWithPrefix = prefix + configField.getName();

            String nameWithPrefixUnderscore = nameWithPrefix.replaceAll("\\.", "_");
            String enumName = "FSIO_SETTING_" + nameWithPrefixUnderscore.toUpperCase();
            enumDefinition.append("\t" +
                    enumName + " = " + currentIndex++ + ",\n");


            String cFieldName = cNamePrefix + configField.getCFieldName();


            content.append("\tcase " + enumName + ":\n");
            content.append("\t\treturn engineConfiguration->" + cFieldName + ";\n");

            String rusEfiName = "\"cfg_" + nameWithPrefixUnderscore + "\"";

            strings.append("\tcase " + enumName + ":\n");
            strings.append("\t\treturn " + rusEfiName + ";\n");

            names.append("static LENameOrdinalPair le" + nameWithPrefixUnderscore + "(" + enumName + ", " + rusEfiName + ");\n");
        }
    }

    public String getNames() {
        return names.toString();
    }


    public String getStrings() {
        return strings.toString();
    }

}
