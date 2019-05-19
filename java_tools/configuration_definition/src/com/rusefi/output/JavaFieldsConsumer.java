package com.rusefi.output;

import com.rusefi.*;

import java.io.*;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import static com.rusefi.ConfigDefinition.EOL;

/**
 * This class generates java representation of rusEfi data structures used by rusEfi console
 */
public class JavaFieldsConsumer implements ConfigurationConsumer {
    private static final Set<String> javaEnums = new HashSet<>();
    private static final String JAVA_SOURCE_CODE_FILE_NAME = "models/src/com/rusefi/config/Fields.java";
    public static final String JAVA_PACKAGE = "com.rusefi.config";

    private final CharArrayWriter javaFieldsWriter;
    private final ReaderState state;
    private final String javaConsolePath;
    private final BufferedWriter javaFields;

    public JavaFieldsConsumer(CharArrayWriter javaFieldsWriter, ReaderState state, String javaConsolePath) throws IOException {
        this.javaFieldsWriter = javaFieldsWriter;
        this.state = state;
        this.javaConsolePath = javaConsolePath;

        javaFields = new BufferedWriter(new FileWriter(javaConsolePath + File.separator + JAVA_SOURCE_CODE_FILE_NAME));
    }

    public static int writeJavaFields(List<ConfigField> tsFields, ReaderState state, String prefix, Writer javaFieldsWriter, int tsPosition) throws IOException {
        FieldIterator fieldIterator = new FieldIterator();
        for (int i = 0; i < tsFields.size(); i++) {
            ConfigField next = i == tsFields.size() - 1 ? ConfigField.VOID : tsFields.get(i + 1);
            ConfigField cf = tsFields.get(i);
            tsPosition = writeJavaFields(cf, prefix, javaFieldsWriter, tsPosition, next, fieldIterator.bitState.get());

            fieldIterator.bitState.incrementBitIndex(cf, next);
        }
        return tsPosition;
    }

    public static int writeJavaFields(ConfigField configField, String prefix, Writer javaFieldsWriter, int tsPosition, ConfigField next, int bitIndex) throws IOException {
        ConfigStructure cs = configField.getState().structures.get(configField.getType());
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? configField.getName() + "_" : "";
            return writeJavaFields(cs.tsFields, configField.getState(), prefix + extraPrefix, javaFieldsWriter, tsPosition);
        }

        String nameWithPrefix = prefix + configField.getName();

        if (configField.getComment() != null && configField.getComment().startsWith(ConfigField.TS_COMMENT_TAG + "")) {
            ConfigDefinition.settingContextHelp.append("\t" + nameWithPrefix + " = \"" + configField.getCommentContent() + "\"" + EOL);
        }

        if (configField.isBit()) {
            configField.writeJavaFieldName(javaFieldsWriter, nameWithPrefix, tsPosition);
            javaFieldsWriter.append("FieldType.BIT, " + bitIndex + ");" + EOL);
            tsPosition += configField.getSize(next);
            return tsPosition;
        }


        if (configField.getArraySize() != 1) {
            // todo: array support
        } else if (TypesHelper.isFloat(configField.getType())) {
            configField.writeJavaFieldName(javaFieldsWriter, nameWithPrefix, tsPosition);
            javaFieldsWriter.write("FieldType.FLOAT);" + EOL);
        } else {
            String enumOptions = VariableRegistry.INSTANCE.get(configField.getType() + ConfigField.ENUM_SUFFIX);

            if (enumOptions != null && !javaEnums.contains(configField.getType())) {
                javaEnums.add(configField.getType());
                javaFieldsWriter.write("\tpublic static final String[] " + configField.getType() + " = {" + enumOptions + "};" + EOL);
            }

            configField.writeJavaFieldName(javaFieldsWriter, nameWithPrefix, tsPosition);
            if (configField.getElementSize() == 1) {
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

        tsPosition += configField.getArraySize() * configField.getElementSize();

        return tsPosition;
    }

    @Override
    public void startFile() throws IOException {
        javaFields.write("package " + JAVA_PACKAGE + ";" + ConfigDefinition.EOL + ConfigDefinition.EOL);
        javaFields.write("// this file " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL);
        javaFields.write("public class Fields {" + ConfigDefinition.EOL);
    }


    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            writeJavaFields(structure.tsFields, state, "", javaFieldsWriter, 0);
        }
    }

    @Override
    public void endFile() throws IOException {
        javaFields.write(VariableRegistry.INSTANCE.getJavaConstants());
        javaFields.write(javaFieldsWriter.toString());
        javaFields.write("}" + ConfigDefinition.EOL);
        javaFields.close();
    }
}
