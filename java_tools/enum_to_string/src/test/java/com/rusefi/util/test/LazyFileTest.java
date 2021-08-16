package com.rusefi.util.test;

import com.rusefi.util.LazyFile;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class LazyFileTest {
    @Test
    public void testUnifySpaces() {
        assertEquals("abc", LazyFile.unifySpaces("a\r\n\r\nb\n\n\nc"));
    }
}
