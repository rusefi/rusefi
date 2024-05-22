package com.rusefi;

import com.rusefi.output.ConfigStructure;
import com.rusefi.output.ConfigurationConsumer;

import java.io.IOException;
import java.util.List;
import java.util.Map;

public interface ReaderState {
    void setWithC_Defines(boolean withC_Defines);

    EnumsReader getEnumsReader();

    void doJob() throws IOException;

    String getHeader();

    void setDefinitionInputFile(String definitionInputFile);

    void addCHeaderDestination(String cHeader);

    void addPrepend(String fileName);

    void addPrependNotInput(String fileName);

    void addDestination(ConfigurationConsumer... consumers);

    VariableRegistry getVariableRegistry();

    Map<String, Integer> getTsCustomSize();

    Map<String, ? extends ConfigStructure> getStructures();

    Map<String, String> getTsCustomLine();

    void setHeaderMessage(String headerMessage);

    String getTsFileOutputName();

    void setTsFileOutputName(String tsFileOutputName);

    boolean isStackEmpty();

    ConfigStructure peek();
}
