package com.rusefi.output;

import com.rusefi.ConfigDefinition;
import com.rusefi.ReaderState;
import com.rusefi.VariableRegistry;
import com.rusefi.util.LazyFile;

import java.io.File;
import java.io.IOException;

import static com.rusefi.ConfigDefinition.EOL;

/**
 * This class generates java representation of rusEfi data structures used by rusEfi console
 */
public class FileJavaFieldsConsumer extends JavaFieldsConsumer {
    private static final String JAVA_PACKAGE = "com.rusefi.config.generated";

    private final LazyFile javaFields;
    private final String className;

    public FileJavaFieldsConsumer(ReaderState state, String javaDestination) {
        super(state);
        javaFields = new LazyFile(javaDestination);
        String className = new File(javaDestination).getName();
        this.className = className.substring(0, className.indexOf('.'));
    }

    @Override
    public void startFile() {
        javaFields.write("package " + JAVA_PACKAGE + ";" + ConfigDefinition.EOL + ConfigDefinition.EOL);
        javaFields.write("// this file " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL + EOL);
        javaFields.write("// by " + getClass() + EOL);
        javaFields.write("import com.rusefi.config.*;" + EOL + EOL);
        javaFields.write("public class " + className + " {" + ConfigDefinition.EOL);
    }

    @Override
    public void endFile() throws IOException {
        javaFields.write(VariableRegistry.INSTANCE.getJavaConstants());
        javaFields.write(getJavaFieldsWriter());

        allFields.append("\t};" + EOL);
        javaFields.write(allFields.toString());

        javaFields.write("}" + ConfigDefinition.EOL);
        javaFields.close();
    }
}
