package com.rusefi.output;

import com.opensr5.ini.IniFileModel;
import com.rusefi.*;

import java.io.CharArrayWriter;
import java.io.IOException;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import static com.rusefi.ToolUtil.EOL;

public abstract class JavaFieldsConsumer implements ConfigurationConsumer {
    // todo: why is this field 'static'?
    protected static final Set<String> javaEnums = new HashSet<>();

    private final CharArrayWriter javaFieldsWriter = new CharArrayWriter();
    protected final StringBuffer allFields = new StringBuffer("\tpublic static final Field[] VALUES = {" + EOL);
    protected final ReaderState state;

    public JavaFieldsConsumer(ReaderState state) {
        this.state = state;
    }

    public String getJavaFieldsWriter() {
        return javaFieldsWriter.toString();
    }

    private void writeJavaFieldName(String nameWithPrefix, int tsPosition, double scale) throws IOException {
        javaFieldsWriter.write("\tpublic static final Field ");
        allFields.append("\t" + nameWithPrefix.toUpperCase() + "," + EOL);
        javaFieldsWriter.write(nameWithPrefix.toUpperCase());
        javaFieldsWriter.write(" = Field.create(\"" + nameWithPrefix.toUpperCase() + "\", "
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
            protected int writeOneField(ConfigField configField, String prefix, int tsPosition, ConfigField next, int bitIndex, ConfigField prev) throws IOException {
                if (configField.isDirective())
                    return tsPosition;
                // skip duplicate names which happens in case of conditional compilation
                if (configField.getName().equals(prev.getName())) {
                    return tsPosition;
                }
                ConfigStructure cs = configField.getState().structures.get(configField.getType());
                if (cs != null) {
                    String extraPrefix = cs.withPrefix ? configField.getName() + "_" : "";
                    return writeJavaFields(cs.tsFields, prefix + extraPrefix, tsPosition);
                }

                String nameWithPrefix = prefix + configField.getName();

                if (configField.isBit()) {
                    writeJavaFieldName(nameWithPrefix, tsPosition, 1);
                    javaFieldsWriter.append("FieldType.BIT, " + bitIndex + ");" + EOL);
                    tsPosition += configField.getSize(next);
                    return tsPosition;
                }

                if (TypesHelper.isFloat(configField.getType())) {
                    writeJavaFieldName(nameWithPrefix, tsPosition, configField.autoscaleSpecNumber());
                    javaFieldsWriter.write("FieldType.FLOAT);" + EOL);
                } else {
                    String enumOptions = state.variableRegistry.get(configField.getType() + VariableRegistry.ENUM_SUFFIX);

                    if (enumOptions != null && !javaEnums.contains(configField.getType())) {
                        javaEnums.add(configField.getType());
                        javaFieldsWriter.write("\tpublic static final String[] " + configField.getType() + " = {" + enumOptions + "};" + EOL);
                    }


                    writeJavaFieldName(nameWithPrefix, tsPosition, configField.autoscaleSpecNumber());
                    if (isStringField(configField)) {
                        String custom = state.tsCustomLine.get(configField.getType());
                        String[] tokens = custom.split(",");
                        String stringSize = tokens[3].trim();
                        javaFieldsWriter.write(stringSize + ", FieldType.STRING");
                    } else {
                        javaFieldsWriter.write(getJavaType(configField.getElementSize()));
                    }
                    if (enumOptions != null) {
                        javaFieldsWriter.write(", " + configField.getType());
                    }
                    javaFieldsWriter.write(")" + ".setScale(" + configField.autoscaleSpecNumber() + ")" +
                            ";" + EOL);
                }

                tsPosition += configField.getSize(next);

                return tsPosition;
            }
        };
        fieldsStrategy.run(state, structure);
    }
}
