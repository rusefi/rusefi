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
    protected final ReaderState state;
    private final int baseOffset;

    public JavaFieldsConsumer(ReaderState state, int baseOffset) {
        this.state = state;
        this.baseOffset = baseOffset;
    }

    public String getContent() {
        return content.toString();
    }

    private void writeJavaFieldName(String nameWithPrefix, int tsPosition) {
        content.append("\tpublic static final Field ");
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
        String custom = state.getTsCustomLine().get(configField.getType());
        return custom != null && custom.toLowerCase().startsWith(IniFileModel.FIELD_TYPE_STRING);
    }

    @Override
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
                    String extraPrefix = cs.isWithPrefix() ? configField.getName() + "_" : "";
                    return writeFields(cs.getTsFields(), prefix + extraPrefix, tsPosition);
                }

                String nameWithPrefix = prefix + configField.getName();

                if (configField.isBit()) {
                    if (!configField.getName().startsWith(ConfigStructure.UNUSED_ANYTHING_PREFIX)) {
                        writeJavaFieldName(nameWithPrefix, tsPosition);
                        content.append("FieldType.BIT, " + bitIndex + ")" + terminateField());
                    }
                    tsPosition += configField.getSize(next);
                    return tsPosition;
                }

                if (TypesHelper.isFloat(configField.getType())) {
                    writeJavaFieldName(nameWithPrefix, tsPosition);
                    content.append("FieldType.FLOAT)" + terminateField());
                } else {
                    String enumOptions = state.getVariableRegistry().get(configField.getType() + VariableRegistry.FULL_JAVA_ENUM);
                    if (enumOptions == null)
                        enumOptions = state.getVariableRegistry().get(configField.getType() + VariableRegistry.ENUM_SUFFIX);

                    if (enumOptions != null && !existingJavaEnums.contains(configField.getType())) {
                        existingJavaEnums.add(configField.getType());
                        content.append("\tpublic static final String[] " + configField.getType() + " = {" + enumOptions + "};" + EOL);
                    }


                    writeJavaFieldName(nameWithPrefix, tsPosition);
                    if (isStringField(configField)) {
                        String custom = state.getTsCustomLine().get(configField.getType());
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
                            terminateField());
                }

                tsPosition += configField.getSize(next);

                return tsPosition;
            }
        };
        fieldsStrategy.run(state, structure, 0);
    }

    private String terminateField() {
        return ".setBaseOffset(" + baseOffset + ")" +
                ";" + EOL;
    }
}
