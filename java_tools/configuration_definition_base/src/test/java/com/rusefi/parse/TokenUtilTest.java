package com.rusefi.parse;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;

public class TokenUtilTest {
    @Test
    public void test() {
        assertEquals("first", TokenUtil.grabFirstTokenAndTheRest("first s").first);
        assertEquals("second", TokenUtil.grabFirstTokenAndTheRest("f second \t").second);
        assertEquals("second", TokenUtil.grabFirstTokenAndTheRest("f\tsecond\t ").second);
    }
}
