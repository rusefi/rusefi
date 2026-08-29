package com.rusefi.openblt;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class OpenBltCanFlasherTest {

    private static final int SEG1_BASE = 0x08008000;
    private static final int SEG2_BASE = 0x08020000;

    private static byte[] pattern(int len, int seed) {
        byte[] out = new byte[len];
        for (int i = 0; i < len; i++) {
            out[i] = (byte) (i * 13 + seed);
        }
        return out;
    }

    private static Path writeImage(List<String> lines) throws IOException {
        Path file = Files.createTempFile("openblt_flash", ".srec");
        Files.write(file, lines, StandardCharsets.US_ASCII);
        file.toFile().deleteOnExit();
        return file;
    }

    private static OpenBltCanFlasher.Config config(Path srec, boolean verify, boolean reset) {
        OpenBltCanFlasher.Config cfg = new OpenBltCanFlasher.Config();
        cfg.srecPath = srec;
        cfg.connectTimeoutSec = 2;
        cfg.verify = verify;
        cfg.reset = reset;
        // Unit tests run against the synchronous in-memory bootloader: no bus
        // pacing needed (and pacing would add ~0.6 ms per frame to the big
        // erase test below).
        cfg.pipelinePaceNanos = 0;
        return cfg;
    }

    private static List<String> buildImage(byte[] seg1, byte[] seg2) {
        List<String> lines = new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1));
        lines.remove(lines.size() - 1); // drop terminator
        lines.addAll(SrecTestUtil.image(SEG2_BASE, seg2));
        return lines;
    }

    @Test
    void flashesSegmentsAndVerifies() throws Exception {
        byte[] seg1 = pattern(40, 1);   // 40 bytes: one 40-byte batch = 6x PROGRAM_MAX
        byte[] seg2 = pattern(5, 2);    // 5 bytes: one 5-byte batch = 1x PROGRAM_MAX
        Path srec = writeImage(buildImage(seg1, seg2));

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        OpenBltCanFlasher.Result result = flasher.flash(link, config(srec, true, true));

        assertEquals(2, result.segments);
        assertEquals(45, result.bytes);
        assertTrue(result.verified);
        assertTrue(result.reset);

        assertArrayEquals(seg1, link.flashAt(SEG1_BASE, 40));
        assertArrayEquals(seg2, link.flashAt(SEG2_BASE, 5));
        assertEquals(2, link.programClearCount());
        assertEquals(1, link.programResetCount());
        assertEquals(2, link.batchCount());
        assertTrue(link.sawProgramSizeZero());

        // Wire-level sanity: first frame is CONNECT (the bootloader entry
        // trigger); the image went out as deferred-ACK batches of PROGRAM_MAX.
        assertEquals(0xFF, link.sent().get(0).data()[0] & 0xFF);
        assertTrue(link.sent().stream().anyMatch(f -> (f.data()[0] & 0xFF) == XcpConstants.CMD_PROGRAM_BATCH));
        assertTrue(link.sent().stream().anyMatch(f -> (f.data()[0] & 0xFF) == XcpConstants.CMD_PROGRAM_MAX));
    }

    @Test
    void erasesInChunksForLargeSegments() throws Exception {
        // 80 KB segment: 32 KB + 32 KB + 16 KB erase chunks.
        byte[] seg1 = pattern(80 * 1024, 7);
        List<String> lines = new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1));
        Path srec = writeImage(lines);

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        flasher.flash(link, config(srec, false, false));

        assertEquals(3, link.programClearCount());
        assertArrayEquals(java.util.Arrays.copyOfRange(seg1, 0, 100), link.flashAt(SEG1_BASE, 100));
    }

    @Test
    void verifyCatchesCorruption() throws Exception {
        byte[] seg1 = pattern(32, 3);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        // Flip one flash byte after programming: the checksum step must catch it.
        CanLink corrupted = new CorruptingLink(link, SEG1_BASE + 10);
        OpenBltCanFlasher.FlashException e = assertThrows(OpenBltCanFlasher.FlashException.class,
                () -> flasher.flash(corrupted, config(srec, true, false)));
        assertTrue(e.getMessage().contains("Checksum mismatch"), e.getMessage());
    }

    /** Wraps a fake link and corrupts one flash byte after the PROGRAM phase. */
    private static final class CorruptingLink implements CanLink {
        private final FakeCanLink delegate;
        private final int corruptAt;
        private boolean corrupted;

        CorruptingLink(FakeCanLink delegate, int corruptAt) {
            this.delegate = delegate;
            this.corruptAt = corruptAt;
        }

        @Override
        public void open() {
        }

        @Override
        public void write(CanFrame frame) throws IOException {
            delegate.write(frame);
            // Corrupt right after the last program frame: PROGRAM size 0 is
            // the finalize command, sent after all batches completed and
            // before the checksum verification.
            if (!corrupted && (frame.data()[0] & 0xFF) == XcpConstants.CMD_PROGRAM
                    && (frame.data()[1] & 0xFF) == 0) {
                corrupted = true;
                delegate.corrupt(corruptAt, 0x5A);
            }
        }

        @Override
        public CanFrame readFrame(int timeoutMs) {
            return delegate.readFrame(timeoutMs);
        }

        @Override
        public CanFrame pollFrame() {
            return delegate.pollFrame();
        }

        @Override
        public void close() {
        }
    }

    @Test
    void refusesToTouchBootloaderArea() throws Exception {
        // Image starting inside the bootloader's 32 KB - must be rejected
        // client-side before anything is erased.
        byte[] seg = pattern(8, 9);
        List<String> lines = new ArrayList<>(SrecTestUtil.image(0x08000000, seg));
        Path srec = writeImage(lines);

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        OpenBltCanFlasher.FlashException e = assertThrows(OpenBltCanFlasher.FlashException.class,
                () -> flasher.flash(link, config(srec, false, false)));
        assertTrue(e.getMessage().contains("overlaps the bootloader"), e.getMessage());
        assertEquals(0, link.programClearCount());
    }

    @Test
    void survivesBootloaderArrivingLate() throws Exception {
        // The first CONNECT frames get no answer (app still running and
        // resetting into the bootloader); the retry loop must keep going.
        byte[] seg1 = pattern(16, 4);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        link.setIgnoredConnects(2);
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        OpenBltCanFlasher.Result result = flasher.flash(link, config(srec, false, false));

        assertEquals(16, result.bytes);
        assertTrue(link.sent().size() >= 5); // several CONNECT attempts happened
    }

    @Test
    void failsWhenBootloaderNeverAnswers() throws Exception {
        byte[] seg1 = pattern(16, 5);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        link.setMute(true);
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        OpenBltCanFlasher.FlashException e = assertThrows(OpenBltCanFlasher.FlashException.class,
                () -> flasher.flash(link, config(srec, false, false)));
        assertTrue(e.getMessage().contains("No response from bootloader"), e.getMessage());
    }

    @Test
    void noResetSkipsProgramReset() throws Exception {
        byte[] seg1 = pattern(16, 6);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        OpenBltCanFlasher.Result result = flasher.flash(link, config(srec, false, false));

        assertEquals(0, link.programResetCount());
        assertTrue(result.bytes > 0);
    }

    @Test
    void batchedFlashProgramsEveryFrame() throws Exception {
        // 1000 bytes: one 1000-byte batch = ceil(1000/7) = 143 PROGRAM_MAX
        // frames, ONE batch acknowledgement, no single-frame PROGRAM tail.
        byte[] seg1 = pattern(1000, 11);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        OpenBltCanFlasher.Result result = flasher.flash(link, config(srec, false, false));

        assertEquals(1000, result.bytes);
        assertEquals(143, link.programMaxCount());
        assertEquals(1, link.batchCount());
        assertArrayEquals(seg1, link.flashAt(SEG1_BASE, 1000));
    }

    @Test
    void noBatchFlagUsesPipelinedMode() throws Exception {
        // --no-batch: the window-pipelined PROGRAM_MAX mode; the segment tail
        // goes out as a single-frame PROGRAM.
        byte[] seg1 = pattern(1000, 12);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });
        OpenBltCanFlasher.Config cfg = config(srec, false, false);
        cfg.batch = false;

        OpenBltCanFlasher.Result result = flasher.flash(link, cfg);

        assertEquals(1000, result.bytes);
        assertEquals(142, link.programMaxCount());
        assertEquals(0, link.batchCount());
        assertArrayEquals(seg1, link.flashAt(SEG1_BASE, 1000));
        assertTrue(link.sent().stream().anyMatch(f -> (f.data()[0] & 0xFF) == XcpConstants.CMD_PROGRAM
                && (f.data()[1] & 0xFF) == 6));
    }

    @Test
    void lostBatchAckAbortsFlash() throws Exception {
        // Simulate the single batch acknowledgement lost on the wire (the
        // batch was programmed but never acknowledged): the flasher must
        // abort with a clear diagnostic instead of guessing the MTA state.
        byte[] seg1 = pattern(64, 13);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        link.dropProgramMaxResponses(1);
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });

        OpenBltCanFlasher.FlashException e = assertThrows(OpenBltCanFlasher.FlashException.class,
                () -> flasher.flash(link, config(srec, false, false)));
        assertTrue(e.getMessage().contains("no acknowledgement"), e.getMessage());
    }

    @Test
    void oneMbitSwitchHappensAroundProgramming() throws Exception {
        // --1mbit: the link must switch to 1M before programming and back to
        // 500k before verify/reset, so the console/car bus always ends at 500k.
        byte[] seg1 = pattern(100, 14);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });
        OpenBltCanFlasher.Config cfg = config(srec, true, true);
        cfg.oneMbit = true;

        OpenBltCanFlasher.Result result = flasher.flash(link, cfg);

        assertTrue(result.verified);
        assertArrayEquals(seg1, link.flashAt(SEG1_BASE, 100));
        assertEquals(2, link.setBaudrateCount());
        assertEquals(XcpConstants.BAUD_500K, link.lastBaudrate());
    }

    @Test
    void noPipelineFlagStillFlashes() throws Exception {
        // The single-frame fallback path must keep working (--no-pipeline).
        byte[] seg1 = pattern(40, 15);
        Path srec = writeImage(new ArrayList<>(SrecTestUtil.image(SEG1_BASE, seg1)));

        FakeCanLink link = new FakeCanLink();
        OpenBltCanFlasher flasher = new OpenBltCanFlasher(new OpenBltCanFlasher.Listener() {
        });
        OpenBltCanFlasher.Config cfg = config(srec, true, false);
        cfg.pipeline = false;
        cfg.batch = false;

        OpenBltCanFlasher.Result result = flasher.flash(link, cfg);

        assertEquals(40, result.bytes);
        assertTrue(result.verified);
        assertArrayEquals(seg1, link.flashAt(SEG1_BASE, 40));
    }
}
