package com.rusefi;

import com.rusefi.util.LazyFile;

import java.io.*;

public interface ReaderProvider {
    ReaderProvider REAL = fileName -> new InputStreamReader(new FileInputStream(fileName), LazyFile.CHARSET.name());

    Reader read(String fileName) throws FileNotFoundException, UnsupportedEncodingException;
}
