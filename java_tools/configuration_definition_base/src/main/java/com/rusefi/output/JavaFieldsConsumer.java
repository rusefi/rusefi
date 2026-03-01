package com.rusefi.output;

import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.*;
import com.rusefi.parse.TypesHelper;

import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import static com.rusefi.ToolUtil.EOL;

public abstract class JavaFieldsConsumer implements ConfigurationConsumer {
    protected final Set<String> existingJavaEnums = new HashSet<>();

    private final StringBuilder content = new StringBuilder();
    protected final StringBuffer allFields = new StringBuffer();
    protected final ReaderState state;
    private final int structureStartingTsPosition;

    public JavaFieldsConsumer(ReaderState state, int structureStartingTsPosition) {
        this.state = state;
        this.structureStartingTsPosition = structureStartingTsPosition;
    }

    public String getContent() {
        return content.toString();
    }

    private void writeJavaFieldName(String nameWithPrefix, int tsPosition) {
        allFields.append("\t" + nameWithPrefix.toUpperCase() + "," + EOL);

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
        String custom = state.getTsCustomLine().get(configField.getTypeName());
        return custom != null && custom.toLowerCase().startsWith(IniFileReader.FIELD_TYPE_STRING);
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
                    if (isUsefulField(configField)) {
                        writeJavaFieldName(nameWithPrefix, tsPosition + iterator.bitState.getByteOffset());
                        content.append("FieldType.BIT, " + iterator.bitState.getBitIndex() + ")" + terminateField());
                    }
                    return iterator.adjustSize(tsPosition);
                }

                if (TypesHelper.isFloat(configField.getTypeName())) {
                    writeJavaFieldName(nameWithPrefix, tsPosition);
                    content.append("FieldType.FLOAT)" + terminateField());
                } else {
                    String enumOptions = state.getVariableRegistry().get(configField.getTypeName() + VariableRegistry.ARRAY_FORMAT_ENUM);
                    if (enumOptions == null)
                        enumOptions = state.getVariableRegistry().get(configField.getTypeName() + VariableRegistry.KEY_VALUE_FORMAT_ENUM);

                    if (enumOptions != null && !existingJavaEnums.contains(configField.getTypeName())) {
                        existingJavaEnums.add(configField.getTypeName());
                        content.append("\tpublic static final String[] " + configField.getTypeName() + " = {" + enumOptions + "};" + EOL);
                    }


                    if (isUsefulField(configField)) {
                        writeJavaFieldName(nameWithPrefix, tsPosition);
                        if (isStringField(configField)) {
                            String custom = state.getTsCustomLine().get(configField.getTypeName());
                            String[] tokens = custom.split(",");
                            String stringSize = tokens[3].trim();
                            content.append(stringSize + ", FieldType.STRING");
                        } else {
                            content.append(getJavaType(configField.getElementSize()));
                        }
                        if (enumOptions != null) {
                            content.append(", " + configField.getTypeName());
                        }
                        content.append(")" + ".setScale(" + configField.autoscaleSpecNumber() + ")" +
                            terminateField());
                    }
                }

                return iterator.adjustSize(tsPosition);
            }
        };
        fieldsStrategy.run(state, structure, 0);
    }

    private static boolean isUsefulField(ConfigField configField) {
        return !configField.getName().startsWith(UnusedPrefix.UNUSED_ANYTHING_PREFIX);
    }

    private String terminateField() {
        return ".setBaseOffset(" + structureStartingTsPosition + ")" +
                ";" + EOL;
    }
}
