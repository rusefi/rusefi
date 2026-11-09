package com.rusefi.pinout;

import java.io.*;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

import static com.devexperts.logging.Logging.getLogging;

public class FileSystemBoardInputsImpl extends FileSystemBoardInputsReaderImpl {
    public FileSystemBoardInputsImpl(String boardName) {
        super(boardName);
    }

    @Override
    public Writer getBoardNamesWriter() throws IOException {
        return getWriter(boardName + PinoutLogicConstants.CONNECTORS + File.separator + "generated_ts_name_by_pin.cpp");
    }

    @Override
    public Writer getOutputsWriter() throws IOException {
        return getWriter(boardName + PinoutLogicConstants.CONNECTORS + File.separator + "generated_outputs.h");
    }

    @Override
    public Writer getBoardPinNamesWriter() throws IOException {
        return getWriter(boardName + PinoutLogicConstants.CONNECTORS + File.separator + "generated_board_pin_names.h");
    }

    private static Writer getWriter(String fileName) throws IOException {
        Path path = Paths.get(fileName);
        StringWriter contents = new StringWriter();
        return new FilterWriter(contents) {
            @Override
            public void close() throws IOException {
                super.close();
                byte[] newContents = contents.toString().getBytes(Charset.defaultCharset());
                if (!Files.exists(path) || !Arrays.equals(Files.readAllBytes(path), newContents)) {
                    Files.write(path, newContents);
                }
            }
        };
    }
}
