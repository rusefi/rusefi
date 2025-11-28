package com.rusefi.util.test;

import com.rusefi.util.LazyFileImpl;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class LazyFileTest {
    @Test
    public void testUnifySpaces() {
        assertEquals("a\nb\nc", LazyFileImpl.unifySpaces("a\r\n\r\nb\n\n\nc"));
    }
}
