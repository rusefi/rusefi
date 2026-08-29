package com.rusefi.openblt;

import peak.can.basic.TPCANHandle;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.concurrent.locks.LockSupport;

/**
 * Flashes an rusEFI ECU through the OpenBLT bootloader over CAN.
 *
 * Entry sequence: the CONNECT frame is DLC=2, which is exactly the trigger
 * the running firmware reacts to when canOpenBLT is enabled - it jumps to the
 * bootloader and the bootloader answers the next CONNECT. When the app is not
 * running, the bootloader's post-reset backdoor window (1 s on m74_9) answers
 * the same retried CONNECT. So one retry loop covers both entry paths.
 *
 * Flash sequence (mirrors BootCommander / LibOpenBLT xcploader):
 *   CONNECT -> per segment: SET_MTA + PROGRAM_CLEAR in 32 KB chunks ->
 *   SET_MTA + PROGRAM_MAX (7 bytes/frame) with PROGRAM for the tail ->
 *   optional BUILD_CHECKSUM verification -> PROGRAM_RESET.
 */
public class OpenBltCanFlasher {

    /**
     * Bus pacing for pipelined PROGRAM_MAX: one 8-byte ext frame takes ~265 us
     * at 500 kbit/s and the bootloader's ACK another ~265 us, so ~620 us per
     * slot lets the ACK interleave with the next frame instead of the dongle
     * transmitting back-to-back and saturating the ECU's 3 ACK mailboxes.
     */
    private static final long PIPELINE_PACE_NANOS = 620_000L;

    public interface Listener {
        default void log(String line) {
            System.out.println(line);
        }

        default void progress(long doneBytes, long totalBytes) {
        }
    }

    public static final class Config {
        int channel = 1;
        int connectTimeoutSec = 8;
        boolean verify = true;
        boolean reset = true;
        boolean probeOnly = false;
        boolean verbose = false;
        boolean pipeline = true;
        /**
         * Pipelined PROGRAM_MAX TX spacing in nanoseconds (default 620 us =
         * one 8-byte ext frame + the bootloader ACK at 500 kbit/s). 0 disables
         * pacing (unit tests; on the wire the dongle then bursts frames).
         */
        long pipelinePaceNanos = PIPELINE_PACE_NANOS;
        Path srecPath;
    }

    public static final class Result {
        public final int segments;
        public final long bytes;
        public final boolean verified;
        public final boolean reset;
        public final long elapsedMs;

        Result(int segments, long bytes, boolean verified, boolean reset, long elapsedMs) {
            this.segments = segments;
            this.bytes = bytes;
            this.verified = verified;
            this.reset = reset;
            this.elapsedMs = elapsedMs;
        }
    }

    public static final class FlashException extends Exception {
        public FlashException(String message) {
            super(message);
        }

        public FlashException(String message, Throwable cause) {
            super(message, cause);
        }
    }

    private final Listener listener;
    /** Set from Config before programming: enables the pipelined PROGRAM_MAX path. */
    private boolean pipelineEnabled = true;
    /** Set from Config before programming: pipelined TX spacing in nanoseconds. */
    private long pipelinePaceNanos = PIPELINE_PACE_NANOS;

    public OpenBltCanFlasher(Listener listener) {
        this.listener = listener;
    }

    public Result flash(CanLink link, Config cfg) throws IOException, FlashException {
        long t0 = System.currentTimeMillis();
        pipelineEnabled = cfg.pipeline;
        pipelinePaceNanos = cfg.pipelinePaceNanos;
        listener.log("Opening CAN channel...");

        try {
            link.open();

            XcpClient xcp = new XcpClient(link, XcpConstants.TX_ID, XcpConstants.EXTENDED,
                    XcpConstants.RX_ID, XcpConstants.COMMAND_TIMEOUT_MS,
                    cfg.verbose ? listener::log : null);
            listener.log(String.format("XCP over CAN: host TX 0x%03X ext, ECU TX 0x%03X ext, 500 kbps",
                    XcpConstants.TX_ID, XcpConstants.RX_ID));

            // ---- connect (with retries; first frame also triggers app->bootloader jump) ----
            XcpResponse connect = connectWithRetries(xcp, cfg);
            if (connect == null) {
                throw new FlashException("No response from bootloader within "
                        + cfg.connectTimeoutSec + " s. Is the ECU powered? Is the CAN adapter connected "
                        + "and terminated?\n"
                        + "  - with the new firmware (canOpenBLT trigger) the bootloader answers by itself;\n"
                        + "  - with older firmware: POWER-CYCLE the ECU (ignition off/on) WHILE this tool is running "
                        + "- the bootloader listens for 1 s after reset.\n"
                        + "  - rerun with --verbose to see whether any CAN traffic arrives at all.");
            }
            if (!connect.isOk()) {
                throw new FlashException("Bootloader rejected CONNECT: " + connect);
            }
            listener.log("Connected to OpenBLT bootloader.");

            // ---- optional station-id sanity check ----
            XcpResponse id = xcp.getId();
            if (id != null && id.isOk() && id.data().length >= 8) {
                int stationIdLen = le32(id.data(), 4);
                listener.log("Bootloader station id length: " + stationIdLen + " bytes");
            }

            			if (cfg.probeOnly) {
            				listener.log("Probe complete.");
            				// Leave the ECU in the application, not stuck in the bootloader:
            				// a probe session holds the bootloader forever (wasConnected).
            				xcp.programReset();
            				listener.log("ECU restarted into the application.");
            				return new Result(0, 0, false, true, System.currentTimeMillis() - t0);
            			}

            // ---- load image ----
            SrecParser.Image image = SrecParser.parse(cfg.srecPath);
            List<SrecParser.Segment> segments = image.segments();
            long total = image.totalBytes();
            listener.log(String.format("Firmware %s: %d segments, %d bytes",
                    cfg.srecPath, segments.size(), total));

            for (SrecParser.Segment seg : segments) {
                if (seg.base() < XcpConstants.APP_BASE) {
                    throw new FlashException(String.format(
                            "Segment at 0x%08X overlaps the bootloader area (app base 0x%08X). "
                                    + "Refusing to flash - is this the app-only .srec?",
                            seg.base(), XcpConstants.APP_BASE));
                }
            }

            // ---- erase + program ----
            long done = 0;
            for (SrecParser.Segment seg : segments) {
                eraseSegment(xcp, seg);
                programSegment(xcp, seg);
                done += seg.data().length;
                listener.progress(done, total);
            }

            // PROGRAM size=0 finalizes: NvmDone()/FlashDone() flushes the last
            // partial flash line on buffered platforms (H7 ECC, AT32 one-shot).
            {
                XcpResponse fin = xcp.programDone();
                if (fin == null || !fin.isOk()) {
                    throw new FlashException("Finalize (PROGRAM size=0) failed: " + fin);
                }
            }

            // ---- verify ----
            boolean verified = false;
            if (cfg.verify) {
                verifySegments(xcp, segments);
                verified = true;
                listener.log("Checksum verification passed.");
            }

            // ---- reset into the new app ----
            if (cfg.reset) {
                listener.log("Resetting into the new firmware...");
                xcp.programReset();
            }

            long elapsedMs = System.currentTimeMillis() - t0;
            listener.log(String.format("Flash complete: %d bytes in %d segments, %.1f s.",
                    total, segments.size(), elapsedMs / 1000.0));
            listener.log(String.format("Effective rate: %.2f ms per XCP frame (wall time), %.1f KB/s.",
                    elapsedMs / (double) Math.max(1, xcp.rttCount()),
                    total / (elapsedMs / 1000.0) / 1024.0));
            // Per-frame XCP round-trip histogram: shows where the time went -
            // host polling vs ECU-side processing vs bus (see XcpClient.rttStats).
            listener.log(xcp.rttStats());
            return new Result(segments.size(), total, verified, cfg.reset, elapsedMs);
        } finally {
            link.close();
        }
    }

    private XcpResponse connectWithRetries(XcpClient xcp, Config cfg) throws IOException {
        long deadline = System.currentTimeMillis() + cfg.connectTimeoutSec * 1000L;
        int attempt = 0;
        while (System.currentTimeMillis() < deadline) {
            attempt++;
            XcpResponse res = xcp.connect(0, 350);
            if (res != null) {
                return res;
            }
            listener.log(String.format("Waiting for bootloader... (attempt %d)", attempt));
            try {
                Thread.sleep(150);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                return null;
            }
        }
        return null;
    }

    private void eraseSegment(XcpClient xcp, SrecParser.Segment seg) throws IOException, FlashException {
        byte[] data = seg.data();
        for (int off = 0; off < data.length; off += XcpConstants.ERASE_CHUNK) {
            int chunk = Math.min(XcpConstants.ERASE_CHUNK, data.length - off);
            int addr = seg.base() + off;

            XcpResponse mta = xcp.setMta(addr);
            if (mta == null || !mta.isOk()) {
                throw new FlashException(String.format("SET_MTA(0x%08X) failed: %s", addr, mta));
            }
            XcpResponse clear = xcp.programClear(chunk, XcpConstants.ERASE_TIMEOUT_MS);
            if (clear == null || !clear.isOk()) {
                throw new FlashException(String.format("PROGRAM_CLEAR(0x%08X, %d) failed: %s", addr, chunk, clear));
            }
            listener.log(String.format("Erased 0x%08X..0x%08X", addr, addr + chunk));
        }
    }

    private void programSegment(XcpClient xcp, SrecParser.Segment seg) throws IOException, FlashException {
        byte[] data = seg.data();
        int off = 0;

        // One SET_MTA before the first frame; PROGRAM/PROGRAM_MAX auto-increment.
        XcpResponse mta = xcp.setMta(seg.base());
        if (mta == null || !mta.isOk()) {
            throw new FlashException(String.format("SET_MTA(0x%08X) failed: %s", seg.base(), mta));
        }

        if (!pipelineEnabled) {
            while (off < data.length) {
                int remaining = data.length - off;
                XcpResponse res;
                if (remaining >= XcpConstants.PROGRAM_MAX_PAYLOAD) {
                    byte[] chunk = new byte[XcpConstants.PROGRAM_MAX_PAYLOAD];
                    System.arraycopy(data, off, chunk, 0, chunk.length);
                    res = xcp.programMax(chunk);
                    off += chunk.length;
                } else {
                    byte[] chunk = new byte[remaining];
                    System.arraycopy(data, off, chunk, 0, chunk.length);
                    res = xcp.program(chunk);
                    off += chunk.length;
                }
                if (res == null || !res.isOk()) {
                    int failedAt = seg.base() + off;
                    throw new FlashException(String.format("PROGRAM at 0x%08X failed: %s", failedAt, res));
                }
            }
            return;
        }

        // Pipelined transfer: keep at most XcpClient.PIPELINE_WINDOW frames in
        // flight and pace the TX so the bootloader's ACKs interleave with our
        // frames on the bus. This amortizes the ~1 ms PCAN-USB dongle latency
        // over the whole stream instead of paying it once per 7-byte frame.
        // Safe by construction: the window cap means the ECU's 3 RX + 3 TX
        // mailboxes can never overflow/block (see XcpClient.PIPELINE_WINDOW).
        long nextSendAt = 0;
        while (off < data.length) {
            int remaining = data.length - off;

            if (remaining < XcpConstants.PROGRAM_MAX_PAYLOAD) {
                // Tail: settle the pipeline, then finish with a single-frame PROGRAM.
                drainPipeline(xcp);
                byte[] chunk = new byte[remaining];
                System.arraycopy(data, off, chunk, 0, chunk.length);
                XcpResponse res = xcp.program(chunk);
                if (res == null || !res.isOk()) {
                    throw new FlashException(String.format(
                            "PROGRAM at 0x%08X failed: %s", seg.base() + off, res));
                }
                off += chunk.length;
                break;
            }

            // Full window: collect one ACK first (the bootloader answers in order).
            if (xcp.getInFlight() >= XcpClient.PIPELINE_WINDOW) {
                XcpResponse res = xcp.readProgramAck(XcpConstants.PROGRAM_TIMEOUT_MS);
                if (res == null) {
                    throw new FlashException(String.format(
                            "PROGRAM_MAX at 0x%08X: no acknowledgement within %d ms "
                                    + "(pipelined mode, %d frames in flight)",
                            seg.base() + off, XcpConstants.PROGRAM_TIMEOUT_MS, xcp.getInFlight()));
                }
                if (!res.isOk()) {
                    throw new FlashException(String.format(
                            "PROGRAM_MAX at 0x%08X failed: %s", seg.base() + off, res));
                }
            }

            // Pace the TX so the ECU's ACK fits between our frames: without
            // this the dongle transmits back-to-back and the bootloader's
            // 3 ACK mailboxes saturate (see XcpClient.PIPELINE_WINDOW notes).
            paceUntil(nextSendAt, xcp);

            byte[] chunk = new byte[XcpConstants.PROGRAM_MAX_PAYLOAD];
            System.arraycopy(data, off, chunk, 0, chunk.length);
            xcp.writeProgramMax(chunk);
            nextSendAt = System.nanoTime() + pipelinePaceNanos;
            off += chunk.length;
        }
        drainPipeline(xcp);
    }

    /**
     * Collects all outstanding PROGRAM_MAX acknowledgements. The bootloader
     * answers in order, so this leaves the link quiet and the MTA at exactly
     * the next unwritten address.
     */
    private void drainPipeline(XcpClient xcp) throws IOException, FlashException {
        while (xcp.getInFlight() > 0) {
            XcpResponse res = xcp.readProgramAck(XcpConstants.PROGRAM_TIMEOUT_MS);
            if (res == null) {
                throw new FlashException("PROGRAM_MAX: no acknowledgement while draining the pipeline");
            }
            if (!res.isOk()) {
                throw new FlashException("PROGRAM_MAX failed while draining the pipeline: " + res);
            }
        }
    }

    /**
     * Waits until {@code targetNanos} (bus pacing) while opportunistically
     * collecting already-arrived acknowledgements, so the in-flight window
     * stays shallow and the per-frame RTT histogram stays accurate.
     */
    private void paceUntil(long targetNanos, XcpClient xcp) throws IOException, FlashException {
        long wait = targetNanos - System.nanoTime();
        while (wait > 0) {
            int slice = (int) Math.min(50, wait / 1_000_000 + 1);
            if (xcp.getInFlight() > 0) {
                XcpResponse res = xcp.readProgramAck(slice);
                if (res != null && !res.isOk()) {
                    throw new FlashException("PROGRAM_MAX failed during pipelining: " + res);
                }
            }
            wait = targetNanos - System.nanoTime();
            if (wait > 200_000) {
                LockSupport.parkNanos(Math.min(wait, 200_000));
            }
        }
    }

    private void verifySegments(XcpClient xcp, List<SrecParser.Segment> segments)
            throws IOException, FlashException {
        for (SrecParser.Segment seg : segments) {
            byte[] data = seg.data();

            XcpResponse mta = xcp.setMta(seg.base());
            if (mta == null || !mta.isOk()) {
                throw new FlashException(String.format("SET_MTA(0x%08X) failed: %s", seg.base(), mta));
            }
            XcpResponse checksum = xcp.buildChecksum(data.length);
            if (checksum == null || !checksum.isOk() || checksum.data().length < 8) {
                throw new FlashException(String.format(
                        "BUILD_CHECKSUM(0x%08X, %d) failed: %s", seg.base(), data.length, checksum));
            }
            int remoteSum = le32(checksum.data(), 4) & 0xFF;
            int localSum = 0;
            for (byte b : data) {
                localSum = (localSum + (b & 0xFF)) & 0xFF;
            }
            if (remoteSum != localSum) {
                throw new FlashException(String.format(
                        "Checksum mismatch at 0x%08X len %d: ECU says 0x%02X, image has 0x%02X",
                        seg.base(), data.length, remoteSum, localSum));
            }
        }
    }

    private static int le32(byte[] buf, int off) {
        return (buf[off] & 0xFF)
                | ((buf[off + 1] & 0xFF) << 8)
                | ((buf[off + 2] & 0xFF) << 16)
                | ((buf[off + 3] & 0xFF) << 24);
    }

    // ---------------------------------------------------------------- CLI

    public static void main(String[] args) {
        Config cfg = parseArgs(args);
        if (cfg == null) {
            System.exit(2);
            return;
        }
        try {
            TPCANHandle channel = resolveChannel(cfg.channel);
            OpenBltCanFlasher flasher = new OpenBltCanFlasher(new Listener() {
            });
            Result result = flasher.flash(new PcanLink(channel), cfg);
            System.out.println("Result: " + result.bytes + " bytes, verified=" + result.verified
                    + ", reset=" + result.reset);
        } catch (Throwable t) {
            System.err.println("FATAL: " + t.getMessage());
            t.printStackTrace(System.err);
            System.exit(1);
        }
    }

    private static Config parseArgs(String[] args) {
        Config cfg = new Config();
        cfg.srecPath = findDefaultSrec();
        for (int i = 0; i < args.length; i++) {
            switch (args[i]) {
                case "-h", "--help" -> {
                    printUsage();
                    return null;
                }
                case "--channel" -> {
                    if (i + 1 >= args.length) {
                        System.err.println("--channel requires a value");
                        return null;
                    }
                    try {
                        cfg.channel = Integer.parseInt(args[++i]);
                    } catch (NumberFormatException e) {
                        System.err.println("--channel requires a number, got " + args[i]);
                        return null;
                    }
                }
                case "--connect-timeout" -> {
                    if (i + 1 >= args.length) {
                        System.err.println("--connect-timeout requires a value");
                        return null;
                    }
                    try {
                        cfg.connectTimeoutSec = Integer.parseInt(args[++i]);
                    } catch (NumberFormatException e) {
                        System.err.println("--connect-timeout requires a number, got " + args[i]);
                        return null;
                    }
                }
                case "--no-verify" -> cfg.verify = false;
                case "--no-reset" -> cfg.reset = false;
                case "--probe" -> cfg.probeOnly = true;
                case "--verbose" -> cfg.verbose = true;
                case "--no-pipeline" -> cfg.pipeline = false;
                default -> {
                    if (args[i].startsWith("-")) {
                        System.err.println("Unknown option: " + args[i]);
                        return null;
                    }
                    cfg.srecPath = Paths.get(args[i]);
                }
            }
        }
        return cfg;
    }

    private static Path findDefaultSrec() {
        Path[] candidates = {
                Paths.get("firmware/build/rusefi.srec"),
                Paths.get("rusefi_update.srec"),
        };
        for (Path candidate : candidates) {
            if (Files.exists(candidate)) {
                return candidate;
            }
        }
        return candidates[0];
    }

    private static TPCANHandle resolveChannel(int channel) {
        try {
            return TPCANHandle.valueOf("PCAN_USBBUS" + channel);
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("Unknown PCAN channel " + channel);
        }
    }

    private static void printUsage() {
        System.out.println("""
                Usage: OpenBltCanFlasher [options] [firmware.srec]

                Flashes the connected ECU through the OpenBLT bootloader over CAN
                (XCP 1.0, extended ids 0x10667/0x107E1, 500 kbps) using a PCAN adapter.

                  --channel <n>          PCAN USB bus number (default 1)
                  --connect-timeout <s>  seconds to wait for the bootloader (default 8);
                                         covers the post-reset backdoor window and the
                                         app-side canOpenBLT jump
                  --no-verify            skip the post-program checksum verification
                  --no-reset             do not start the new firmware after programming
                  --probe                connect and print bootloader info, do not flash
                  --no-pipeline          fall back to one-request-one-reply programming
                                         (slow, useful to isolate link problems)
                  --verbose              log every sent/received CAN frame

                The default firmware path is firmware/build/rusefi.srec.

                Requires libpcanbasic_jni.dylib on java.library.path, e.g.
                  java -Djava.library.path=java_console -cp java_console/openblt_can/build/libs/openblt_can-all.jar \\
                       com.rusefi.openblt.OpenBltCanFlasher firmware/build/rusefi.srec
                """);
    }
}
