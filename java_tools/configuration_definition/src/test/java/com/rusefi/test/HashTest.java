package com.rusefi.test;

import com.rusefi.output.HashUtil;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class HashTest {
    @Test
    public void testdjb2() {
        assertEquals(HashUtil.djb2lowerCase("Hello1"), 30950378);
        assertEquals(HashUtil.djb2lowerCase("Hello2"), 30950379);
        assertEquals(HashUtil.djb2lowerCase("HELLO2"), 30950379);
    }

}
