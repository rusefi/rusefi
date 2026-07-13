package com.rusefi.core;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.io.IOException;
import java.nio.file.*;
import java.util.List;
import java.util.Optional;
import java.util.function.Supplier;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import java.util.Arrays;

import static com.rusefi.core.FileUtil.RUSEFI_SETTINGS_FOLDER;

public class FindFileHelper {
    private static final Logging log = Logging.getLogging(FindFileHelper.class);
    public static String INPUT_FILES_PATH = System.getProperty("input_files_path", "..");
    /**
     * Same .bin used by primary DFU and a bit unneeded ST-LINK options
     */
    public static final String FIRMWARE_BIN_FILE = INPUT_FILES_PATH + "/" + "rusefi.bin";
    private static final String PREFIX = "";
    private static final String SUFFIX = "srec";

    @Nullable
    public static String findFile(String fileDirectory, String prefix, String suffix, AdditionalFileHandler fileMoveOperation, boolean keepOneFile) {
        File dir = new File(fileDirectory);
        log.info("Scanning " + dir.getAbsolutePath() + " for " + prefix + "/" + suffix);
        if (!dir.isDirectory()) {
            throw new IllegalStateException("Not a directory: " + fileDirectory);
        }

        log.info("Searching for " + prefix + "*" + suffix + " in " + dir.getAbsolutePath());

        return findXNumberOfFile(fileDirectory, prefix, suffix, dir.list(), fileMoveOperation, keepOneFile);
    }

    public interface AdditionalFileHandler {
        void onAdditionalFile(String fileDirectory, String fileName);
    }

    @Nullable
    public static String findXNumberOfFile(String fileDirectory, String prefix, String suffix, String[] directoryListing, AdditionalFileHandler fileMoveOperation, boolean keepOneFile) {
        Optional<String[]> fileList = Optional.ofNullable(directoryListing);

        Supplier<Stream<String>> search_file = () -> Arrays.stream(fileList.get()).filter(file -> !file.contains(" "))  // spaces not acceptable
            .filter(file -> file.startsWith(prefix) && file.endsWith(suffix));

        List<String> files = search_file.get().collect(Collectors.toList());
        if (files.isEmpty())
            return null;


        for (int index = keepOneFile ? 1 : 0; index < files.size(); index++) {
            fileMoveOperation.onAdditionalFile(fileDirectory, files.get(index));
        }
        if (!keepOneFile)
            return null;

        String file = files.get(0);

        String result = fileDirectory + File.separator + file;
        log.info("Located " + result);
        return result;
    }

    @Nullable
    public static String findSrecFile() {
        return findSrecFile(true);
    }

    @Nullable
    public static String findSrecFile(boolean keepOneFile) {
        String fileAtFirstLocation = findFile(INPUT_FILES_PATH, PREFIX, SUFFIX, new AdditionalFileHandler() {
            @Override
            public void onAdditionalFile(String fileDirectory, String fileName) {
                moveFile(fileDirectory, fileName);
            }
        }, keepOneFile);
        if (fileAtFirstLocation == null) {
            // todo: what is this second location about?!
            log.info("Second choice: current folder");
            return findFile(".", PREFIX, SUFFIX, new AdditionalFileHandler() {
                @Override
                public void onAdditionalFile(String fileDirectory, String fileName) {
                    moveFile(fileDirectory, fileName);
                }
            }, keepOneFile);
        }

        return fileAtFirstLocation;
    }

    private static void moveFile(String fileDirectory, String fileName) {
        String destinationFolder = RUSEFI_SETTINGS_FOLDER + "older-fw" + File.separator;
        boolean wasCreated = new File(destinationFolder).mkdirs();
        log.info(destinationFolder + ": created: " + wasCreated);
        String fullSourceFileName = fileDirectory + File.separator + fileName;
        Path sourcePath = Paths.get(fullSourceFileName);
        Path destinationDirectory = Paths.get(destinationFolder + fileName);
        try {
            Files.move(sourcePath, destinationDirectory);
            log.info("Moved " + sourcePath + " to " + destinationFolder);
        } catch (IOException e) {
            // this could happen when we have too many plainly named older files like rusefi.srec for many different boards
            log.warn(sourcePath + "Move failed, will try to remove: " + e);
            boolean deletedOk = new File(fullSourceFileName).delete();
            log.info(fullSourceFileName + ": deleted: " + deletedOk);
        }
    }

    /**
     * (rusefi_&lt;branch&gt;_&lt;date&gt;_&lt;board&gt;_&lt;sig&gt;_&lt;sha&gt;.bin)
     * Prefix "rusefi_" + suffix ".bin" uniquely selects it:
     * <p>
     * "openblt.bin" has the wrong prefix, and
     * "rusefi-obfuscated.bin" uses a hyphen rather than an underscore.
     * <p>
     * Dev (non-bundle) builds still drop a plain rusefi.bin next to the console, so fall back to that.
     */
    public static String findFirmwareFile() {
        String globbed = findFile(INPUT_FILES_PATH, "rusefi_", ".bin", (fileDirectory, fileName) -> {
        }, true);
        return globbed != null ? globbed : FIRMWARE_BIN_FILE;
    }

    public static boolean isObfuscated() {
        String srecFile = findSrecFile();
        return srecFile != null && srecFile.contains("obfuscated");
    }

    /**
     * Extract the board target from a firmware artifact file name, e.g.
     * {@code rusefi_development_2026-07-06_uaefi_pro_2528425206_<sha>_update.srec} -> {@code uaefi_pro}.
     * The target is the token(s) between the {@code yyyy-MM-dd} date and the numeric signature. Returns
     * null for names that don't follow this pattern (e.g. a plain dev {@code rusefi.srec}). [tag:better_ux_for_flashing]
     */
    public static String extractTargetFromFirmwareName(String path) {
        if (path == null) {
            return null;
        }
        final String name = new File(path).getName();
        final String[] parts = name.split("_");
        int dateIdx = -1;
        for (int i = 0; i < parts.length; i++) {
            if (parts[i].matches("\\d{4}-\\d{2}-\\d{2}")) {
                dateIdx = i;
                break;
            }
        }
        if (dateIdx < 0) {
            return null;
        }
        final StringBuilder target = new StringBuilder();
        boolean foundSignature = false;
        for (int i = dateIdx + 1; i < parts.length; i++) {
            if (parts[i].matches("\\d+")) {
                foundSignature = true;
                break; // reached the numeric signature — target tokens are done
            }
            if (target.length() > 0) {
                target.append('_');
            }
            target.append(parts[i]);
        }
        // No numeric signature after the date (e.g. obfuscated builds, or an unrelated name) means there
        // is no parseable target — return null so callers fall back / don't false-alarm.
        if (!foundSignature || target.length() == 0) {
            return null;
        }
        return target.toString();
    }

    /**
     * Pick the {@code .srec} matching the currently-connected board, falling back to the generic
     * single-file pick for dev builds / single-board bundles. This is the flash-time entry point that
     * prevents grabbing another board's leftover image from a shared bundle dir. [tag:better_ux_for_flashing]
     */
    public static String findSrecFileForConnectedBoard(com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget) {
        return findForConnectedBoard(connectedEcuTarget.effectiveTarget(), ".srec", findSrecFile());
    }

    /** @see #findSrecFileForConnectedBoard — same, for the {@code .bin} DFU image. */
    public static String findFirmwareFileForConnectedBoard(com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget) {
        return findForConnectedBoard(connectedEcuTarget.effectiveTarget(), ".bin", findFirmwareFile());
    }

    /**
     * Flash-time file priority for the connected board:
     * <ol>
     *   <li>exact target match sitting next to the console — the multi-board-bundle brick-guard;</li>
     *   <li>the local single build ({@code sibling}) when it is safe (its name has no parseable target,
     *       i.e. a dev/local build, or the target matches) — preferred over the archive so we never
     *       substitute a wrong-key {@code obfuscated_public} image left in {@code older-fw} by an earlier
     *       public download;</li>
     *   <li>a native image displaced into {@code older-fw} by a foreign download (recovered, not lost);</li>
     *   <li>last resort: whatever single file is next to the console.</li>
     * </ol>
     * [tag:better_ux_for_flashing]
     */
    private static String findForConnectedBoard(String target, String suffix, String sibling) {
        final String localMatch = newestMatchingTarget(INPUT_FILES_PATH, target, suffix);
        if (localMatch != null) {
            return localMatch;
        }
        if (isSafeLocalSibling(sibling, target)) {
            return sibling;
        }
        final String archived = newestMatchingTarget(RUSEFI_SETTINGS_FOLDER + "older-fw", target, suffix);
        if (archived != null) {
            return archived;
        }
        return sibling;
    }

    /**
     * True when {@code sibling} is safe to flash onto {@code target} without a name-based board match:
     * a dev/local build whose name carries no parseable target, or one whose target matches. A file whose
     * name encodes a *different* board is not safe (that is the brick scenario the target match guards).
     */
    private static boolean isSafeLocalSibling(String sibling, String target) {
        if (sibling == null) {
            return false;
        }
        final String siblingTarget = extractTargetFromFirmwareName(sibling);
        return siblingTarget == null || (target != null && siblingTarget.equalsIgnoreCase(target));
    }

    /**
     * Newest {@code .srec} whose embedded target matches {@code target}, searching the bundle input dir
     * first, then the {@code older-fw} archive (so a native image displaced there by a foreign download is
     * recovered rather than lost). Null if none matches. [tag:better_ux_for_flashing]
     */
    public static String findSrecFileForTarget(String target) {
        return findFirmwareForTarget(target, ".srec");
    }

    public static String findFirmwareFileForTarget(String target) {
        return findFirmwareForTarget(target, ".bin");
    }

    private static String findFirmwareForTarget(String target, String suffix) {
        if (target == null || target.trim().isEmpty()) {
            return null;
        }
        for (String dir : new String[]{INPUT_FILES_PATH, RUSEFI_SETTINGS_FOLDER + "older-fw"}) {
            final String match = newestMatchingTarget(dir, target, suffix);
            if (match != null) {
                return match;
            }
        }
        return null;
    }

    private static String newestMatchingTarget(String dir, String target, String suffix) {
        final File[] files = new File(dir).listFiles((d, name) -> name.startsWith("rusefi") && name.endsWith(suffix));
        if (files == null) {
            return null;
        }
        File best = null;
        for (File f : files) {
            final String t = extractTargetFromFirmwareName(f.getName());
            if (t != null && t.equalsIgnoreCase(target) && (best == null || f.lastModified() > best.lastModified())) {
                best = f;
            }
        }
        return best == null ? null : best.getAbsolutePath();
    }
}
