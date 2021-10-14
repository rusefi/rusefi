package com.rusefi.ui.livedata;

import org.junit.Test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.net.URISyntaxException;
import java.net.URL;

import static org.junit.Assert.assertNotNull;

public class SourceReaderTest {
    @Test
    public void testResourceLocated() throws URISyntaxException, FileNotFoundException {
        URL cpp = getClass().getResource("/c_sources/ac_control.cpp");
        assertNotNull(cpp);
        new FileReader(new File(cpp.toURI()));
    }
}
