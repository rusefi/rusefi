package com.rusefi.util;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;
import com.rusefi.tune.xml.Msq;
import jakarta.xml.bind.JAXBException;

import java.io.IOException;
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.Clock;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;
import java.util.Optional;
import java.util.stream.Stream;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Tune restore points: dated copies of the tune the user can go back to, kept before each Burn, Load tune and
 * firmware update, or made by hand.
 * <p>
 * Each board has its own folder under {@code ~/.rusEFI/restore_points/}, so points survive firmware updates (which
 * change the signature) but tunes of different boards do not crowd each other out. A file is named
 * {@code 2026-11-23_14-02-05_before-burn.msq}: when and why. The optional note is the tune's comment. A point the
 * same as the newest one is not saved again, and only the newest {@link #DEFAULT_KEEP} of a folder are kept.
 */
public final class RestorePointStore {
    private static final Logging log = getLogging(RestorePointStore.class);
    public static final int DEFAULT_KEEP = 50;
    static final String DIR_PROPERTY = "rusefi.restore_points.dir";
    private static final DateTimeFormatter STAMP = DateTimeFormatter.ofPattern("yyyy-MM-dd_HH-mm-ss", Locale.ROOT);
    private static final String EXTENSION = ".msq";

    public enum Reason {
        BEFORE_BURN("before-burn", "Before burn"),
        BEFORE_LOAD("before-load", "Before Load tune"),
        BEFORE_FIRMWARE_UPDATE("before-update", "Before firmware update"),
        MANUAL("manual", "Manual");

        final String fileTag;
        public final String label;

        Reason(String fileTag, String label) {
            this.fileTag = fileTag;
            this.label = label;
        }

        static Optional<Reason> ofTag(String tag) {
            for (Reason reason : values()) {
                if (reason.fileTag.equals(tag)) {
                    return Optional.of(reason);
                }
            }
            return Optional.empty();
        }
    }

    /** One saved tune: its file, when and why it was saved. */
    public static final class RestorePoint {
        public final Path file;
        public final LocalDateTime when;
        public final Reason reason;
        /** 0, or n for the n-th later point within the same second. */
        final int sequence;

        RestorePoint(Path file, LocalDateTime when, Reason reason, int sequence) {
            this.file = file;
            this.when = when;
            this.reason = reason;
            this.sequence = sequence;
        }

        /** The note given when it was made, or empty; reads the file. */
        public String readNote() {
            try {
                String comment = Msq.readTune(file.toString()).bibliography.getTuneComment();
                return comment == null ? "" : comment;
            } catch (JAXBException | RuntimeException e) {
                return "";
            }
        }

        public Msq readTune() throws JAXBException {
            return Msq.readTune(file.toString());
        }

        @Override
        public String toString() {
            return file.getFileName().toString();
        }
    }

    private final Path root;
    private final int keep;
    private final Clock clock;
    private final java.util.List<Runnable> listeners = new java.util.concurrent.CopyOnWriteArrayList<>();

    public RestorePointStore() {
        this(defaultRoot(), DEFAULT_KEEP, Clock.systemDefaultZone());
    }

    /** {@code ~/.rusEFI/restore_points}, or the {@value #DIR_PROPERTY} system property [tag:restore_points_dir]. */
    static Path defaultRoot() {
        String override = System.getProperty(DIR_PROPERTY);
        return override != null && !override.trim().isEmpty()
                ? Paths.get(override)
                : Paths.get(FileUtil.RUSEFI_SETTINGS_FOLDER, "restore_points");
    }

    /** The store the console uses. */
    public static RestorePointStore shared() {
        return Shared.INSTANCE;
    }

    private static final class Shared {
        static final RestorePointStore INSTANCE = new RestorePointStore();
    }

    /**
     * {@link #save} for the automatic points: a restore point that cannot be written is logged and never stops the
     * Burn, Load tune or update it comes before.
     */
    public Optional<RestorePoint> saveQuietly(Msq tune, Reason reason, String note) {
        try {
            Optional<RestorePoint> point = save(tune, reason, note);
            point.ifPresent(saved -> log.info("Restore point " + saved.file));
            return point;
        } catch (IOException | RuntimeException e) {
            log.error("Failed to save restore point (" + reason.label + "): " + e, e);
            return Optional.empty();
        }
    }

    public RestorePointStore(Path root, int keep, Clock clock) {
        if (keep < 1) {
            throw new IllegalArgumentException("Keep at least one restore point");
        }
        this.root = root;
        this.keep = keep;
        this.clock = clock;
    }

    /** The folder of the board an ECU signature names. */
    public Path folderFor(String signature) {
        return root.resolve(boardOf(signature));
    }

    /**
     * The board in a signature like {@code rusEFI master.2026.11.20.uaefi.3405491532}: the part before the hash.
     * Anything else becomes a file-safe name of the whole signature.
     */
    static String boardOf(String signature) {
        if (signature == null || signature.trim().isEmpty()) {
            return "unknown";
        }
        String[] parts = signature.trim().split("\\.");
        boolean hashLast = parts.length >= 3 && parts[parts.length - 1].matches("\\d+");
        String board = hashLast ? parts[parts.length - 2] : signature.trim();
        String safe = board.replaceAll("[^A-Za-z0-9_-]+", "_");
        return safe.replace("_", "").isEmpty() ? "unknown" : safe;
    }

    /**
     * Saves the tune as a restore point of its board, unless it is the same calibration as the newest one there,
     * then drops the oldest beyond the limit.
     *
     * @param note shown in the list; null or blank for none
     * @return the new point, or empty when the newest point already holds this tune
     */
    public Optional<RestorePoint> save(Msq tune, Reason reason, String note) throws IOException {
        List<RestorePoint> existing = list(folderFor(tune.versionInfo.getSignature()));
        if (!existing.isEmpty() && TuneSnapshot.matches(existing.get(0).file, tune)) {
            return Optional.empty();
        }
        return Optional.of(write(tune, reason, note, existing));
    }

    /** Saves the tune as a restore point even when the newest one holds the same tune: one made by hand. */
    public RestorePoint saveAlways(Msq tune, Reason reason, String note) throws IOException {
        return write(tune, reason, note, list(folderFor(tune.versionInfo.getSignature())));
    }

    private RestorePoint write(Msq tune, Reason reason, String note, List<RestorePoint> existing) throws IOException {
        Path folder = folderFor(tune.versionInfo.getSignature());
        Files.createDirectories(folder);
        LocalDateTime now = LocalDateTime.now(clock).withNano(0);
        String previousComment = tune.bibliography.getTuneComment();
        tune.bibliography.setTuneComment(note == null ? "" : note.trim());
        Path temporary = Files.createTempFile(folder, ".restore_", ".tmp");
        try {
            tune.writeXmlFile(temporary.toString());
            for (int suffix = 0; suffix < 100; suffix++) {
                // two points in one second: the later one gets a suffix, whatever their reasons
                int sequence = suffix;
                if (existing.stream().anyMatch(point -> point.when.equals(now) && point.sequence == sequence)) {
                    continue;
                }
                String stamp = STAMP.format(now) + (suffix == 0 ? "" : "-" + suffix);
                Path file = folder.resolve(stamp + "_" + reason.fileTag + EXTENSION);
                try {
                    Files.copy(temporary, file);
                    prune(folder);
                    fireChanged();
                    return new RestorePoint(file, now, reason, suffix);
                } catch (FileAlreadyExistsException taken) {
                    // try the next suffix
                }
            }
            throw new IOException("No free restore point name at " + STAMP.format(now));
        } catch (JAXBException e) {
            throw new IOException("Failed to write restore point", e);
        } finally {
            tune.bibliography.setTuneComment(previousComment);
            Files.deleteIfExists(temporary);
        }
    }

    /** The board's restore points, newest first; files that are not restore points are left out. */
    public List<RestorePoint> list(String signature) throws IOException {
        return list(folderFor(signature));
    }

    private static List<RestorePoint> list(Path folder) throws IOException {
        List<RestorePoint> points = new ArrayList<>();
        if (!Files.isDirectory(folder)) {
            return points;
        }
        try (Stream<Path> files = Files.list(folder)) {
            files.forEach(file -> parse(file).ifPresent(points::add));
        }
        points.sort(Comparator.comparing((RestorePoint point) -> point.when)
                .thenComparingInt(point -> point.sequence).reversed());
        return points;
    }

    /** A point from its file name, {@code <date>_<time>[-n]_<reason>.msq}. */
    static Optional<RestorePoint> parse(Path file) {
        String name = file.getFileName().toString();
        if (!name.endsWith(EXTENSION) || name.length() < 21) {
            return Optional.empty();
        }
        String stem = name.substring(0, name.length() - EXTENSION.length());
        int reasonStart = stem.indexOf('_', 11);
        if (reasonStart < 0) {
            return Optional.empty();
        }
        Optional<Reason> reason = Reason.ofTag(stem.substring(reasonStart + 1));
        String suffix = stem.substring(19, reasonStart);
        try {
            LocalDateTime when = LocalDateTime.parse(stem.substring(0, 19), STAMP);
            int sequence = suffix.isEmpty() ? 0 : suffix.startsWith("-") ? Integer.parseInt(suffix.substring(1)) : -1;
            if (sequence < 0) {
                return Optional.empty();
            }
            return reason.map(r -> new RestorePoint(file, when, r, sequence));
        } catch (DateTimeParseException | NumberFormatException | StringIndexOutOfBoundsException e) {
            return Optional.empty();
        }
    }

    public void delete(RestorePoint point) throws IOException {
        Files.deleteIfExists(point.file);
        fireChanged();
    }

    /** Told, on the thread that made the change, after a point is saved or deleted; e.g. to refresh a list. */
    public void addListener(Runnable listener) {
        listeners.add(listener);
    }

    public void removeListener(Runnable listener) {
        listeners.remove(listener);
    }

    private void fireChanged() {
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    private void prune(Path folder) throws IOException {
        List<RestorePoint> points = list(folder);
        for (int i = keep; i < points.size(); i++) {
            Files.deleteIfExists(points.get(i).file);
        }
    }
}
