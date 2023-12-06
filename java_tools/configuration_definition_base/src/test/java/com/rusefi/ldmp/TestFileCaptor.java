package com.rusefi.ldmp;

import com.rusefi.util.LazyFile;

import java.util.HashMap;
import java.util.Map;

public class TestFileCaptor implements LazyFile.LazyFileFactory {
    Map<String, StringBufferLazyFile> fileCapture = new HashMap<>();

    @Override
    public LazyFile create(String fileName) {
        StringBufferLazyFile file = new StringBufferLazyFile();
        fileCapture.put(fileName, file);
        return file;
    }

}
