package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.LogFiles;

import java.io.File;
import java.time.Instant;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * The newest data logs: those in the rusEFI logs folder (where Binary Logging records) and those opened from
 * elsewhere, which are remembered in the console config. The duration of every opened log is cached there
 * too, keyed by path and checked against the file's size and time, so the list can show it without reading
 * the files.
 */
public final class RecentLogs {
    static final int LIMIT = 10;
    /** Config key of the logs opened from other folders, newest first, separated by {@link #SEPARATOR}. */
    static final String KEY = "recent_logs";
    private static final String SEPARATOR = "|";
    /** The date and time Binary Logging puts in its file names, e.g. 2026-10-21_19_44_54. */
    private static final Pattern NAME_DATE = Pattern.compile("(\\d{4})-(\\d{2})-(\\d{2})_(\\d{2})_(\\d{2})_(\\d{2})");
    private static final DateTimeFormatter TIME = DateTimeFormatter.ofPattern("HH:mm", Locale.ENGLISH);
    private static final DateTimeFormatter DATE = DateTimeFormatter.ofPattern("d MMM yyyy", Locale.ENGLISH);

    private RecentLogs() {
    }

    static final class Entry {
        final File file;
        /** When the log was recorded: from the file name when it has a date, otherwise the file time. */
        final LocalDateTime recorded;
        final long size;
        /** Seconds from the first to the last sample, or NaN until the log has been opened once. */
        final double duration;

        Entry(File file, LocalDateTime recorded, long size, double duration) {
            this.file = file;
            this.recorded = recorded;
            this.size = size;
            this.duration = duration;
        }
    }

    /** Up to {@link #LIMIT} logs from the folder and the remembered files, newest first; missing files are skipped. */
    static List<Entry> find(File folder, List<File> remembered) {
        return find(folder, remembered, new Node());
    }

    /** As {@link #find(File, List)}, with the durations cached in {@code durations}. */
    static List<Entry> find(File folder, List<File> remembered, Node durations) {
        Map<String, File> files = new LinkedHashMap<>();
        File[] inFolder = folder.listFiles((dir, name) -> LogFiles.isSupported(name));
        if (inFolder != null) {
            for (File file : inFolder) {
                files.put(file.getAbsolutePath(), file);
            }
        }
        for (File file : remembered) {
            files.putIfAbsent(file.getAbsolutePath(), file);
        }
        List<Entry> entries = new ArrayList<>();
        for (File file : files.values()) {
            if (file.isFile()) {
                entries.add(new Entry(file.getAbsoluteFile(), recorded(file), file.length(), duration(durations, file)));
            }
        }
        entries.sort(Comparator.comparing((Entry entry) -> entry.recorded).reversed());
        return entries.size() > LIMIT ? new ArrayList<>(entries.subList(0, LIMIT)) : entries;
    }

    static LocalDateTime recorded(File file) {
        Matcher matcher = NAME_DATE.matcher(file.getName());
        if (matcher.find()) {
            try {
                return LocalDateTime.of(Integer.parseInt(matcher.group(1)), Integer.parseInt(matcher.group(2)),
                        Integer.parseInt(matcher.group(3)), Integer.parseInt(matcher.group(4)),
                        Integer.parseInt(matcher.group(5)), Integer.parseInt(matcher.group(6)));
            } catch (RuntimeException invalidDate) {
                // fall back to the file time
            }
        }
        return LocalDateTime.ofInstant(Instant.ofEpochMilli(file.lastModified()), ZoneId.systemDefault());
    }

    /** "Today 19:44", "Yesterday 21:15", otherwise "18 Oct 2026". */
    static String when(LocalDateTime recorded, LocalDate today) {
        LocalDate day = recorded.toLocalDate();
        if (day.equals(today)) {
            return "Today " + TIME.format(recorded);
        }
        if (day.equals(today.minusDays(1))) {
            return "Yesterday " + TIME.format(recorded);
        }
        return DATE.format(recorded);
    }

    /** "17.0 s", "2 min 15 s", "1 h 05 min"; empty when unknown. */
    public static String duration(double seconds) {
        if (!(seconds >= 0) || !Double.isFinite(seconds)) {
            return "";
        }
        if (seconds < 60) {
            return String.format(Locale.ROOT, "%.1f s", seconds);
        }
        long whole = Math.round(seconds);
        if (whole < 3600) {
            return String.format(Locale.ROOT, "%d min %02d s", whole / 60, whole % 60);
        }
        return String.format(Locale.ROOT, "%d h %02d min", whole / 3600, (whole % 3600) / 60);
    }

    /** Caches a log's duration under its path, with the file's size and time to detect a changed file. */
    static void rememberDuration(Node durations, File file, double seconds) {
        if (Double.isFinite(seconds)) {
            durations.setProperty(file.getAbsolutePath(),
                    file.length() + ";" + file.lastModified() + ";" + seconds);
        }
    }

    /** The cached duration, or NaN when unknown or the file has changed since. */
    static double duration(Node durations, File file) {
        String[] cached = durations.getProperty(file.getAbsolutePath(), "").split(";");
        if (cached.length != 3) {
            return Double.NaN;
        }
        try {
            if (Long.parseLong(cached[0]) != file.length() || Long.parseLong(cached[1]) != file.lastModified()) {
                return Double.NaN;
            }
            return Double.parseDouble(cached[2]);
        } catch (NumberFormatException corrupt) {
            return Double.NaN;
        }
    }

    /** Keeps the cached durations of the listed logs only, so the cache stays as small as the list. */
    static void pruneDurations(Node durations, List<Entry> entries) {
        Set<String> keep = new HashSet<>();
        for (Entry entry : entries) {
            keep.add(entry.file.getAbsolutePath());
        }
        durations.getConfig().keySet().retainAll(keep);
    }

    public static String size(long bytes) {
        if (bytes < 1024) {
            return bytes + " B";
        }
        if (bytes < 1024 * 1024) {
            return Math.round(bytes / 1024.0) + " KB";
        }
        return String.format(Locale.ROOT, "%.1f MB", bytes / (1024.0 * 1024));
    }

    static List<File> remembered(Node settings) {
        List<File> files = new ArrayList<>();
        for (String path : settings.getProperty(KEY, "").split(Pattern.quote(SEPARATOR))) {
            if (!path.isEmpty()) {
                files.add(new File(path));
            }
        }
        return files;
    }

    /** Puts an opened log first in the remembered list, keeping {@link #LIMIT}. */
    static void remember(Node settings, File file) {
        List<String> paths = new ArrayList<>();
        paths.add(file.getAbsolutePath());
        for (File old : remembered(settings)) {
            if (!old.getAbsolutePath().equals(file.getAbsolutePath()) && paths.size() < LIMIT) {
                paths.add(old.getAbsolutePath());
            }
        }
        settings.setProperty(KEY, String.join(SEPARATOR, paths));
    }
}
