package com.rusefi;

import com.rusefi.output.ConfigStructure;
import com.rusefi.output.ConfigurationConsumer;

import java.io.IOException;
import java.util.List;
import java.util.Map;

public interface ReaderState {
    void setWithC_Defines(boolean withC_Defines);

    void doJob() throws IOException;

    void setDefinitionInputFile(String definitionInputFile);

    void addCHeaderDestination(String cHeader);

    void addPrepend(String fileName);

    void addDestination(ConfigurationConsumer... consumers);

    VariableRegistry getVariableRegistry();

    Map<String, Integer> getTsCustomSize();

    Map<String, ? extends ConfigStructure> getStructures();

    Map<String, String> getTsCustomLine();

    String getTsFileOutputName();

    void setTsFileOutputName(String tsFileOutputName);

    String getDefinitionInputFile();

    List<String> getPrependFiles();

    boolean isDestinationsEmpty();

    boolean isStackEmpty();
}
