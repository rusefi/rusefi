package com.rusefi.core.io;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.util.concurrent.atomic.AtomicInteger;

import com.rusefi.core.FindFileHelper;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class FindFileHelperTest {
    @Test
    public void testNullInCaseOfNoMatches() {
        String testPrefix = "file1";
        String testSuffix = "bin";

        String[] testFileArray = {"file1", "invalid file"};

        AtomicInteger counter = new AtomicInteger();
        String result = FindFileHelper.findXNumberOfFile("fileDirectory", testPrefix, testSuffix, testFileArray, (fileDirectory, fileName) -> counter.incrementAndGet(), true);
        Assertions.assertNull(result);
        assertEquals(0, counter.get());
    }

    @Test
    public void testValueInCaseOfOneMatches() {
        String testPrefix = "file1";
        String testSuffix = "bin";


        String[] testFileArray = {"file1", "invalid file", "file1 .bin", "file1.bin"};

        AtomicInteger counter = new AtomicInteger();
        String result = FindFileHelper.findXNumberOfFile("fileDirectory", testPrefix, testSuffix, testFileArray, (fileDirectory, fileName) -> counter.incrementAndGet(), true);
        assertEquals("fileDirectory" + File.separator + "file1.bin", result);
        assertEquals(0, counter.get());
    }

    @Test
    public void testMultipleMatches() {
        String testPrefix = "file1";
        String testSuffix = "bin";

        String[] testFileArray = {"file1.bin", "invalid file", "file1.bin"};

        AtomicInteger counter = new AtomicInteger();

        FindFileHelper.findXNumberOfFile("fileDirectory", testPrefix, testSuffix, testFileArray, (fileDirectory, fileName) -> counter.incrementAndGet(), true);
        assertEquals(1, counter.get());
    }
}

