package com.rusefi.io.commands;

import org.jetbrains.annotations.NotNull;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;

public class WriteChunkCommand {
    @NotNull
    public static DataInputStream createPayLoadStream(byte[] payload) {
        return new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
    }
}
