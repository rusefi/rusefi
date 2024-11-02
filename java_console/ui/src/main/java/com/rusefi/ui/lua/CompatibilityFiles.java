package com.rusefi.ui.lua;

import java.io.IOException;
import java.nio.file.Path;

import static java.nio.file.Files.readAllBytes;

// [tag:java8] java 11 API in java 8 world
public class CompatibilityFiles {

    public static String readString(Path path) throws IOException {
        byte[] ba = readAllBytes(path);
        return new String(ba);
    }
}
