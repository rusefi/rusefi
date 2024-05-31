package com.rusefi.pinout;

import com.devexperts.logging.Logging;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;

public abstract class FileSystemBoardInputsReaderImpl implements BoardInputs {
    private static final Logging log = getLogging(FileSystemBoardInputsReaderImpl.class);
    protected final String boardName;
    private final List<File> boardYamlFiles;
    public static String PREFIX = "";

    public FileSystemBoardInputsReaderImpl(String boardName) {
        this.boardName = boardName;
        String dirPath = boardName + PinoutLogicConstants.CONNECTORS;
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
    public List<? extends File> getBoardYamlKeys() {
        return boardYamlFiles;
    }

    @Override
    public Reader getReader(File yamlKey) throws FileNotFoundException {
        return new FileReader(yamlKey);
    }

    @Override
    public String getName() {
        return boardName;
    }

    @Override
    public List<String> getInputFiles() {
        List<String> result = new ArrayList<>();
        for (File yamlFile : boardYamlFiles) {
            result.add(boardName + PinoutLogicConstants.CONNECTORS + File.separator + yamlFile.getName());
        }
        return result;
    }

    @Override
    public List<String> getBoardMeta(String boardMetaFileName) {
        try {
            String fileName = PREFIX + boardMetaFileName;
            log.info("Reading from " + new File(fileName).getAbsolutePath());
            return Files.readAllLines(Paths.get(fileName));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
