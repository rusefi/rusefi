package com.rusefi.util;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;
import com.rusefi.core.SensorCentral;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
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
import java.util.OptionalLong;
import java.util.function.Consumer;
import java.util.function.Supplier;
import java.util.stream.Stream;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;

/**
 * Tune restore points: dated copies of the tune the user can go back to, kept before each Burn, Load tune and
 * firmware update, or made by hand.
 * <p>
 * The points are kept in {@code ~/.rusEFI/restore_points}, or a folder the user chose (e.g. one synced between PCs).
 * Each car has its own folder there, {@code <board>/<car>/}, so points survive firmware updates
 * (which change the signature) and two cars on the same board neither share a list nor crowd each other out. The car
 * is the tune's Vehicle Name or, when that is blank, the ECU's serial number ({@code ecu-<serial>}); with neither,
 * points go in the board folder itself. A file is named {@code 2026-11-23_14-02-05_before-burn.msq}: when and why.
 * The optional note is the tune's comment. A point the same as the newest one is not saved again, and only the newest
 * {@link #DEFAULT_KEEP} of a folder are kept.
 */
public final class RestorePointStore {
    private static final Logging log = getLogging(RestorePointStore.class);
    public static final int DEFAULT_KEEP = 50;
    static final String DIR_PROPERTY = "rusefi.restore_points.dir";
    /** The console setting holding the folder chosen in Restore Points; blank for the default. */
    static final String ROOT_SETTING = "restore_points_folder";
    public static final String VEHICLE_NAME = "vehicleName";
    private static final String MCU_SERIAL = "mcuSerial";
    private static final String SERIAL_PREFIX = "ecu-";
    /** Characters Windows does not allow in a folder name. */
    private static final String NOT_IN_FOLDER_NAME = "[\\\\/:*?\"<>|\\p{Cntrl}]+";
    private static final String RESERVED_FOLDER_NAME = "(?i)(con|prn|aux|nul|com\\d|lpt\\d)";
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

    private final Path defaultRoot;
    private volatile Path root;
    /** Remembers the folder chosen with {@link #setRoot}; null for the default. */
    private final Consumer<Path> rootSaver;
    private final int keep;
    private final Clock clock;
    private final java.util.List<Runnable> listeners = new java.util.concurrent.CopyOnWriteArrayList<>();
    private volatile Supplier<OptionalLong> serialSource = RestorePointStore::connectedEcuSerial;

    /** {@code ~/.rusEFI/restore_points}, or the {@value #DIR_PROPERTY} system property [tag:restore_points_dir]. */
    static Path defaultRoot() {
        String override = testRoot();
        return override != null ? Paths.get(override) : Paths.get(FileUtil.RUSEFI_SETTINGS_FOLDER, "restore_points");
    }

    private static String testRoot() {
        String override = System.getProperty(DIR_PROPERTY);
        return override != null && !override.trim().isEmpty() ? override : null;
    }

    /** The store the console uses: in the folder chosen in Restore Points, else {@link #defaultRoot()}. */
    public static RestorePointStore shared() {
        return Shared.INSTANCE;
    }

    private static final class Shared {
        static final RestorePointStore INSTANCE = create();

        private static RestorePointStore create() {
            if (testRoot() != null) {
                // test runs never read or change the console's own settings
                return new RestorePointStore(defaultRoot(), null, path -> { }, DEFAULT_KEEP,
                    Clock.systemDefaultZone());
            }
            String chosen = getConfig().getRoot().getProperty(ROOT_SETTING, "");
            return new RestorePointStore(defaultRoot(), chosen.trim().isEmpty() ? null : Paths.get(chosen.trim()),
                path -> {
                    getConfig().getRoot().setProperty(ROOT_SETTING, path == null ? "" : path.toString());
                    getConfig().save();
                }, DEFAULT_KEEP, Clock.systemDefaultZone());
        }
    }

    /** Where the points are kept: the chosen folder or the default; cars' board folders are inside. */
    public Path getRoot() {
        return root;
    }

    public Path getDefaultRoot() {
        return defaultRoot;
    }

    public boolean isDefaultRoot() {
        return root.equals(defaultRoot);
    }

    /**
     * Keeps the points in another folder from now on, e.g. one synced between PCs, and remembers it; null goes back to
     * the default. Points already kept stay where they are: see {@link #movePoints}.
     */
    public void setRoot(Path folder) throws IOException {
        Path next = folder == null ? defaultRoot : folder.toAbsolutePath().normalize();
        Files.createDirectories(next);
        if (!Files.isWritable(next)) {
            throw new IOException(next + " cannot be written to");
        }
        root = next;
        rootSaver.accept(next.equals(defaultRoot) ? null : next);
        fireChanged();
    }

    /** The restore points under a root folder: in its board folders and their cars' folders. */
    public static List<RestorePoint> allPoints(Path root) throws IOException {
        List<RestorePoint> points = new ArrayList<>();
        if (!Files.isDirectory(root)) {
            return points;
        }
        try (Stream<Path> files = Files.walk(root, 3)) {
            files.filter(Files::isRegularFile)
                    .filter(file -> root.relativize(file).getNameCount() >= 2)
                    .forEach(file -> parse(file).ifPresent(points::add));
        }
        return points;
    }

    /**
     * Moves the restore points under {@code from} to the same board and car folders under {@code to}; a file the
     * new folder already has (the same point, synced from another PC) is left in the old one. Other files stay.
     *
     * Tries them all: one that cannot be moved (e.g. a file another program has open, on Windows) stays in the old
     * folder, and the move goes on.
     */
    public static MoveResult movePoints(Path from, Path to) throws IOException {
        Path source = from.toAbsolutePath().normalize();
        Path target = to.toAbsolutePath().normalize();
        int moved = 0;
        List<Path> notMoved = new ArrayList<>();
        IOException firstFailure = null;
        for (Path file : movable(source, target)) {
            Path destination = target.resolve(source.relativize(file).toString());
            if (Files.exists(destination)) {
                continue;
            }
            try {
                Files.createDirectories(destination.getParent());
                Files.move(file, destination);
                moved++;
            } catch (IOException e) {
                notMoved.add(file);
                if (firstFailure == null) {
                    firstFailure = e;
                }
            }
        }
        return new MoveResult(moved, notMoved, firstFailure);
    }

    /** What {@link #movePoints} did. */
    public static final class MoveResult {
        public final int moved;
        /** The points left in the old folder because moving them failed. */
        public final List<Path> notMoved;
        /** Why the first of {@link #notMoved} failed, for the log; null when all were moved. */
        public final IOException firstFailure;

        MoveResult(int moved, List<Path> notMoved, IOException firstFailure) {
            this.moved = moved;
            this.notMoved = notMoved;
            this.firstFailure = firstFailure;
        }
    }

    /** How many restore points {@link #movePoints} would look at: those under {@code from} but not under {@code to}. */
    public static int countMovable(Path from, Path to) throws IOException {
        return movable(from.toAbsolutePath().normalize(), to.toAbsolutePath().normalize()).size();
    }

    private static List<Path> movable(Path source, Path target) throws IOException {
        List<Path> files = new ArrayList<>();
        if (source.equals(target)) {
            return files;
        }
        for (RestorePoint point : allPoints(source)) {
            Path file = point.file.toAbsolutePath().normalize();
            // when the new folder is inside the old one, its points are there already
            if (!file.startsWith(target)) {
                files.add(file);
            }
        }
        return files;
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

    /** A store in {@code root}; a folder chosen with {@link #setRoot} is not remembered. */
    public RestorePointStore(Path root, int keep, Clock clock) {
        this(root, null, path -> { }, keep, clock);
    }

    /**
     * @param chosen    the folder chosen earlier, or null for {@code defaultRoot}
     * @param rootSaver remembers a newly chosen folder; null for the default
     */
    RestorePointStore(Path defaultRoot, Path chosen, Consumer<Path> rootSaver, int keep, Clock clock) {
        if (keep < 1) {
            throw new IllegalArgumentException("Keep at least one restore point");
        }
        this.defaultRoot = defaultRoot.toAbsolutePath().normalize();
        this.root = chosen == null ? this.defaultRoot : chosen.toAbsolutePath().normalize();
        this.rootSaver = rootSaver;
        this.keep = keep;
        this.clock = clock;
    }

    /** The folder of the board an ECU signature names; points of a car with no name and no serial go here. */
    public Path folderFor(String signature) {
        return root.resolve(boardOf(signature));
    }

    /** The folder of one car: by its Vehicle Name, else by the serial of the ECU connected now. */
    public Path folderFor(String signature, String vehicleName) {
        Path board = folderFor(signature);
        return carOf(vehicleName, serialSource.get()).map(board::resolve).orElse(board);
    }

    /** The folder the tune's points go in: by its board and its own Vehicle Name (or the ECU's serial). */
    public Path folderFor(Msq tune) {
        return folderFor(tune.versionInfo.getSignature(), vehicleNameOf(tune));
    }

    /** True when the Vehicle Name is blank, or nothing of it can name a folder: the car goes by its ECU's serial. */
    public static boolean isUnnamed(String vehicleName) {
        return !carOf(vehicleName, OptionalLong.empty()).isPresent();
    }

    /**
     * The car's folder name: the Vehicle Name made file-safe, else {@code ecu-<serial>}; empty when there is neither
     * (a serial of 0 is one the ECU did not report).
     */
    static Optional<String> carOf(String vehicleName, OptionalLong serial) {
        String name = vehicleName == null ? "" : vehicleName.trim();
        name = name.replaceAll(NOT_IN_FOLDER_NAME, "_").replaceAll("[. ]+$", "");
        if (!name.replace("_", "").isEmpty() && !name.matches(RESERVED_FOLDER_NAME)) {
            return Optional.of(name);
        }
        if (serial.isPresent() && serial.getAsLong() != 0) {
            return Optional.of(SERIAL_PREFIX + Long.toUnsignedString(serial.getAsLong()));
        }
        return Optional.empty();
    }

    /** The Vehicle Name saved in a tune, or empty. */
    public static String vehicleNameOf(Msq tune) {
        Page page = tune.findPage();
        if (page == null || page.constant == null) {
            return "";
        }
        for (Constant constant : page.constant) {
            if (VEHICLE_NAME.equals(constant.getName())) {
                String value = constant.getValue();
                return value == null ? "" : value.replace("\"", "").trim();
            }
        }
        return "";
    }

    /** The {@value #MCU_SERIAL} output channel of the connected ECU, when it has reported one. */
    private static OptionalLong connectedEcuSerial() {
        double serial = SensorCentral.getInstance().getValue(MCU_SERIAL);
        return Double.isNaN(serial) || serial == 0 ? OptionalLong.empty() : OptionalLong.of((long) serial);
    }

    /** Where the ECU serial comes from, for tests. */
    public void setSerialSourceForTests(Supplier<OptionalLong> serialSource) {
        this.serialSource = serialSource;
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
        return saveInto(folderFor(tune), tune, reason, note);
    }

    /**
     * {@link #save(Msq, Reason, String)} into the named car's folder rather than the tune's own: the tune before a
     * Burn that sets the Vehicle Name has none yet, but belongs with the car's later points.
     */
    public Optional<RestorePoint> save(Msq tune, String vehicleName, Reason reason, String note) throws IOException {
        return saveInto(folderFor(tune.versionInfo.getSignature(), vehicleName), tune, reason, note);
    }

    private Optional<RestorePoint> saveInto(Path folder, Msq tune, Reason reason, String note) throws IOException {
        List<RestorePoint> existing = list(folder);
        if (!existing.isEmpty() && TuneSnapshot.matches(existing.get(0).file, tune)) {
            return Optional.empty();
        }
        return Optional.of(write(folder, tune, reason, note, existing));
    }

    /** Saves the tune as a restore point even when the newest one holds the same tune: one made by hand. */
    public RestorePoint saveAlways(Msq tune, Reason reason, String note) throws IOException {
        Path folder = folderFor(tune);
        return write(folder, tune, reason, note, list(folder));
    }

    private RestorePoint write(Path folder, Msq tune, Reason reason, String note, List<RestorePoint> existing)
            throws IOException {
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

    /**
     * The folders of the board's cars that hold restore points, by name; the board folder itself (cars without a name
     * or serial, and points from before cars had folders) first when it holds any.
     */
    public List<Path> carFolders(String signature) throws IOException {
        Path board = folderFor(signature);
        List<Path> folders = new ArrayList<>();
        if (!Files.isDirectory(board)) {
            return folders;
        }
        if (!list(board).isEmpty()) {
            folders.add(board);
        }
        List<Path> cars = new ArrayList<>();
        try (Stream<Path> children = Files.list(board)) {
            children.filter(Files::isDirectory).forEach(cars::add);
        }
        cars.sort(Comparator.comparing(folder -> folder.getFileName().toString().toLowerCase(Locale.ROOT)));
        for (Path car : cars) {
            if (!list(car).isEmpty()) {
                folders.add(car);
            }
        }
        return folders;
    }

    /** The car's restore points, newest first; see {@link #folderFor(String, String)}. */
    public List<RestorePoint> list(String signature, String vehicleName) throws IOException {
        return list(folderFor(signature, vehicleName));
    }

    /** The folder's restore points, newest first; other files and the cars' sub-folders are left out. */
    public static List<RestorePoint> list(Path folder) throws IOException {
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
