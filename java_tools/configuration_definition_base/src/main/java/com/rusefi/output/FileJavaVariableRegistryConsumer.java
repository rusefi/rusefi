package com.rusefi.output;

import com.rusefi.ReaderState;
import com.rusefi.ReaderStateImpl;
import com.rusefi.ToolUtil;
import com.rusefi.util.LazyFile;

import java.io.IOException;

public class FileJavaVariableRegistryConsumer implements ConfigurationConsumer {
    private final String className;

    private final LazyFile java;
    private final ReaderStateImpl state;

    public FileJavaVariableRegistryConsumer(ReaderStateImpl state, String folderName, LazyFile.LazyFileFactory fileFactory, String className) {
        this.state = state;
        this.className = className;
        java = fileFactory.create(folderName + className + ".java");
    }

    @Override
    public void startFile() {
        FileJavaFieldsConsumer.startJavaFile(java, className, state, getClass());
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) {

    }

    public void endFile() throws IOException {
        java.write(state.getVariableRegistry().getJavaConstants());
        java.write("}" + ToolUtil.EOL);
        java.close();
    }
}
