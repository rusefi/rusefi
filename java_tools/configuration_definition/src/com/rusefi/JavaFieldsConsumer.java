package com.rusefi;

import java.io.*;

public class JavaFieldsConsumer implements ConfigurationConsumer {
    private static final String FIELDS_JAVA = "models/src/com/rusefi/config/Fields.java";

    private final CharArrayWriter javaFieldsWriter;
    private final ReaderState state;

    public JavaFieldsConsumer(CharArrayWriter javaFieldsWriter, ReaderState state) {
        this.javaFieldsWriter = javaFieldsWriter;
        this.state = state;
    }

    static void writeFields(String javaConsolePath, CharArrayWriter javaFieldsWriter) throws IOException {
        BufferedWriter javaFields = new BufferedWriter(new FileWriter(javaConsolePath + File.separator + FIELDS_JAVA));
        javaFields.write("package com.rusefi.config;" + ConfigDefinition.EOL + ConfigDefinition.EOL);
        javaFields.write("// this file " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL);
        javaFields.write("public class Fields {" + ConfigDefinition.EOL);
        javaFields.write(VariableRegistry.INSTANCE.getJavaConstants());
        javaFields.write(javaFieldsWriter.toString());
        javaFields.write("}" + ConfigDefinition.EOL);
        javaFields.close();
    }

    @Override
    public void startFile() throws IOException {

    }

    @Override
    public void endFile() throws IOException {

    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        if (state.stack.isEmpty()) {
            structure.writeJavaFields(state,"", javaFieldsWriter, 0);
        }
    }

    @Override
    public void onTotalEnd() {

    }
}
