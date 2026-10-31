package com.rusefi.util.test;

import com.rusefi.util.LazyFileImpl;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class LazyFileTest {
    @Test
    public void testUnifySpaces() {
        // The shared logging helper normalizes CRLF while retaining line breaks.
        assertEquals("a\n\nb\n\n\nc", LazyFileImpl.unifySpaces("a\r\n\r\nb\n\n\nc"));
    }
}
