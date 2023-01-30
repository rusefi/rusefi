package com.rusefi.core.io;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class BundleUtilTest {
    @Test
    public void testExtractBundleTarget() {
        assertEquals("proteus_f7", BundleUtil.getBundleTarget("rusefi.snapshot.proteus_f7"));
    }
}
