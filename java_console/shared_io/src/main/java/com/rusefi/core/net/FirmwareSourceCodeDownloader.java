package com.rusefi.core.net;

import com.rusefi.core.FileUtil;
import com.rusefi.core.net.ConnectionAndMeta.DownloadProgressListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.OverlappingFileLockException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.nio.file.attribute.FileTime;
import java.time.Clock;
import java.time.Duration;
import java.util.Enumeration;
import java.util.Objects;
import java.util.zip.CRC32;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/** Blocking source/wiki cache preparation. Invoke on a worker, not the Swing event thread. */
public final class FirmwareSourceCodeDownloader {
    public static final String DOWNLOAD_URL = "https://rusefi.com/build_server/firmware-source.zip";
    public static final String ARCHIVE_NAME = "firmware-source.zip";
    public static final long MIN_ARCHIVE_SIZE = 10L * 1024 * 1024;
    private static final long MAX_ARCHIVE_SIZE = 256L * 1024 * 1024;
    private static final long MAX_EXTRACTED_SIZE = 1024L * 1024 * 1024;
    private static final Duration MAX_AGE = Duration.ofDays(30);
    private final Path directory;
    private final URL source;
    private final Clock clock;

    public FirmwareSourceCodeDownloader() {
        this(Paths.get(FileUtil.RUSEFI_SETTINGS_FOLDER, "llm-temp"));
    }

    public FirmwareSourceCodeDownloader(Path directory) {
        this(directory, defaultSource(), Clock.systemUTC());
    }

    /** Local HTTP server and clock seam for deterministic cache tests. */
    FirmwareSourceCodeDownloader(Path directory, URL source, Clock clock) {
        this.directory = directory.toAbsolutePath().normalize();
        this.source = source;
        this.clock = clock;
    }

    private static URL defaultSource() {
        try {
            return new URL(DOWNLOAD_URL);
        } catch (java.net.MalformedURLException e) {
            throw new AssertionError(e);
        }
    }

    /**
     * Returns the directory containing the ZIP and extracted source/wiki trees.
     * Progress is monotonic 0..100 for this call, on the calling thread (marshal to EDT for a progress bar).
     * Each call extracts even a reused ZIP, restoring missing files without another download.
     */
    public Path download(DownloadProgressListener listener) throws IOException {
        return prepare(listener, true, false);
    }

    /** Explicit user-requested refresh, including recovery from a corrupt but recent cached ZIP. */
    public Path downloadFresh(DownloadProgressListener listener) throws IOException {
        return prepare(listener, true, true);
    }

    /**
     * Validate and extract a usable cached ZIP without contacting the server.
     * Returns null if the ZIP is missing, too small or expired; throws if validation/extraction fails.
     */
    public Path prepareCached(DownloadProgressListener listener) throws IOException {
        return prepare(listener, false, false);
    }

    private Path prepare(DownloadProgressListener listener, boolean allowDownload, boolean forceRefresh) throws IOException {
        Progress progress = new Progress(Objects.requireNonNull(listener, "listener"));
        progress.report(0);
        if (Files.isSymbolicLink(directory)) {
            throw new IOException("Source cache directory must not be a symbolic link: " + directory);
        }
        Files.createDirectories(directory);
        Path lockPath = directory.resolve(".download.lock");
        try (FileChannel channel = FileChannel.open(lockPath, StandardOpenOption.CREATE,
                StandardOpenOption.WRITE, LinkOption.NOFOLLOW_LINKS)) {
            try (FileLock lock = channel.tryLock()) {
                if (lock == null) {
                    throw new IOException("Source cache is already being prepared: " + directory);
                }
                return prepare(progress, allowDownload, forceRefresh);
            } catch (OverlappingFileLockException e) {
                throw new IOException("Source cache is already being prepared: " + directory, e);
            }
        }
    }

    private Path prepare(Progress progress, boolean allowDownload, boolean forceRefresh) throws IOException {
        Path archive = directory.resolve(ARCHIVE_NAME);
        boolean refresh = forceRefresh || !Files.isRegularFile(archive, LinkOption.NOFOLLOW_LINKS)
                || Files.size(archive) < MIN_ARCHIVE_SIZE
                || Files.getLastModifiedTime(archive).toInstant().isBefore(clock.instant().minus(MAX_AGE));
        if (refresh && !allowDownload) {
            return null;
        }
        Path temporary = Files.createTempDirectory(directory, ".source-");
        try {
            Path candidate = refresh ? temporary.resolve(ARCHIVE_NAME) : archive;
            if (refresh) {
                fetch(candidate, progress);
            }
            Path extracted = Files.createDirectory(temporary.resolve("extracted"));
            extract(candidate, extracted, progress, refresh ? 70 : 0);
            // Validate the entire ZIP before replacing any previously extracted content.
            try (DirectoryStream<Path> roots = Files.newDirectoryStream(extracted)) {
                for (Path root : roots) {
                    Path target = directory.resolve(root.getFileName());
                    if (Files.isSymbolicLink(target)) {
                        throw new IOException("Source cache entry must not be a symbolic link: " + target);
                    }
                }
            }
            try (DirectoryStream<Path> roots = Files.newDirectoryStream(extracted)) {
                for (Path root : roots) {
                    Path target = directory.resolve(root.getFileName());
                    deleteTree(target);
                    Files.move(root, target);
                }
            }
            if (refresh) {
                // Age measures successful local download time, not the server's source timestamp.
                Files.setLastModifiedTime(candidate, FileTime.from(clock.instant()));
                try {
                    Files.move(candidate, archive, StandardCopyOption.ATOMIC_MOVE, StandardCopyOption.REPLACE_EXISTING);
                } catch (AtomicMoveNotSupportedException e) {
                    Files.move(candidate, archive, StandardCopyOption.REPLACE_EXISTING);
                }
            }
            progress.report(100);
            return directory;
        } finally {
            deleteTree(temporary);
        }
    }

    private void fetch(Path target, Progress progress) throws IOException {
        HttpURLConnection connection = (HttpURLConnection) source.openConnection();
        connection.setConnectTimeout(30_000);
        connection.setReadTimeout(60_000);
        connection.setUseCaches(false);
        connection.setRequestProperty("User-Agent", "RE-Internal-Sync");
        connection.setRequestProperty("Cache-Control", "no-cache");
        try {
            int status = connection.getResponseCode();
            if (status != HttpURLConnection.HTTP_OK) {
                throw new IOException("Firmware source download returned HTTP " + status);
            }
            long expected = connection.getContentLengthLong();
            if (expected > MAX_ARCHIVE_SIZE) {
                throw new IOException("Firmware source archive is too large.");
            }
            long copied = 0;
            byte[] buffer = new byte[32 * 1024];
            try (InputStream input = connection.getInputStream(); OutputStream output = Files.newOutputStream(target)) {
                int count;
                while ((count = input.read(buffer)) != -1) {
                    copied += count;
                    if (copied > MAX_ARCHIVE_SIZE) {
                        throw new IOException("Firmware source archive is too large.");
                    }
                    output.write(buffer, 0, count);
                    if (expected > 0) {
                        progress.report((int) Math.min(69, copied * 70 / expected));
                    }
                }
            }
            if (expected >= 0 && copied != expected) {
                throw new IOException("Incomplete firmware source download.");
            }
            if (copied < MIN_ARCHIVE_SIZE) {
                throw new IOException("Firmware source archive is smaller than 10 MiB.");
            }
            progress.report(70);
        } finally {
            connection.disconnect();
        }
    }

    private static void extract(Path archive, Path destination, Progress progress, int start) throws IOException {
        try (ZipFile zip = new ZipFile(archive.toFile())) {
            if (zip.size() == 0 || zip.size() > 100_000) {
                throw new IOException("Invalid firmware source archive entry count.");
            }
            int completed = 0;
            long total = 0;
            byte[] buffer = new byte[32 * 1024];
            Enumeration<? extends ZipEntry> entries = zip.entries();
            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                String name = entry.getName().replace('\\', '/');
                if (name.contains(":")) {
                    throw new IOException("Invalid source ZIP entry: " + name);
                }
                Path target;
                try {
                    target = destination.resolve(name).normalize();
                } catch (InvalidPathException e) {
                    throw new IOException("Invalid source ZIP entry: " + name, e);
                }
                if (!target.startsWith(destination) || target.equals(destination)
                        || target.getName(destination.getNameCount()).toString().startsWith(".")
                        || target.getName(destination.getNameCount()).toString().equalsIgnoreCase(ARCHIVE_NAME)) {
                    throw new IOException("Invalid source ZIP entry: " + name);
                }
                if (entry.isDirectory()) {
                    Files.createDirectories(target);
                } else {
                    Files.createDirectories(target.getParent());
                    CRC32 crc = new CRC32();
                    long bytes = 0;
                    try (InputStream input = zip.getInputStream(entry);
                         OutputStream output = Files.newOutputStream(target, StandardOpenOption.CREATE_NEW)) {
                        int count;
                        while ((count = input.read(buffer)) != -1) {
                            bytes += count;
                            total += count;
                            if (total > MAX_EXTRACTED_SIZE) {
                                throw new IOException("Extracted firmware source is too large.");
                            }
                            crc.update(buffer, 0, count);
                            output.write(buffer, 0, count);
                        }
                    }
                    if (bytes != entry.getSize() || crc.getValue() != entry.getCrc()) {
                        throw new IOException("Corrupt source ZIP entry: " + name);
                    }
                    // ZIP symlink entries are intentionally regular text containing their link target.
                }
                progress.report(start + (int) (++completed * (99L - start) / zip.size()));
            }
        }
    }

    private static void deleteTree(Path root) throws IOException {
        if (!Files.exists(root, LinkOption.NOFOLLOW_LINKS)) {
            return;
        }
        Files.walkFileTree(root, new SimpleFileVisitor<Path>() {
            @Override public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                Files.delete(file);
                return FileVisitResult.CONTINUE;
            }
            @Override public FileVisitResult postVisitDirectory(Path dir, IOException error) throws IOException {
                if (error != null) {
                    throw error;
                }
                Files.delete(dir);
                return FileVisitResult.CONTINUE;
            }
        });
    }

    private static final class Progress {
        private final DownloadProgressListener listener;
        private int previous = -1;
        Progress(DownloadProgressListener listener) { this.listener = listener; }
        void report(int value) {
            if (Thread.currentThread().isInterrupted()) {
                throw new java.util.concurrent.CancellationException("Firmware source download interrupted");
            }
            if (value > previous) {
                previous = value;
                listener.onPercentage(value);
            }
        }
    }
}
