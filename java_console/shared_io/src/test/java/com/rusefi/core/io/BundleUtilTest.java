package com.rusefi.core.io;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

public class BundleUtilTest {
    @Test
    public void testExtractBundleTarget() {
        BundleInfo info = BundleUtil.parse(Arrays.asList("platform=proteus_f7", "release=development"));

        Assertions.assertEquals("proteus_f7", info.getTarget());
        Assertions.assertEquals("development", info.getBranchName());
    }
}

