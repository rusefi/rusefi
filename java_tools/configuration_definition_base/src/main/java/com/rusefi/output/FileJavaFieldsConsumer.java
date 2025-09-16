package com.rusefi.output;

import com.rusefi.ReaderState;
import com.rusefi.ToolUtil;
import com.rusefi.util.LazyFile;
import org.jetbrains.annotations.NotNull;

import static com.rusefi.ToolUtil.EOL;

/**
 * This class generates java representation of rusEfi data structures used by rusEfi console
 */
public class FileJavaFieldsConsumer {
    private static final String JAVA_PACKAGE = "com.rusefi.config.generated";

    @NotNull
    public static String remoteExtension(String fileNameWithExtension) {
        return fileNameWithExtension.substring(0, fileNameWithExtension.indexOf('.'));
    }

    static void startJavaFile(LazyFile file, String className, ReaderState state, Class<?> clazz) {
        writePackageLine(file);
        file.write("// this file " + state.getHeader() + ToolUtil.EOL + EOL);
        file.write("// by " + clazz + EOL);
        file.write("import com.rusefi.config.*;" + EOL + EOL);
        writeClassOpenLine(file, className);
    }

    public static void writeClassOpenLine(LazyFile lazyFile, String className) {
        lazyFile.write("public class " + className + " {" + ToolUtil.EOL);
    }

    public static void writePackageLine(LazyFile lazyFile) {
        lazyFile.write("package " + JAVA_PACKAGE + ";" + ToolUtil.EOL + ToolUtil.EOL);
    }
}
