package com.rusefi.core.io;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class FirmwareBranchTest {
    @Test
    void normalizesAndRestrictsAutomaticBranchPolicy() {
        assertEquals("development", FirmwareBranch.normalize("master"));
        assertEquals("development", FirmwareBranch.normalize("development"));
        assertEquals("lts-26lima", FirmwareBranch.normalize("lts-26lima"));
        assertTrue(FirmwareBranch.isSameBranch("master", "development"));
        assertTrue(FirmwareBranch.isSameBranch("lts-25kansas", "lts-25kansas"));
        assertFalse(FirmwareBranch.isSameBranch("lts-25kansas", "development"));
        assertFalse(FirmwareBranch.isSameBranch("unknown", "unknown"));
        String[] branches = {"master", "development", "lts-25kansas", "lts-26lima", "lts-26lima-dev", "unknown"};
        for (String from : branches) {
            for (String to : branches) {
                String source = FirmwareBranch.normalize(from);
                String destination = FirmwareBranch.normalize(to);
                boolean expected = ("lts-26lima".equals(source)
                    && ("development".equals(destination) || "lts-26lima-dev".equals(destination)))
                    || ("lts-26lima-dev".equals(source) && "development".equals(destination));
                assertEquals(expected, FirmwareBranch.canSwitch(from, to), from + " -> " + to);
            }
        }
    }
}
