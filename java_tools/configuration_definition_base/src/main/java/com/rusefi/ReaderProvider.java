package com.rusefi;

import java.io.*;
import java.nio.charset.StandardCharsets;

public interface ReaderProvider {
    ReaderProvider REAL = fileName -> new InputStreamReader(new FileInputStream(fileName), StandardCharsets.ISO_8859_1);

    Reader read(String fileName) throws FileNotFoundException, UnsupportedEncodingException;
}
