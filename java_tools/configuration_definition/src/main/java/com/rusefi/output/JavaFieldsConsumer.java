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

    private void writeJavaFieldName(String nameWithPrefix, int tsPosition) throws IOException {
        javaFieldsWriter.write("\tpublic static final Field ");
        allFields.append("\t" + nameWithPrefix.toUpperCase() + "," + EOL);
        javaFieldsWriter.write(nameWithPrefix.toUpperCase());
        javaFieldsWriter.write(" = Field.create(\"" + nameWithPrefix.toUpperCase() + "\", "
                + tsPosition + ", ");
    }

    private int writeJavaFields(List<ConfigField> tsFields, String prefix, int tsPosition) throws IOException {
        BitState bitState = new BitState();
        ConfigField prev = ConfigField.VOID;
        for (int i = 0; i < tsFields.size(); i++) {
            ConfigField next = i == tsFields.size() - 1 ? ConfigField.VOID : tsFields.get(i + 1);
            ConfigField cf = tsFields.get(i);
            // skip duplicate names
            if (cf.getName().equals(prev.getName()) || cf.isDirective())
                continue;
            tsPosition = writeOneField(cf, prefix, tsPosition, next, bitState.get());

            bitState.incrementBitIndex(cf, next);
            prev = cf;
        }
        return tsPosition;
    }

    private int writeOneField(ConfigField configField, String prefix, int tsPosition, ConfigField next, int bitIndex) throws IOException {
        ConfigStructure cs = configField.getState().structures.get(configField.getType());
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? configField.getName() + "_" : "";
            return writeJavaFields(cs.tsFields, prefix + extraPrefix, tsPosition);
        }

        String nameWithPrefix = prefix + configField.getName();

        if (configField.isBit()) {
            writeJavaFieldName(nameWithPrefix, tsPosition);
            javaFieldsWriter.append("FieldType.BIT, " + bitIndex + ");" + EOL);
            tsPosition += configField.getSize(next);
            return tsPosition;
        }

        if (TypesHelper.isFloat(configField.getType())) {
            writeJavaFieldName(nameWithPrefix, tsPosition);
            javaFieldsWriter.write("FieldType.FLOAT);" + EOL);
        } else {
            String enumOptions = state.variableRegistry.get(configField.getType() + VariableRegistry.ENUM_SUFFIX);

            if (enumOptions != null && !javaEnums.contains(configField.getType())) {
                javaEnums.add(configField.getType());
                javaFieldsWriter.write("\tpublic static final String[] " + configField.getType() + " = {" + enumOptions + "};" + EOL);
            }


            writeJavaFieldName(nameWithPrefix, tsPosition);
            if (isStringField(configField)) {
                String custom = state.tsCustomLine.get(configField.getType());
                String[] tokens = custom.split(",");
                String stringSize = tokens[3].trim();
                javaFieldsWriter.write(stringSize + ", FieldType.STRING");
            } else  if (configField.getElementSize() == 1) {
                javaFieldsWriter.write("FieldType.INT8");
            } else if (configField.getElementSize() == 2) {
                javaFieldsWriter.write("FieldType.INT16");
            } else {
                javaFieldsWriter.write("FieldType.INT");
            }
            if (enumOptions != null) {
                javaFieldsWriter.write(", " + configField.getType());
            }
            javaFieldsWriter.write(");" + EOL);
        }

        tsPosition += configField.getSize(next);

        return tsPosition;
    }

    private boolean isStringField(ConfigField configField) {
        String custom = state.tsCustomLine.get(configField.getType());
        return custom != null && custom.toLowerCase().startsWith(IniFileModel.FIELD_TYPE_STRING);
    }

    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            writeJavaFields(structure.tsFields, "", 0);
        }
    }
}
