package com.rusefi.core;

import com.devexperts.logging.Logging;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InterruptedIOException;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;

/** Reads one exact-signature INI from a bounded, private snapshot of a 7z archive. */
final class CompressedIniArchive {
    private static final Logging log = getLogging(CompressedIniArchive.class);
    static final long MAX_ARCHIVE_BYTES = 32L * 1024 * 1024;
    static final int MAX_ARCHIVE_ENTRIES = 256;
    static final int MAX_INI_BYTES = 4 * 1024 * 1024;
    static final int MAX_TOTAL_INI_BYTES = 16 * 1024 * 1024;
    static final int MAX_LIST_BYTES = 1024 * 1024;
    static final int MAX_STDERR_BYTES = 64 * 1024;
    static final long PROCESS_TIMEOUT_MILLIS = 5_000;
    static final long ARCHIVE_TIMEOUT_MILLIS = 12_000;

    private static final Semaphore PROCESS_LIMIT = new Semaphore(16);
    private static final String WINDOWS = "windows";

    interface ProcessStarter {
        Process start(List<String> command) throws IOException;
    }

    private static final ProcessStarter SYSTEM_PROCESS_STARTER = command ->
        new ProcessBuilder(command).start();

    private CompressedIniArchive() {
    }

    static byte[] readMatchingIni(String signature, File source) throws IOException {
        Path snapshot = Files.createTempFile("rusefi-ini-", ".7z");
        try {
            copyBoundedSnapshot(source.toPath(), snapshot);
            String executable = resolveExecutable();
            byte[] ini = readMatchingSnapshot(signature, snapshot, SYSTEM_PROCESS_STARTER,
                executable, ARCHIVE_TIMEOUT_MILLIS);
            log.info("Extracted matching INI from archive candidate " + source.getAbsolutePath()
                + " using 7-Zip executable " + executable);
            return ini;
        } finally {
            Files.deleteIfExists(snapshot);
        }
    }

    static byte[] readMatchingSnapshotForTests(String signature, Path snapshot, String executable,
                                               long timeoutMillis, ProcessStarter starter) throws IOException {
        return readMatchingSnapshot(signature, snapshot, starter, executable, timeoutMillis);
    }

    static String resolveExecutable() throws IOException {
        if (isWindows()) {
            try {
                Path classesOrJar = new File(SignatureHelper.class.getProtectionDomain()
                    .getCodeSource().getLocation().toURI()).toPath();
                return bundledExecutable(classesOrJar).toString();
            } catch (URISyntaxException | NullPointerException e) {
                throw new IOException("Unable to locate bundled 7za.exe from the Console JAR", e);
            }
        }
        // Linux host tests and Linux Console installations use the host's installed 7z.
        return "7z";
    }

    static Path bundledExecutable(Path classesOrJar) throws IOException {
        Path applicationDirectory = Files.isDirectory(classesOrJar)
            ? classesOrJar : classesOrJar.getParent();
        if (applicationDirectory == null) {
            throw new IOException("Unable to locate application directory for bundled 7za.exe");
        }
        Path executable = applicationDirectory.resolve("7za.exe");
        if (!Files.isRegularFile(executable)) {
            throw new IOException("Bundled 7za.exe was not found next to the Console JAR: " + executable);
        }
        return executable.toAbsolutePath();
    }

    private static boolean isWindows() {
        return System.getProperty("os.name", "").toLowerCase(Locale.ROOT).contains(WINDOWS);
    }

    private static void copyBoundedSnapshot(Path source, Path snapshot) throws IOException {
        long total = 0;
        try (InputStream input = Files.newInputStream(source);
             java.io.OutputStream output = Files.newOutputStream(snapshot)) {
            checkInterrupted();
            byte[] buffer = new byte[8192];
            int read;
            while ((read = input.read(buffer)) != -1) {
                checkInterrupted();
                total += read;
                if (total > MAX_ARCHIVE_BYTES) {
                    throw new IOException("INI 7z archive exceeds the " + MAX_ARCHIVE_BYTES + " byte limit");
                }
                output.write(buffer, 0, read);
            }
            checkInterrupted();
        }
    }

    private static byte[] readMatchingSnapshot(String signature, Path snapshot, ProcessStarter starter,
                                               String executable, long timeoutMillis) throws IOException {
        long deadlineNanos = System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(timeoutMillis);
        List<String> listCommand = command(executable, "l", "-t7z", "-slt", "-sccUTF-8", snapshot.toString());
        byte[] listingBytes = run(listCommand, MAX_LIST_BYTES, starter, remainingMillis(deadlineNanos));
        List<ArchiveEntry> entries = parseListing(new String(listingBytes, StandardCharsets.UTF_8));

        byte[] matchingIni = null;
        int totalIniBytes = 0;
        for (ArchiveEntry entry : entries) {
            checkInterrupted();
            if (entry.directory || !entry.path.toLowerCase(Locale.ROOT).endsWith(".ini")) {
                continue;
            }
            if (entry.size > MAX_INI_BYTES
                    || entry.size >= 0 && totalIniBytes + entry.size > MAX_TOTAL_INI_BYTES) {
                throw new IOException("INI 7z contents exceed the decompressed-size limit");
            }
            List<String> extractCommand = command(executable, "e", "-t7z", "-so", "-spd",
                snapshot.toString(), "--", entry.path);
            byte[] contents = run(extractCommand, MAX_INI_BYTES, starter, remainingMillis(deadlineNanos));
            if (entry.size >= 0 && contents.length != entry.size) {
                throw new IOException("7z extracted a different byte count than the listed INI member");
            }
            totalIniBytes += contents.length;
            if (totalIniBytes > MAX_TOTAL_INI_BYTES) {
                throw new IOException("INI 7z contents exceed the decompressed-size limit");
            }
            if (signature.equals(SignatureHelper.readIniSignature(contents))) {
                if (matchingIni != null) {
                    throw new IOException("INI 7z archive contains multiple files with signature " + signature);
                }
                matchingIni = contents;
            }
        }
        if (matchingIni == null) {
            throw new IOException("INI 7z archive contains no INI with signature " + signature);
        }
        return matchingIni;
    }

    private static long remainingMillis(long deadlineNanos) throws IOException {
        checkInterrupted();
        long remaining = deadlineNanos - System.nanoTime();
        if (remaining <= 0) {
            throw new IOException("INI 7z lookup exceeded its time limit");
        }
        return Math.max(1, TimeUnit.NANOSECONDS.toMillis(remaining));
    }

    private static List<String> command(String executable, String... arguments) {
        List<String> command = new ArrayList<>();
        command.add(executable);
        for (String argument : arguments) {
            command.add(argument);
        }
        return command;
    }

    private static List<ArchiveEntry> parseListing(String listing) throws IOException {
        List<ArchiveEntry> entries = new ArrayList<>();
        Set<String> paths = new HashSet<>();
        Set<String> properties = new HashSet<>();
        boolean entriesStarted = false;
        ArchiveEntry current = null;
        String[] lines = listing.split("\n", -1);
        for (String rawLine : lines) {
            String line = rawLine.endsWith("\r") ? rawLine.substring(0, rawLine.length() - 1) : rawLine;
            if (!entriesStarted) {
                if (line.equals("----------")) {
                    entriesStarted = true;
                }
                continue;
            }
            if (line.isEmpty()) {
                if (current != null) {
                    entries.add(current);
                    current = null;
                    properties.clear();
                    if (entries.size() > MAX_ARCHIVE_ENTRIES) {
                        throw new IOException("INI 7z archive contains more than "
                            + MAX_ARCHIVE_ENTRIES + " entries");
                    }
                }
                continue;
            }
            int separator = line.indexOf(" = ");
            if (separator <= 0) {
                throw new IOException("Ambiguous 7z listing member (newline or malformed property)");
            }
            String key = line.substring(0, separator);
            String value = line.substring(separator + 3);
            if ("Path".equals(key)) {
                if (current != null || value.isEmpty() || containsLineBreak(value)
                        || value.indexOf('\0') >= 0 || !paths.add(value)) {
                    throw new IOException("Ambiguous or duplicate 7z member path");
                }
                current = new ArchiveEntry(value);
                properties.clear();
                properties.add(key);
                continue;
            }
            if (current == null || !properties.add(key)) {
                throw new IOException("Malformed 7z listing entry");
            }
            if ("Folder".equals(key)) {
                current.directory = "+".equals(value);
            } else if ("Size".equals(key)) {
                try {
                    current.size = Long.parseLong(value);
                } catch (NumberFormatException e) {
                    throw new IOException("Malformed 7z member size", e);
                }
            }
        }
        if (current != null) {
            entries.add(current);
        }
        if (!entriesStarted || entries.size() > MAX_ARCHIVE_ENTRIES) {
            throw new IOException("Invalid or oversized 7z listing");
        }
        return entries;
    }

    private static boolean containsLineBreak(String value) {
        return value.indexOf('\n') >= 0 || value.indexOf('\r') >= 0;
    }

    private static byte[] run(List<String> command, int stdoutLimit,
                              ProcessStarter starter, long timeoutMillis) throws IOException {
        long commandTimeoutMillis = Math.min(timeoutMillis, PROCESS_TIMEOUT_MILLIS);
        long deadlineNanos = System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(commandTimeoutMillis);
        try {
            if (!PROCESS_LIMIT.tryAcquire(commandTimeoutMillis, TimeUnit.MILLISECONDS)) {
                throw new IOException("Timed out waiting for a 7z process slot");
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw interrupted("Interrupted while waiting to read INI 7z archive", e);
        }
        try {
            return runLimited(command, stdoutLimit, starter, remainingMillis(deadlineNanos));
        } finally {
            PROCESS_LIMIT.release();
        }
    }

    private static byte[] runLimited(List<String> command, int stdoutLimit,
                                     ProcessStarter starter, long timeoutMillis) throws IOException {
        Process process = starter.start(command);
        BoundedReader stdout = new BoundedReader(process.getInputStream(), stdoutLimit);
        BoundedReader stderr = new BoundedReader(process.getErrorStream(), MAX_STDERR_BYTES);
        Thread stdoutReader = readerThread(stdout, "7z stdout");
        Thread stderrReader = readerThread(stderr, "7z stderr");
        closeQuietly(process.getOutputStream());
        stdoutReader.start();
        stderrReader.start();
        boolean interrupted = false;
        try {
            if (!process.waitFor(timeoutMillis, TimeUnit.MILLISECONDS)) {
                throw new IOException("7z command timed out after " + timeoutMillis + " ms");
            }
            joinReader(stdoutReader, process);
            joinReader(stderrReader, process);
            if (stdout.error != null) {
                throw new IOException("Failed reading 7z output", stdout.error);
            }
            if (stderr.error != null) {
                throw new IOException("Failed draining 7z diagnostics", stderr.error);
            }
            if (stdout.overflow) {
                throw new IOException("7z output exceeds the " + stdoutLimit + " byte limit");
            }
            if (stderr.overflow) {
                throw new IOException("7z diagnostics exceed the " + MAX_STDERR_BYTES + " byte limit");
            }
            if (process.exitValue() != 0) {
                throw new IOException("7z command failed with exit code " + process.exitValue());
            }
            return stdout.bytes();
        } catch (InterruptedException e) {
            interrupted = true;
            throw interrupted("INI 7z lookup was cancelled", e);
        } finally {
            if (process.isAlive()) {
                process.destroyForcibly();
            }
            try {
                process.waitFor(1_000, TimeUnit.MILLISECONDS);
            } catch (InterruptedException e) {
                interrupted = true;
            }
            closeQuietly(process.getInputStream());
            closeQuietly(process.getErrorStream());
            closeQuietly(process.getOutputStream());
            joinAfterCleanup(stdoutReader);
            joinAfterCleanup(stderrReader);
            if (interrupted) {
                Thread.currentThread().interrupt();
            }
        }
    }

    private static Thread readerThread(BoundedReader reader, String name) {
        Thread thread = new Thread(reader, name);
        thread.setDaemon(true);
        return thread;
    }

    private static void joinReader(Thread reader, Process process) throws InterruptedException, IOException {
        reader.join(500);
        if (reader.isAlive()) {
            process.destroyForcibly();
            throw new IOException("7z output reader did not finish");
        }
    }

    private static void joinAfterCleanup(Thread reader) {
        try {
            reader.join(1_000);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    private static void closeQuietly(java.io.Closeable closeable) {
        try {
            closeable.close();
        } catch (IOException ignored) {
            // Process cleanup must continue even if a stream has already closed.
        }
    }

    private static InterruptedIOException interrupted(String message, InterruptedException cause) {
        InterruptedIOException exception = new InterruptedIOException(message);
        exception.initCause(cause);
        return exception;
    }

    private static void checkInterrupted() throws InterruptedIOException {
        if (Thread.currentThread().isInterrupted()) {
            throw new InterruptedIOException("INI lookup was cancelled");
        }
    }

    private static final class ArchiveEntry {
        final String path;
        long size = -1;
        boolean directory;

        ArchiveEntry(String path) {
            this.path = path;
        }
    }

    private static final class BoundedReader implements Runnable {
        private final InputStream input;
        private final int maximum;
        private final ByteArrayOutputStream output = new ByteArrayOutputStream();
        volatile boolean overflow;
        volatile IOException error;

        BoundedReader(InputStream input, int maximum) {
            this.input = input;
            this.maximum = maximum;
        }

        @Override
        public void run() {
            byte[] buffer = new byte[8192];
            try {
                int read;
                while ((read = input.read(buffer)) != -1) {
                    int remaining = maximum - output.size();
                    if (read > remaining) {
                        overflow = true;
                    }
                    if (remaining > 0) {
                        output.write(buffer, 0, Math.min(read, remaining));
                    }
                }
            } catch (IOException e) {
                error = e;
            }
        }

        byte[] bytes() {
            return output.toByteArray();
        }
    }
}
