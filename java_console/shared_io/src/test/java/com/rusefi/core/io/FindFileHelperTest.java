package com.rusefi.core.io;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.File;

import com.rusefi.core.FindFileHelper;

public class FindFileHelperTest {
    @Test
    public void testNullInCaseOfNoMatches() {
        String testPrefix = "file1";
        String testSuffix = "bin";
        
        String[] testFileArray = { "file1", "invalid file"};
        
        String result = FindFileHelper.findOneFile("fileDirectory", testPrefix, testSuffix,testFileArray);
        Assertions.assertNull(result);
    }
    
    @Test
    public void testValueInCaseOfOneMatches() {
        String testPrefix = "file1";
        String testSuffix = "bin";
        
        String[] testFileArray = { "file1", "invalid file", "file1 .bin", "file1.bin" };
        
        String result = FindFileHelper.findOneFile("fileDirectory", testPrefix, testSuffix,testFileArray);
        Assertions.assertEquals("fileDirectory" + File.separator + "file1.bin", result);
    }
    
    @Test
    public void testExceptioInCaseOfMultipleMatches() {
        String testPrefix = "file1";
        String testSuffix = "bin";
        
        String[] testFileArray = { "file1.bin", "invalid file", "file1.bin" };
        
        Assertions.assertThrows(IllegalStateException.class, () -> {
        	FindFileHelper.findOneFile("fileDirectory", testPrefix, testSuffix,testFileArray);
        });
    }
}

