package com.rusefi.output;

import com.opensr5.ini.IniFileModel;
import com.rusefi.*;

import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import static com.rusefi.ToolUtil.EOL;

public abstract class JavaFieldsConsumer implements ConfigurationConsumer {
    protected final Set<String> existingJavaEnums = new HashSet<>();

    private final StringBuilder content = new StringBuilder();
    protected final StringBuffer allFields = new StringBuffer("\tpublic static final Field[] VALUES = {" + EOL);
    protected final ReaderState state;

    public JavaFieldsConsumer(ReaderState state) {
        this.state = state;
    }

    public String getContent() {
        return content.toString();
    }

    private void writeJavaFieldName(String nameWithPrefix, int tsPosition, double scale) {
        content.append("\tpublic static final Field ");
        allFields.append("\t" + nameWithPrefix.toUpperCase() + "," + EOL);
        content.append(nameWithPrefix.toUpperCase());
        content.append(" = Field.create(\"" + nameWithPrefix.toUpperCase() + "\", "
                + tsPosition + ", ");
    }

    public static String getJavaType(int elementSize) {
        if (elementSize == 1) {
            return ("FieldType.INT8");
        } else if (elementSize == 2) {
            return "FieldType.INT16";
        } else {
            return "FieldType.INT";
        }
    }

    private boolean isStringField(ConfigField configField) {
        String custom = state.tsCustomLine.get(configField.getType());
        return custom != null && custom.toLowerCase().startsWith(IniFileModel.FIELD_TYPE_STRING);
    }

    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        FieldsStrategy fieldsStrategy = new FieldsStrategy() {
            protected int writeOneField(FieldIterator iterator, String prefix, int tsPosition) {
                ConfigField prev = iterator.getPrev();
                ConfigField configField = iterator.cf;
                ConfigField next = iterator.next;
                int bitIndex = iterator.bitState.get();

                if (configField.isDirective())
                    return tsPosition;
                // skip duplicate names which happens in case of conditional compilation
                if (configField.getName().equals(prev.getName())) {
                    return tsPosition;
                }
                ConfigStructure cs = configField.getStructureType();
                if (cs != null) {
                    String extraPrefix = cs.withPrefix ? configField.getName() + "_" : "";
                    return writeFields(cs.tsFields, prefix + extraPrefix, tsPosition);
                }

                String nameWithPrefix = prefix + configField.getName();

                if (configField.isBit()) {
                    writeJavaFieldName(nameWithPrefix, tsPosition, 1);
                    content.append("FieldType.BIT, " + bitIndex + ");" + EOL);
                    tsPosition += configField.getSize(next);
                    return tsPosition;
                }

                if (TypesHelper.isFloat(configField.getType())) {
                    writeJavaFieldName(nameWithPrefix, tsPosition, configField.autoscaleSpecNumber());
                    content.append("FieldType.FLOAT);" + EOL);
                } else {
                    String enumOptions = state.variableRegistry.get(configField.getType() + VariableRegistry.FULL_JAVA_ENUM);
                    if (enumOptions == null)
                        enumOptions = state.variableRegistry.get(configField.getType() + VariableRegistry.ENUM_SUFFIX);

                    if (enumOptions != null && !existingJavaEnums.contains(configField.getType())) {
                        existingJavaEnums.add(configField.getType());
                        content.append("\tpublic static final String[] " + configField.getType() + " = {" + enumOptions + "};" + EOL);
                    }


                    writeJavaFieldName(nameWithPrefix, tsPosition, configField.autoscaleSpecNumber());
                    if (isStringField(configField)) {
                        String custom = state.tsCustomLine.get(configField.getType());
                        String[] tokens = custom.split(",");
                        String stringSize = tokens[3].trim();
                        content.append(stringSize + ", FieldType.STRING");
                    } else {
                        content.append(getJavaType(configField.getElementSize()));
                    }
                    if (enumOptions != null) {
                        content.append(", " + configField.getType());
                    }
                    content.append(")" + ".setScale(" + configField.autoscaleSpecNumber() + ")" +
                            ";" + EOL);
                }

                tsPosition += configField.getSize(next);

                return tsPosition;
            }
        };
        fieldsStrategy.run(state, structure, 0);
    }
}
