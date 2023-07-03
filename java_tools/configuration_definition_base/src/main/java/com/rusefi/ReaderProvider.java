package com.rusefi;

import com.rusefi.util.IoUtils;

import java.io.*;

public interface ReaderProvider {
    ReaderProvider REAL = fileName -> new InputStreamReader(new FileInputStream(fileName), IoUtils.CHARSET.name());

    Reader read(String fileName) throws FileNotFoundException, UnsupportedEncodingException;
}
