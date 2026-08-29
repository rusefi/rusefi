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
 * running, the bootloader's post-reset backdoor window (1 s on m74_9, 6 s
 * after a 1 Mbit baudrate-switch reboot) answers the same retried CONNECT.
 * So one retry loop covers both entry paths.
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
     * Kept as the documented alternative: the DEFAULT is 0 (window-only flow
     * control), which measured fastest on the bench - see Config.
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
        boolean batch = true;
        boolean oneMbit = false;
        /**
         * Escape hatch to actually attempt the 1 Mbit switch (--1mbit-force).
         * Disabled by default: the MacCAN/PCAN-USB TX path corrupts frames
         * after the mid-session switch - measured deterministically 2026-08-29
         * (the image checksum was off by the SAME +40 in two runs, while every
         * frame was CRC-valid and ACKed by the ECU - the wrong bytes were
         * formed host-side, so the firmware cannot fix it). Even on a working
         * adapter, 1 Mbit costs two reboots plus the ACK-per-frame program
         * mode, which measures SLOWER than the 500k batch (~57 s vs ~45 s).
         */
        boolean oneMbitAllowed = false;
        /**
         * Bound on the CONNECT retry loop after a baudrate-switch reboot.
         * The bootloader's backdoor window is 6 s at 1 Mbit and 500 ms at
         * 500k, so the reconnect must not sleep between attempts.
         */
        int baudReconnectMs = 3000;
        /**
         * Total time after the SET_CAN_BAUDRATE request by which the ECU has
         * recovered to 500 kbit on its own (bootloader no-traffic fallback at
         * 5 s + app backdoor). The host sleeps the remainder and reconnects
         * at 500k.
         */
        int baudFallbackAfterMs = 7000;
        /**
         * Pipelined PROGRAM_MAX TX spacing in nanoseconds. 0 (default) = no
         * explicit pacing: the 8-frame window alone throttles the stream, the
         * host bursts frames and the ACK stream paces the bus at its ~0.68 ms
         * frame+ACK cycle - measured 68.5 s for 694 KB (0.69 ms/frame), the
         * fastest host-only mode. A positive value (e.g. 620000 = 620 us)
         * spaces frames out; measured slower (90.8 s) because the blocking
         * MacCAN write overlaps less efficiently.
         */
        long pipelinePaceNanos = 0;
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
    /** Set from Config before programming: enables the deferred-ACK batch path. */
    private boolean batchEnabled = true;
    /** Set from Config before programming: switch the link to 1 Mbit for the program phase. */
    private boolean oneMbitEnabled = false;
    /** Set from Config: CONNECT retry bound after a baudrate-switch reboot. */
    private int baudReconnectMs = 3000;
    /** Set from Config: time after the switch request by which the ECU is back at 500k. */
    private int baudFallbackAfterMs = 7000;
    /** Set from Config before programming: pipelined TX spacing in nanoseconds. */
    private long pipelinePaceNanos = 0;
    // Per-phase wall-time accounting for the program loop (bench diagnostics:
    // shows whether the residual ~1.1 ms/frame sits in the CAN write, the
    // pacing sleep or the ACK wait).
    private long writeNanos;
    private long paceNanos;
    private long ackWaitNanos;

    public OpenBltCanFlasher(Listener listener) {
        this.listener = listener;
    }

    public Result flash(CanLink link, Config cfg) throws IOException, FlashException {
        long t0 = System.currentTimeMillis();
        pipelineEnabled = cfg.pipeline;
        batchEnabled = cfg.batch;
        oneMbitEnabled = cfg.oneMbit;
        baudReconnectMs = cfg.baudReconnectMs;
        baudFallbackAfterMs = cfg.baudFallbackAfterMs;
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
            if (oneMbitEnabled && !cfg.oneMbitAllowed) {
                listener.log("1 Mbit is disabled: the MacCAN/PCAN-USB TX path corrupts frames after the"
                        + " mid-session switch (measured deterministically 2026-08-29 - the same checksum"
                        + " offset in two runs, with every frame ACKed). Flashing at 500 kbit (batch, ~45 s)."
                        + " Use --1mbit-force to override.");
                oneMbitEnabled = false;
            }
            if (oneMbitEnabled) {
                if (!switchBaudrate(xcp, link, XcpConstants.BAUD_1M)) {
                    // 1 Mbit is not usable with this adapter/driver: the ECU
                    // recovered to 500 kbit by itself and we reconnected there.
                    // Continue the flash at 500k instead of failing.
                    oneMbitEnabled = false;
                } else if (batchEnabled) {
                    // Deferred-ACK batch is UNSAFE at 1 Mbit: the batch mode
                    // sends ~293 PROGRAM_MAX frames with a single ACK, and
                    // ONE lost frame silently shifts the whole batch buffer.
                    // Measured on the bench (2026-08-29): the 1M batch run
                    // corrupted the image - only the final checksum caught it
                    // (ADD11 off by 40). At 1 Mbit the wire margin is 4x
                    // smaller, so dropped frames are a real risk. Use the
                    // ACK-per-frame pipelined mode instead: any lost frame
                    // aborts loudly instead of silently corrupting the image.
                    batchEnabled = false;
                    listener.log("1 Mbit: using the ACK-per-frame pipelined mode (a lost frame in"
                            + " deferred-ACK batch mode would silently corrupt the image)");
                }
            }
            try {
                for (SrecParser.Segment seg : segments) {
                    eraseSegment(xcp, seg);
                    programSegment(xcp, seg);
                    done += seg.data().length;
                    listener.progress(done, total);
                }
            } finally {
                // The car/console bus is 500k: always come back before the
                // verify/reset phase, even when programming threw. A failed
                // switch-back must not mask the original error: the bootloader
                // watchdog recovers the ECU at 500k on its own.
                if (oneMbitEnabled) {
                    try {
                        switchBaudrate(xcp, link, XcpConstants.BAUD_500K);
                    } catch (IOException | FlashException e) {
                        listener.log("WARNING: could not switch back to 500 kbit: " + e.getMessage()
                                + " - the bootloader's no-traffic fallback returns it to 500k by itself.");
                    }
                }
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
            long frames = Math.max(1, xcp.rttCount());
            listener.log(String.format("Program loop timing: write %.1f us/frame, pace %.1f us/frame, ack-wait %.1f us/frame.",
                    writeNanos / 1000.0 / frames, paceNanos / 1000.0 / frames, ackWaitNanos / 1000.0 / frames));
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
        if (batchEnabled) {
            programSegmentBatched(xcp, seg);
            return;
        }

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
                long ackStart = System.nanoTime();
                XcpResponse res = xcp.readProgramAck(XcpConstants.PROGRAM_TIMEOUT_MS);
                ackWaitNanos += System.nanoTime() - ackStart;
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
            // Pure sleep: draining is left to the window check above so the
            // hot loop makes zero JNI reads.
            paceUntil(nextSendAt);

            byte[] chunk = new byte[XcpConstants.PROGRAM_MAX_PAYLOAD];
            System.arraycopy(data, off, chunk, 0, chunk.length);
            long writeStart = System.nanoTime();
            xcp.writeProgramMax(chunk);
            writeNanos += System.nanoTime() - writeStart;
            // Schedule from the frame START, not from after the write: the
            // blocking MacCAN Write (~0.4 ms) then overlaps with the pace
            // slot instead of adding to it. The bus self-arbitrates at its
            // ~0.68 ms frame+ACK cycle; the dongle buffers the small surplus.
            nextSendAt = writeStart + pipelinePaceNanos;
            off += chunk.length;
        }
        drainPipeline(xcp);
    }

    /**
     * Deferred-ACK batch programming (plan B): the image goes out in 2 KB
     * batches of PROGRAM_MAX frames with ONE acknowledgement per batch, so
     * the per-frame ACK traffic disappears from the bus. The bootloader
     * buffers the batch in RAM and writes it to flash in one NvmWrite, which
     * also amortizes the AT32 per-word program cost. The host side needs no
     * flow control: the blocking MacCAN write (~0.3 ms/frame) is slower than
     * the bus, and the bootloader consumes frames far faster than either.
     */
    private void programSegmentBatched(XcpClient xcp, SrecParser.Segment seg) throws IOException, FlashException {
        byte[] data = seg.data();
        int off = 0;

        while (off < data.length) {
            int batchLen = Math.min(XcpConstants.BATCH_MAX, data.length - off);

            // Deterministic address per batch; PROGRAM_MAX auto-increments the
            // bootloader MTA by 7 per frame, so a non-multiple-of-7 batch end
            // would drift - SET_MTA fixes the next batch start regardless.
            XcpResponse mta = xcp.setMta(seg.base() + off);
            if (mta == null || !mta.isOk()) {
                throw new FlashException(String.format("SET_MTA(0x%08X) failed: %s", seg.base() + off, mta));
            }

            long batchStart = System.nanoTime();
            XcpResponse start = xcp.programBatchStart(batchLen);
            if (start == null || !start.isOk()) {
                throw new FlashException(String.format(
                        "PROGRAM_BATCH(0x%08X, %d) failed: %s - is the ECU running the new "
                                + "bootloader? (old bootloaders reject the batch command; use --no-batch)",
                        seg.base() + off, batchLen, start));
            }

            int sent = 0;
            while (sent < batchLen) {
                int chunkLen = Math.min(XcpConstants.PROGRAM_MAX_PAYLOAD, batchLen - sent);
                byte[] chunk = new byte[XcpConstants.PROGRAM_MAX_PAYLOAD];
                System.arraycopy(data, off + sent, chunk, 0, chunkLen);
                xcp.writeProgramMaxBatch(chunk);
                sent += chunkLen;
            }

            XcpResponse ack = xcp.readBatchAck(XcpConstants.PROGRAM_TIMEOUT_MS, batchStart);
            if (ack == null) {
                throw new FlashException(String.format(
                        "PROGRAM_BATCH at 0x%08X: no acknowledgement within %d ms",
                        seg.base() + off, XcpConstants.PROGRAM_TIMEOUT_MS));
            }
            if (!ack.isOk()) {
                throw new FlashException(String.format(
                        "PROGRAM_BATCH at 0x%08X failed: %s", seg.base() + off, ack));
            }

            off += batchLen;
        }
    }

    /**
     * Switches both sides of the link to a new baudrate (rusEFI extension).
     *
     * The switch is a REBOOT on the ECU side: SET_CAN_BAUDRATE is answered at
     * the old speed, then the bootloader stores the request in SharedParams
     * slot 4, resets and comes back up at the new speed (a runtime
     * canStop/canStart wedges the AT32 CAN peripheral - that is why the
     * switch is a reboot). The XCP session dies with the reboot, so the host
     * re-initializes its adapter and re-CONNECTs instead of GET_STATUS.
     *
     * The car/console bus stays at 500k: the 1 Mbit phase covers only the
     * erase+program window and the caller switches back before verify/reset.
     *
     * Safety: when the host cannot follow to 1 Mbit, the ECU recovers by
     * itself - the bootloader's 5 s no-traffic fallback reboots it back to
     * 500k, and after that its 500 ms backdoor exits into the app (which
     * answers a CONNECT trigger at 500k as well).
     *
     * @return true when the link is confirmed at the requested rate; false
     *         when a 1 Mbit switch failed and the session was re-established
     *         at 500k (only possible for rate == BAUD_1M).
     */
    private boolean switchBaudrate(XcpClient xcp, CanLink link, int rate) throws IOException, FlashException {
        String speed = rate == XcpConstants.BAUD_1M ? "1 Mbit" : "500 kbit";
        listener.log("Switching the link to " + speed + "...");
        long switchStartedAt = System.currentTimeMillis();
        XcpResponse res = xcp.setCanBaudrate(rate);
        if (res == null || !res.isOk()) {
            throw new FlashException("SET_CAN_BAUDRATE(" + rate + ") failed: " + res);
        }
        // The ACK went out at the old speed. Give it time to leave the wire
        // and the ECU to reboot, then point the adapter at the new speed.
        sleepMs(50);
        try {
            link.setBaudrate(rate);
        } catch (IOException e) {
            // The adapter/driver cannot do the requested rate (MacCAN does
            // not support 1 Mbit): the ECU is at the requested speed, but no
            // valid frame will ever arrive, so its own fallback brings it
            // back to 500k. Wait for that and reconnect there.
            if (rate != XcpConstants.BAUD_1M) {
                throw new FlashException("Adapter cannot switch to " + speed + ": " + e.getMessage()
                        + ". Power-cycle the ECU and rerun WITHOUT --1mbit.", e);
            }
            listener.log("Adapter cannot switch to 1 Mbit (" + e.getMessage()
                    + ") - waiting for the ECU's own fallback to 500 kbit...");
            return awaitEcuFallbackTo500K(xcp, link, switchStartedAt);
        }
        // Re-CONNECT at the new speed. The bootloader's backdoor window is
        // short, so retry immediately - no sleeps between attempts; each
        // failed attempt costs the CONNECT timeout.
        if (reconnect(xcp, baudReconnectMs)) {
            listener.log("Link switched to " + speed + ".");
            return true;
        }
        if (rate != XcpConstants.BAUD_1M) {
            throw new FlashException("No CONNECT response at 500 kbit after the switch-back. "
                    + "Power-cycle the ECU and rerun WITHOUT --1mbit.");
        }
        listener.log("No response at 1 Mbit - waiting for the ECU's own fallback to 500 kbit...");
        return awaitEcuFallbackTo500K(xcp, link, switchStartedAt);
    }

    /** Tight CONNECT retry loop: the bootloader's post-reboot backdoor
     *  window is short (6 s at 1 Mbit, 500 ms at 500k), so the attempts must
     *  not be separated by sleeps. A CONNECT also triggers the running app
     *  to jump back into the bootloader, so this covers every ECU state. */
    private boolean reconnect(XcpClient xcp, int timeoutMs) throws IOException {
        long deadline = System.currentTimeMillis() + timeoutMs;
        while (System.currentTimeMillis() < deadline) {
            XcpResponse con = xcp.connect(0, 300);
            if (con != null && con.isOk()) {
                return true;
            }
        }
        return false;
    }

    /**
     * Waits until the ECU must have recovered to 500k by itself (bootloader
     * no-traffic fallback at 5 s, then the 500 ms backdoor exits into the
     * app, which answers the CONNECT trigger too), re-points the adapter at
     * 500k and re-CONNECTs.
     *
     * @return false: the flash continues at 500k, the 1 Mbit phase is over.
     */
    private boolean awaitEcuFallbackTo500K(XcpClient xcp, CanLink link, long switchStartedAt)
            throws IOException, FlashException {
        long remaining = Math.max(0,
                baudFallbackAfterMs - (System.currentTimeMillis() - switchStartedAt));
        sleepMs(remaining);
        try {
            link.setBaudrate(XcpConstants.BAUD_500K);
        } catch (IOException e) {
            throw new FlashException("Adapter cannot return to 500 kbit: " + e.getMessage()
                    + ". Power-cycle the ECU and rerun WITHOUT --1mbit.", e);
        }
        if (!reconnect(xcp, baudReconnectMs)) {
            throw new FlashException("1 Mbit switch failed and the ECU does not answer at 500 kbit either. "
                    + "Power-cycle the ECU and rerun WITHOUT --1mbit.");
        }
        listener.log("ECU recovered at 500 kbit - continuing the flash at 500 kbit.");
        return false;
    }

    private static void sleepMs(long ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
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
     * Waits until {@code targetNanos} (bus pacing). Pure sleep - no CAN reads:
     * the ACK drain happens in the window check before each send, so the hot
     * loop makes zero JNI calls and the pace is as close to wall clock as
     * parkNanos allows.
     */
    private void paceUntil(long targetNanos) {
        long t0 = System.nanoTime();
        while (true) {
            long wait = targetNanos - System.nanoTime();
            if (wait <= 0) {
                break;
            }
            LockSupport.parkNanos(Math.min(wait, 200_000L));
        }
        paceNanos += System.nanoTime() - t0;
    }

    private void verifySegments(XcpClient xcp, List<SrecParser.Segment> segments)
            throws IOException, FlashException {
        for (SrecParser.Segment seg : segments) {
            byte[] data = seg.data();

            // Verify in 32 KB chunks: on a mismatch the error names the exact
            // chunk address and both checksums, which localizes any corruption
            // (e.g. the 1 Mbit host-side TX corruption) to a precise window
            // in ONE bench run instead of guessing over the whole image.
            for (int off = 0; off < data.length; off += XcpConstants.ERASE_CHUNK) {
                int len = Math.min(XcpConstants.ERASE_CHUNK, data.length - off);

                XcpResponse mta = xcp.setMta(seg.base() + off);
                if (mta == null || !mta.isOk()) {
                    throw new FlashException(String.format(
                            "SET_MTA(0x%08X) failed: %s", seg.base() + off, mta));
                }
                XcpResponse checksum = xcp.buildChecksum(len);
                if (checksum == null || !checksum.isOk() || checksum.data().length < 8) {
                    throw new FlashException(String.format(
                            "BUILD_CHECKSUM(0x%08X, %d) failed: %s", seg.base() + off, len, checksum));
                }
                int remoteSum = le32(checksum.data(), 4) & 0xFF;
                int localSum = 0;
                for (int i = off; i < off + len; i++) {
                    localSum = (localSum + (data[i] & 0xFF)) & 0xFF;
                }
                if (remoteSum != localSum) {
                    throw new FlashException(String.format(
                            "Checksum mismatch at 0x%08X len %d: ECU says 0x%02X, image has 0x%02X",
                            seg.base() + off, len, remoteSum, localSum));
                }
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
                case "--no-batch" -> cfg.batch = false;
                case "--1mbit" -> cfg.oneMbit = true;
                case "--1mbit-force" -> {
                    cfg.oneMbit = true;
                    cfg.oneMbitAllowed = true;
                }
                case "--pace-us" -> {
                    if (i + 1 >= args.length) {
                        System.err.println("--pace-us requires a value");
                        return null;
                    }
                    try {
                        cfg.pipelinePaceNanos = Long.parseLong(args[++i]) * 1000L;
                    } catch (NumberFormatException e) {
                        System.err.println("--pace-us requires a number, got " + args[i]);
                        return null;
                    }
                }
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
                  --no-batch             disable deferred-ACK batch programming (plan B),
                                         use the window-pipelined PROGRAM_MAX mode
                  --1mbit                flash at 500 kbit - the 1 Mbit mode is
                                         disabled on this adapter/driver (MacCAN
                                         corrupts TX after the mid-session switch,
                                         measured 2026-08-29); kept for compatibility
                  --1mbit-force          actually attempt the 1 Mbit switch for the
                                         erase+program phase (the console/car bus
                                         stays at 500k; the program phase uses the
                                         ACK-per-frame mode - deferred-ACK batch is
                                         unsafe there)
                  --pace-us <n>          pipelined frame-to-frame spacing in microseconds
                                         (default 0: window-only flow control, fastest);
                                         620 = the old spaced-out mode
                  --verbose              log every sent/received CAN frame

                The default firmware path is firmware/build/rusefi.srec.

                Requires libpcanbasic_jni.dylib on java.library.path, e.g.
                  java -Djava.library.path=java_console -cp java_console/openblt_can/build/libs/openblt_can-all.jar \\
                       com.rusefi.openblt.OpenBltCanFlasher firmware/build/rusefi.srec
                """);
    }
}
