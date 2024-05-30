package com.rusefi.core.io;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class BundleUtilTest {
    @Test
    public void testExtractBundleTarget() {
        Assertions.assertEquals("proteus_f7", BundleUtil.getBundleTarget("rusefi.snapshot.proteus_f7"));
        Assertions.assertEquals("snapshot", BundleUtil.parse("rusefi.snapshot.proteus_f7").getBranchName());
    }
}

