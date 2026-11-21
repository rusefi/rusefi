package com.rusefi.core;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InterruptedIOException;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

class CompressedIniArchiveTest {
    private static final String SIGNATURE = "rusEFI master.2026.07.02.uaefi.2367417284";
    private static final byte[] INI = ("[MegaTune]\nsignature = \"" + SIGNATURE + "\"\n")
        .getBytes(StandardCharsets.UTF_8);

    @Test
    void bundledExecutableIsResolvedBesideJarWithoutUsingWorkingDirectory(@TempDir Path directory)
            throws IOException {
        Path application = Files.createDirectory(directory.resolve("Console With Spaces"));
        Path jar = Files.createFile(application.resolve("console.jar"));
        Path executable = Files.createFile(application.resolve("7za.exe"));

        assertEquals(executable.toAbsolutePath(), CompressedIniArchive.bundledExecutable(jar));
        assertThrows(IOException.class,
            () -> CompressedIniArchive.bundledExecutable(directory.resolve("missing-console.jar")));
    }

    @Test
    void launchesWithFixedArgumentsAndPreservesExecutableAndMemberPathsWithSpaces() throws IOException {
        String executable = "C:\\Program Files\\rusEFI Console\\7za.exe";
        Path snapshot = java.nio.file.Paths.get("C:\\Temp Files\\owned snapshot.7z");
        String member = "-rusefi [board]*.ini";
        String listing = "----------\n\nPath = " + member + "\nSize = " + INI.length
            + "\nFolder = -\n\n";
        List<List<String>> commands = new ArrayList<>();
        CompressedIniArchive.ProcessStarter starter = command -> {
            commands.add(new ArrayList<>(command));
            return new FakeProcess(commands.size() == 1
                ? listing.getBytes(StandardCharsets.UTF_8) : INI, new byte[0]);
        };

        assertArrayEquals(INI, CompressedIniArchive.readMatchingSnapshotForTests(
            SIGNATURE, snapshot, executable, 2_000, starter));

        assertEquals(2, commands.size());
        assertEquals(executable, commands.get(0).get(0));
        List<String> extract = commands.get(1);
        assertEquals("e", extract.get(1));
        assertTrue(extract.contains("-spd"));
        int separator = extract.indexOf("--");
        assertTrue(separator > 0);
        assertEquals(snapshot.toString(), extract.get(separator - 1));
        assertEquals(member, extract.get(separator + 1));
    }

    @Test
    void timedOutProcessIsKilledAndItsReadersAreReleased() {
        FakeProcess process = new FakeProcess(new byte[0], new byte[0], true);

        IOException failure = assertThrows(IOException.class,
            () -> CompressedIniArchive.readMatchingSnapshotForTests(
                SIGNATURE, java.nio.file.Paths.get("snapshot.7z"), "fake 7za", 2_000,
                command -> process));

        assertTrue(failure.getMessage().contains("timed out"));
        assertTrue(process.destroyed);
        assertFalse(process.isAlive());
    }

    @Test
    void interruptionKillsProcessAndReturnsWithoutWaitingForChild() throws Exception {
        CountDownLatch waiting = new CountDownLatch(1);
        FakeProcess process = new FakeProcess(new byte[0], new byte[0], waiting);
        AtomicReference<Throwable> failure = new AtomicReference<>();
        Thread lookup = new Thread(() -> {
            try {
                CompressedIniArchive.readMatchingSnapshotForTests(
                    SIGNATURE, java.nio.file.Paths.get("snapshot.7z"), "fake 7za", 10_000,
                    command -> process);
            } catch (Throwable e) {
                failure.set(e);
            }
        }, "7z cancellation test");
        lookup.start();

        assertTrue(waiting.await(1, TimeUnit.SECONDS));
        lookup.interrupt();
        lookup.join(2_000);

        assertFalse(lookup.isAlive());
        assertTrue(failure.get() instanceof InterruptedIOException);
        assertTrue(process.destroyed);
        assertFalse(process.isAlive());
    }

    @Test
    void rejectsAmbiguousListingWithDuplicateMemberPaths() {
        byte[] listing = ("----------\n\nPath = first.ini\nSize = " + INI.length
            + "\n\nPath = first.ini\nSize = " + INI.length + "\n\n").getBytes(StandardCharsets.UTF_8);
        FakeProcess process = new FakeProcess(listing, new byte[0]);

        assertThrows(IOException.class,
            () -> CompressedIniArchive.readMatchingSnapshotForTests(
                SIGNATURE, java.nio.file.Paths.get("snapshot.7z"), "fake 7za", 2_000,
                command -> process));
    }

    @Test
    void rejectsMemberPathContainingAListingNewline() {
        byte[] listing = ("----------\n\nPath = board\ninjected.ini\nSize = " + INI.length
            + "\n\n").getBytes(StandardCharsets.UTF_8);
        FakeProcess process = new FakeProcess(listing, new byte[0]);

        assertThrows(IOException.class,
            () -> CompressedIniArchive.readMatchingSnapshotForTests(
                SIGNATURE, java.nio.file.Paths.get("snapshot.7z"), "fake 7za", 2_000,
                command -> process));
    }

    @Test
    void boundsListingAndDiagnosticOutput() {
        FakeProcess oversizedListing = new FakeProcess(
            new byte[CompressedIniArchive.MAX_LIST_BYTES + 1], new byte[0]);
        IOException listingFailure = assertThrows(IOException.class,
            () -> CompressedIniArchive.readMatchingSnapshotForTests(
                SIGNATURE, java.nio.file.Paths.get("snapshot.7z"), "fake 7za", 2_000,
                command -> oversizedListing));
        assertTrue(listingFailure.getMessage().contains("output exceeds"));

        String listing = "----------\n\nPath = board.ini\nSize = " + INI.length + "\n\n";
        FakeProcess oversizedStderr = new FakeProcess(listing.getBytes(StandardCharsets.UTF_8),
            new byte[CompressedIniArchive.MAX_STDERR_BYTES + 1]);
        IOException stderrFailure = assertThrows(IOException.class,
            () -> CompressedIniArchive.readMatchingSnapshotForTests(
                SIGNATURE, java.nio.file.Paths.get("snapshot.7z"), "fake 7za", 2_000,
                command -> oversizedStderr));
        assertTrue(stderrFailure.getMessage().contains("diagnostics exceed"));
    }

    private static final class FakeProcess extends Process {
        private final ByteArrayInputStream stdout;
        private final ByteArrayInputStream stderr;
        private final boolean timeout;
        private final CountDownLatch waitForInterrupt;
        private volatile boolean alive = true;
        volatile boolean destroyed;

        FakeProcess(byte[] stdout, byte[] stderr) {
            this(stdout, stderr, false, null);
        }

        FakeProcess(byte[] stdout, byte[] stderr, boolean timeout) {
            this(stdout, stderr, timeout, null);
        }

        FakeProcess(byte[] stdout, byte[] stderr, CountDownLatch waitForInterrupt) {
            this(stdout, stderr, false, waitForInterrupt);
        }

        private FakeProcess(byte[] stdout, byte[] stderr, boolean timeout, CountDownLatch waitForInterrupt) {
            this.stdout = new ByteArrayInputStream(stdout);
            this.stderr = new ByteArrayInputStream(stderr);
            this.timeout = timeout;
            this.waitForInterrupt = waitForInterrupt;
        }

        @Override
        public OutputStream getOutputStream() {
            return new ByteArrayOutputStream();
        }

        @Override
        public ByteArrayInputStream getInputStream() {
            return stdout;
        }

        @Override
        public ByteArrayInputStream getErrorStream() {
            return stderr;
        }

        @Override
        public int waitFor() throws InterruptedException {
            alive = false;
            return 0;
        }

        @Override
        public boolean waitFor(long timeout, TimeUnit unit) throws InterruptedException {
            if (!alive) {
                return true;
            }
            if (waitForInterrupt != null) {
                waitForInterrupt.countDown();
                new CountDownLatch(1).await();
            }
            if (this.timeout) {
                return false;
            }
            alive = false;
            return true;
        }

        @Override
        public int exitValue() {
            if (alive) {
                throw new IllegalThreadStateException();
            }
            return 0;
        }

        @Override
        public void destroy() {
            destroyForcibly();
        }

        @Override
        public Process destroyForcibly() {
            destroyed = true;
            alive = false;
            return this;
        }

        @Override
        public boolean isAlive() {
            return alive;
        }
    }
}
