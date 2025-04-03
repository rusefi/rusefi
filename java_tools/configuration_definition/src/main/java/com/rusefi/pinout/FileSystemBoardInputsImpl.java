package com.rusefi.pinout;

import java.io.*;

import static com.devexperts.logging.Logging.getLogging;

public class FileSystemBoardInputsImpl extends FileSystemBoardInputsReaderImpl {
    public FileSystemBoardInputsImpl(String boardName) {
        super(boardName);
    }

    @Override
    public Writer getBoardNamesWriter() throws IOException {
        return new FileWriter(boardName + PinoutLogicConstants.CONNECTORS + File.separator + "generated_ts_name_by_pin.cpp");
    }

    @Override
    public Writer getOutputsWriter() throws IOException {
        return new FileWriter(boardName + PinoutLogicConstants.CONNECTORS + File.separator + "generated_outputs.h");
    }

    @Override
    public Writer getBoardPinNamesWriter() throws IOException {
        return new FileWriter(boardName + PinoutLogicConstants.CONNECTORS + File.separator + "generated_board_pin_names.h");
    }
}
