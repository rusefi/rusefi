package com.rusefi.io.commands;

import com.rusefi.io.IoStream;

import java.io.IOException;

public interface Command {
    byte getCommand();

    void handle(IoStream stream) throws IOException;
}
