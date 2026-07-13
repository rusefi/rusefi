package com.rusefi.core;

import com.rusefi.core.io.ConnectedEcuTarget;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the flash-time firmware-file selection by board target — the file-picking layer of the
 * brick guard: never grab another board's leftover image from a shared bundle dir, and recover a
 * native image displaced into the {@code older-fw} archive by a foreign download.
 * [tag:better_ux_for_flashing]
 * <p>
 * {@code INPUT_FILES_PATH} is redirected to a JUnit temp dir per test; the {@code older-fw} archive
 * lives under the real ~/.rusEFI, so the test uses a target token unique to this suite
 * ({@code t2tboard}) and deletes every file it creates there.
 */
public class FindFileHelperTargetSelectionTest {

    /** Target token used nowhere outside this suite, so leftover real files can never match. */
    private static final String TARGET = "t2tboard";
    private static final String OLDER_FW_DIR = FileUtil.RUSEFI_SETTINGS_FOLDER + "older-fw";

    @TempDir
    Path inputDir;

    private String savedInputFilesPath;
    private final List<File> createdInOlderFw = new ArrayList<>();

    @BeforeEach
    void redirectInputFilesPath() {
        savedInputFilesPath = FindFileHelper.INPUT_FILES_PATH;
        FindFileHelper.INPUT_FILES_PATH = inputDir.toString();
    }

    @AfterEach
    void restoreInputFilesPath() {
        FindFileHelper.INPUT_FILES_PATH = savedInputFilesPath;
        for (File f : createdInOlderFw) {
            f.delete();
        }
        createdInOlderFw.clear();
    }

    private static String srecName(String target, String sha) {
        return "rusefi_release_2026-07-01_" + target + "_1234567890_" + sha + ".srec";
    }

    private File writeInputFile(String name, long lastModified) throws IOException {
        File f = inputDir.resolve(name).toFile();
        Files.write(f.toPath(), new byte[]{1});
        assertTrue(f.setLastModified(lastModified));
        return f;
    }

    @Test
    public void newestMatchingFileWins() throws IOException {
        writeInputFile(srecName(TARGET, "aaaa"), 1_000_000_000_000L);
        File newer = writeInputFile(srecName(TARGET, "bbbb"), 1_000_000_002_000L);

        assertEquals(newer.getAbsolutePath(), FindFileHelper.findSrecFileForTarget(TARGET));
    }

    @Test
    public void relatedBoardNameIsNotGoodEnough() throws IOException {
        // "t2tboard" must NOT pick a "t2tboard_pro" image — that is the #1 brick scenario in a shared dir
        writeInputFile(srecName(TARGET + "_pro", "aaaa"), 1_000_000_000_000L);

        assertNull(FindFileHelper.findSrecFileForTarget(TARGET));
    }

    @Test
    public void exactTargetIsPickedOutOfMixedDirectory() throws IOException {
        writeInputFile(srecName(TARGET + "_pro", "aaaa"), 1_000_000_009_000L);
        File exact = writeInputFile(srecName(TARGET, "bbbb"), 1_000_000_000_000L);

        // the exact match wins even though the foreign image is newer
        assertEquals(exact.getAbsolutePath(), FindFileHelper.findSrecFileForTarget(TARGET));
    }

    @Test
    public void imageDisplacedIntoOlderFwArchiveIsRecovered() throws IOException {
        // A foreign download displaces the native image into ~/.rusEFI/older-fw; flashing the native
        // board later must recover it from there rather than reporting "no image".
        File archiveDir = new File(OLDER_FW_DIR);
        assertTrue(archiveDir.isDirectory() || archiveDir.mkdirs());
        File archived = new File(archiveDir, srecName(TARGET, "cccc"));
        Files.write(archived.toPath(), new byte[]{1});
        createdInOlderFw.add(archived);

        assertEquals(archived.getAbsolutePath(), FindFileHelper.findSrecFileForTarget(TARGET));
    }

    @Test
    public void binVariantUsesTheSameTargetSelection() throws IOException {
        File bin = writeInputFile("rusefi_release_2026-07-01_" + TARGET + "_1234567890_dddd.bin",
            1_000_000_000_000L);
        writeInputFile(srecName(TARGET, "eeee"), 1_000_000_000_000L);

        assertEquals(bin.getAbsolutePath(), FindFileHelper.findFirmwareFileForTarget(TARGET));
    }

    @Test
    public void nullBlankOrUnmatchedTargetResolvesToNull() throws IOException {
        writeInputFile(srecName(TARGET, "aaaa"), 1_000_000_000_000L);

        assertNull(FindFileHelper.findSrecFileForTarget(null));
        assertNull(FindFileHelper.findSrecFileForTarget("  "));
        assertNull(FindFileHelper.findSrecFileForTarget("someotherboard"));
    }

    /**
     * "Secure: decode error 2": a plain local build sits next to the console while a stale
     * public-obfuscated image for the same board is in older-fw. The connected-board pick must
     * prefer the local sibling (right key) over the archived obfuscated one (wrong key), even though the
     * archived name parses to the connected target and the local one does not.
     */
    @Test
    public void localSiblingWinsOverArchivedObfuscatedImage() throws IOException {
        File localPlain = writeInputFile("rusefi.srec", 1_000_000_000_000L); // dev/local build, no parseable target

        File archiveDir = new File(OLDER_FW_DIR);
        assertTrue(archiveDir.isDirectory() || archiveDir.mkdirs());
        File archivedObfuscated = new File(archiveDir,
            "rusefi-development_2026-07-06_" + TARGET + "_99386034_deadbeef_obfuscated.srec");
        Files.write(archivedObfuscated.toPath(), new byte[]{1});
        createdInOlderFw.add(archivedObfuscated);

        ConnectedEcuTarget connected = new ConnectedEcuTarget();
        connected.setConnectedTargetForUnitTest(TARGET);

        assertEquals(localPlain.getAbsolutePath(), FindFileHelper.findSrecFileForConnectedBoard(connected));
    }

    /**
     * The brick-guard still holds: a local sibling whose name encodes a *different* board is not used;
     * the exact-target image recovered from older-fw wins instead.
     */
    @Test
    public void foreignNamedLocalSiblingIsNotUsedForConnectedBoard() throws IOException {
        writeInputFile(srecName(TARGET + "_pro", "aaaa"), 1_000_000_000_000L); // foreign board next to console

        File archiveDir = new File(OLDER_FW_DIR);
        assertTrue(archiveDir.isDirectory() || archiveDir.mkdirs());
        File archived = new File(archiveDir, srecName(TARGET, "cccc"));
        Files.write(archived.toPath(), new byte[]{1});
        createdInOlderFw.add(archived);

        ConnectedEcuTarget connected = new ConnectedEcuTarget();
        connected.setConnectedTargetForUnitTest(TARGET);

        assertEquals(archived.getAbsolutePath(), FindFileHelper.findSrecFileForConnectedBoard(connected));
    }
}
