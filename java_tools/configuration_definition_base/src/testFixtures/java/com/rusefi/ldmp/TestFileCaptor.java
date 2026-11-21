package com.rusefi.ldmp;

import com.rusefi.util.LazyFile;

import java.util.HashMap;
import java.util.Map;

import static com.rusefi.AssertCompatibility.assertEquals;
import static com.rusefi.AssertCompatibility.assertNotNull;

public class TestFileCaptor implements LazyFile.LazyFileFactory {
    Map<String, StringBufferLazyFile> fileCapture = new HashMap<>();
    /** how many times each file was opened for writing, see [tag:live_data_single_write] */
    Map<String, Integer> createCount = new HashMap<>();

    void assertOutput(String expected, String key) {
        StringBufferLazyFile stringBufferLazyFile = fileCapture.get(key);
        assertNotNull("Not found " + key, stringBufferLazyFile);
        assertEquals(expected, stringBufferLazyFile.sb.toString());
    }

    @Override
    public LazyFile create(String fileName) {
        StringBufferLazyFile file = new StringBufferLazyFile();
        fileCapture.put(fileName, file);
        createCount.merge(fileName, 1, Integer::sum);
        return file;
    }

}
