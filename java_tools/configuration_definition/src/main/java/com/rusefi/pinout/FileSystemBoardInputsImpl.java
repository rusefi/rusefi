package com.rusefi.pinout;

import com.devexperts.logging.Logging;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;

public class FileSystemBoardInputsImpl implements BoardInputs {
    private static final Logging log = getLogging(FileSystemBoardInputsImpl.class);
    private final String boardName;
    private final List<File> boardYamlFiles;

    public FileSystemBoardInputsImpl(String boardName) {
        this.boardName = boardName;
        String dirPath = boardName + PinoutLogic.CONNECTORS;
        File dirName = new File(dirPath);
        FilenameFilter filter = (f, name) -> name.endsWith(".yaml");
        File[] boardYamlFilesArray = dirName.listFiles(filter);
        if (boardYamlFilesArray == null) {
            log.info("No yaml files in " + dirPath);
            boardYamlFiles = Collections.emptyList();
            return;
        }
        Arrays.sort(boardYamlFilesArray);
        log.info(boardName + ": " + Arrays.toString(boardYamlFilesArray));
        boardYamlFiles = Arrays.asList(boardYamlFilesArray);
    }

    @Override
    public List<?> getBoardYamlKeys() {
        return boardYamlFiles;
    }

    @Override
    public Reader getReader(Object yamlKey) throws FileNotFoundException {
        return new FileReader((File)yamlKey);
    }

    @Override
    public String getName() {
        return boardName;
    }

    @Override
    public List<String> getInputFiles() {
        List<String> result = new ArrayList<>();
        for (File yamlFile : boardYamlFiles) {
            result.add(boardName + PinoutLogic.CONNECTORS + File.separator + yamlFile.getName());
        }
        return result;
    }

    @Override
    public Writer getWriter() throws IOException {
        return new FileWriter(boardName + PinoutLogic.CONNECTORS + File.separator + "generated_ts_name_by_pin.cpp");
    }
}
