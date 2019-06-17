package com.rusefi.output;

import com.rusefi.*;
import com.rusefi.util.LazyFile;

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
    private static final String JAVA_PACKAGE = "com.rusefi.config.generated";

    private final CharArrayWriter javaFieldsWriter;
    private final ReaderState state;
    private final LazyFile javaFields;
    private final StringBuffer allFields = new StringBuffer("\tpublic static final Field[] VALUES = {" + EOL);
    private final String className;

    public JavaFieldsConsumer(ReaderState state, String javaDestination) {
        this.javaFieldsWriter = new CharArrayWriter();
        this.state = state;

        javaFields = new LazyFile(javaDestination);
        String className = new File(javaDestination).getName();
        this.className = className.substring(0, className.indexOf('.'));
    }

    private int writeJavaFields(List<ConfigField> tsFields, String prefix, int tsPosition) throws IOException {
        FieldIterator fieldIterator = new FieldIterator();
        for (int i = 0; i < tsFields.size(); i++) {
            ConfigField next = i == tsFields.size() - 1 ? ConfigField.VOID : tsFields.get(i + 1);
            ConfigField cf = tsFields.get(i);
            tsPosition = writeJavaFields(cf, prefix, tsPosition, next, fieldIterator.bitState.get());

            fieldIterator.bitState.incrementBitIndex(cf, next);
        }
        return tsPosition;
    }

    private int writeJavaFields(ConfigField configField, String prefix, int tsPosition, ConfigField next, int bitIndex) throws IOException {
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

        if (configField.getArraySize() != 1) {
            // todo: array support
        } else if (TypesHelper.isFloat(configField.getType())) {
            writeJavaFieldName(nameWithPrefix, tsPosition);
            javaFieldsWriter.write("FieldType.FLOAT);" + EOL);
        } else {
            String enumOptions = VariableRegistry.INSTANCE.get(configField.getType() + ConfigField.ENUM_SUFFIX);

            if (enumOptions != null && !javaEnums.contains(configField.getType())) {
                javaEnums.add(configField.getType());
                javaFieldsWriter.write("\tpublic static final String[] " + configField.getType() + " = {" + enumOptions + "};" + EOL);
            }

            writeJavaFieldName(nameWithPrefix, tsPosition);
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

    private void writeJavaFieldName(String nameWithPrefix, int tsPosition) throws IOException {
        javaFieldsWriter.write("\tpublic static final Field ");
        allFields.append("\t" + nameWithPrefix.toUpperCase() + "," + EOL);
        javaFieldsWriter.write(nameWithPrefix.toUpperCase());
        javaFieldsWriter.write(" = Field.create(\"" + nameWithPrefix.toUpperCase() + "\", "
                + tsPosition + ", ");
    }

    @Override
    public void startFile() {
        javaFields.write("package " + JAVA_PACKAGE + ";" + ConfigDefinition.EOL + ConfigDefinition.EOL);
        javaFields.write("// this file " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL + EOL);
        javaFields.write("import com.rusefi.config.*;" + EOL + EOL);
        javaFields.write("public class " + className + " {" + ConfigDefinition.EOL);
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            writeJavaFields(structure.tsFields, "", 0);
        }
    }

    @Override
    public void endFile() throws IOException {
        javaFields.write(VariableRegistry.INSTANCE.getJavaConstants());
        javaFields.write(javaFieldsWriter.toString());

        allFields.append("\t};" + EOL);
        javaFields.write(allFields.toString());

        javaFields.write("}" + ConfigDefinition.EOL);
        javaFields.close();
    }
}
