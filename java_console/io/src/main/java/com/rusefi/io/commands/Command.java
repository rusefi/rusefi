package com.rusefi.io.commands;

import com.rusefi.io.IoStream;
import com.rusefi.io.tcp.BinaryProtocolServer;

import java.io.IOException;

public interface Command {
    byte getCommand();

    void handle(BinaryProtocolServer.Packet packet, IoStream stream) throws IOException;
}
