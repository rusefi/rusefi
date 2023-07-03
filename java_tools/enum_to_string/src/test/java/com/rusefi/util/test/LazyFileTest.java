package com.rusefi.util.test;

import com.rusefi.util.LazyFileImpl;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class LazyFileTest {
    @Test
    public void testUnifySpaces() {
        assertEquals("abc", LazyFileImpl.unifySpaces("a\r\n\r\nb\n\n\nc"));
    }
}
