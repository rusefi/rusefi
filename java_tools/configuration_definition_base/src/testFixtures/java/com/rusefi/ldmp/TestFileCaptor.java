package com.rusefi.ldmp;

import com.rusefi.util.LazyFile;

import java.util.HashMap;
import java.util.Map;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class TestFileCaptor implements LazyFile.LazyFileFactory {
    Map<String, StringBufferLazyFile> fileCapture = new HashMap<>();

    void assertOutput(String expected, String key) {
        StringBufferLazyFile stringBufferLazyFile = fileCapture.get(key);
        assertNotNull("Not found " + key, stringBufferLazyFile);
        assertEquals(expected, stringBufferLazyFile.sb.toString());
    }

    @Override
    public LazyFile create(String fileName) {
        StringBufferLazyFile file = new StringBufferLazyFile();
        fileCapture.put(fileName, file);
        return file;
    }

}
