package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;

import java.time.LocalDateTime;
import java.time.ZoneOffset;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class FirmwareRollbackResolverTest {
    private static final String NEW_SHA = "cae1680e40511aa8d798edf60f4cb0c95ce3d841";
    private static final String OLD_SHA = "fa411d09fe1636710229fc7b83a05da7fdd96119";
    private static final String INDEX =
        "<html><body><pre>\n" +
        "<a href=\"?C=N;O=D\">Name</a> Last modified Size\n" +
        "<a href=\"../\">Parent Directory</a> -\n" +
        "<a href=\"" + OLD_SHA + "/\">fa411d09fe1636710229..&gt;</a> 2026-07-23 12:17 -\n" +
        "<a href=\"not-a-build/\">not-a-build/</a> 2026-07-24 20:00 -\n" +
        "<a href=\"" + NEW_SHA + "/\">cae1680e40511aa8d798..&gt;</a> 2026-07-23 19:26 -\n" +
        "<a href=\"0123456789012345678901234567890123456789-extra/\">invalid/</a> 2026-07-25 20:00 -\n" +
        "</pre></body></html>";

    @Test
    void buildsHistoryUrlInsideExistingLtsBranch() {
        FirmwareRollbackResolver resolver = new FirmwareRollbackResolver("https://example.test/build_server/lts");
        assertEquals("https://example.test/build_server/lts/uaefi/lts-25houston/",
            resolver.historyUrl("uaefi", "lts-25houston"));
    }

    @Test
    void parsesFullShaLinksAndSortsByTimestamp() {
        List<FirmwareRollbackResolver.Build> builds = FirmwareRollbackResolver.parseIndex(
            INDEX, "uaefi", "lts-26-test", "https://example.test/build_server/lts/uaefi/lts-26-test/");

        assertEquals(2, builds.size());
        assertEquals(NEW_SHA, builds.get(0).getSha());
        assertEquals(OLD_SHA, builds.get(1).getSha());
        assertEquals("uaefi", builds.get(0).getBoard());
        assertEquals("lts-26-test", builds.get(0).getBranch());
        assertEquals("https://example.test/build_server/lts/uaefi/lts-26-test/" + NEW_SHA + "/",
            builds.get(0).getDirectoryUrl());
        assertEquals(LocalDateTime.of(2026, 7, 23, 19, 26).toInstant(ZoneOffset.UTC).toEpochMilli(),
            builds.get(0).getLastModified());
    }

    @Test
    void selectsLatestAndPreviousFromUnsortedInput() {
        FirmwareRollbackResolver resolver = new FirmwareRollbackResolver("https://example.test/");
        List<FirmwareRollbackResolver.Build> builds = FirmwareRollbackResolver.parseIndex(
            INDEX, "uaefi-121", "lts-26", "https://example.test/build_server/lts/uaefi-121/lts-26");
        List<FirmwareRollbackResolver.Build> unsorted = Arrays.asList(builds.get(1), builds.get(0));

        assertEquals(NEW_SHA, resolver.latest(unsorted).orElseThrow(AssertionError::new).getSha());
        assertEquals(OLD_SHA, resolver.previous(unsorted, NEW_SHA).orElseThrow(AssertionError::new).getSha());
        assertFalse(resolver.previous(unsorted, OLD_SHA).isPresent());
        assertFalse(resolver.previous(unsorted, "0000000000000000000000000000000000000000").isPresent());
        assertEquals(2, resolver.rollbackCandidates(unsorted, null).size());
        assertEquals(2, resolver.rollbackCandidates(unsorted,
            "0000000000000000000000000000000000000000").size());
        assertEquals(1, resolver.rollbackCandidates(unsorted, NEW_SHA).size());
        assertEquals(OLD_SHA, resolver.rollbackCandidates(unsorted, NEW_SHA).get(0).getSha());
    }

    @Test
    void rejectsUnsafeUrlSegments() {
        assertThrows(IllegalArgumentException.class, () -> FirmwareRollbackResolver.parseIndex(
            INDEX, "../uaefi", "lts-26-test", "https://example.test/"));
    }

    @Test
    void unknownCurrentCanExplicitlySelectOnlyBuild() {
        List<FirmwareRollbackResolver.Build> builds = FirmwareRollbackResolver.parseIndex(
            "<a href=\"" + OLD_SHA + "/\">build</a> 2026-07-23 12:17 -",
            "uaefi", "lts-26-test", "https://example.test/");

        FirmwareRollbackResolver resolver = new FirmwareRollbackResolver("https://example.test/");
        assertEquals(1, resolver.rollbackCandidates(builds, null).size());
        assertTrue(resolver.rollbackCandidates(builds, OLD_SHA).isEmpty());
    }
}
