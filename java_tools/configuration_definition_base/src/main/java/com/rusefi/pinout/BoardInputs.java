package com.rusefi.pinout;

import java.io.*;
import java.util.List;

public interface BoardInputs {
    List<? extends File> getBoardYamlKeys();

    Reader getReader(File yamlKey) throws FileNotFoundException;

    String getName();

    List<String> getInputFiles();

    Writer getBoardNamesWriter() throws IOException;

    Writer getOutputsWriter() throws IOException;

    Writer getBoardPinNamesWriter() throws IOException;

    List<String> getBoardMeta(String boardMetaFileName);
}
