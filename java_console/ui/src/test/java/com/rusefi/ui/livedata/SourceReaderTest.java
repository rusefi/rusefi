package com.rusefi.ui.livedata;

import com.rusefi.livedata.LiveDataParserPanel;
import org.junit.Test;

import java.io.*;
import java.net.URISyntaxException;

import static org.junit.Assert.assertNotNull;

public class SourceReaderTest {
    @Test
    public void testResourceLocated() throws URISyntaxException, IOException {
        String fileName = "ac_control.cpp";
        String content = LiveDataParserPanel.getContent(getClass(), fileName);
        assertNotNull(content);

    }

}
