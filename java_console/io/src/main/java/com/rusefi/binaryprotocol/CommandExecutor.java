package com.rusefi.binaryprotocol;

public interface CommandExecutor {
    byte[] executeCommand(char opcode, byte[] packet, String loggingMsg);
}
