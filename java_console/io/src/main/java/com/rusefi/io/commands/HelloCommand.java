package com.rusefi.io.commands;

import com.opensr5.Logger;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.tcp.BinaryProtocolServer;

import java.io.IOException;

public class HelloCommand implements Command {
    private final Logger logger;
    private final String tsSignature;

    public HelloCommand(Logger logger, String tsSignature) {
        this.logger = logger;
        this.tsSignature = tsSignature;
    }

    @Override
    public byte getCommand() {
        return Fields.TS_HELLO_COMMAND;
    }

    @Override
    public void handle(BinaryProtocolServer.Packet packet, IoStream stream) throws IOException {
        stream.sendPacket((BinaryProtocolServer.TS_OK + tsSignature).getBytes(), logger);
    }
}
