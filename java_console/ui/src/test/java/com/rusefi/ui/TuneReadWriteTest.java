package com.rusefi.ui;

import com.opensr5.io.ConfigurationImageFile;
import org.junit.Test;

import java.io.IOException;

/**
 * from IDEA this unit test needs to be exectuted with "empty" working directory
 */
public class TuneReadWriteTest {
    @Test
    public void testReadTsTune() throws IOException {
        String binary = "ui/src/test/resources/frankenso/current_configuration.rusefi_binary";
        System.out.println("Reading " + binary);
        ConfigurationImageFile.readFromFile(binary);
    }
}
