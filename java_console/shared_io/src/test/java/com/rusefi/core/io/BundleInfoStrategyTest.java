package com.rusefi.core.io;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class BundleInfoStrategyTest {
    @Test
    void developmentConsoleUpdatesStayIndependentOfEcuFirmwareChannels() {
        BundleInfo console = new BundleInfo("development", "lts-26lima", "all");
        String base = "https://example.test";
        String softwareUrl = BundleInfoStrategy.getDownloadUrl(console, base,
            ignored -> fail("A development Console must not switch software channel"));
        for (String ecuBranch : new String[]{"lts-26lima", "lts-26lima-dev"}) {
            BundleInfo firmware = new BundleInfo(ecuBranch, null, "test_board");
            String firmwareUrl = BundleInfoStrategy.getDownloadUrl(firmware, base, BundleInfo::getBranchName);
            assertTrue(firmwareUrl.contains("/lts/" + ecuBranch + "/"));
            assertNotEquals(softwareUrl, firmwareUrl);
            assertEquals("development", console.getBranchName());
        }
    }
}
