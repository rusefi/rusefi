package com.rusefi.core.io;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;

public class BundleUtilTest {
    @Test
    public void testExtractBundleTarget() {
        BundleInfo info = BundleUtil.parse(Arrays.asList("platform=proteus_f7", "release=development"));

        Assertions.assertEquals("proteus_f7", info.getTarget());
        Assertions.assertEquals("development", info.getBranchName());
    }

    /**
     * #6564: a truncated or hand-edited release.txt used to take the updater down with an
     * ArrayIndexOutOfBoundsException before it could report anything useful.
     */
    @Test
    public void testLineWithoutSeparatorIsIgnored() {
        BundleInfo info = BundleUtil.parse(Arrays.asList(
            "platform=proteus_f7",
            "",
            "# hand written comment",
            "release=development"
        ));

        Assertions.assertEquals("proteus_f7", info.getTarget());
        Assertions.assertEquals("development", info.getBranchName());
    }

    @Test
    public void testTruncatedContentIsReportedAsUndefined() {
        Assertions.assertTrue(BundleInfo.isUndefined(BundleUtil.parse(Collections.singletonList("platform"))));
    }

    @Test
    public void testMissingReleaseIsReportedAsUndefined() {
        Assertions.assertTrue(BundleInfo.isUndefined(BundleUtil.parse(Collections.singletonList("platform=proteus_f7"))));
    }

    @Test
    public void testEmptyContentIsReportedAsUndefined() {
        Assertions.assertTrue(BundleInfo.isUndefined(BundleUtil.parse(Collections.emptyList())));
    }

    @Test
    public void testNextReleaseIsParsed() {
        BundleInfo info = BundleUtil.parse(Arrays.asList(
            "platform=proteus_f7",
            "release=lts-25jersey",
            "nextRelease=lts-26jersey"
        ));

        Assertions.assertEquals("lts-25jersey", info.getBranchName());
        Assertions.assertEquals("lts-26jersey", info.getNextBranchName());
    }
}
