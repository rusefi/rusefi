package com.rusefi.pinout;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.util.List;

public interface BoardInputs {
    List<?> getBoardYamlKeys();

    Reader getReader(Object yamlKey) throws FileNotFoundException;

    String getName();

    List<String> getInputFiles();

    Writer getWriter() throws IOException;
}
