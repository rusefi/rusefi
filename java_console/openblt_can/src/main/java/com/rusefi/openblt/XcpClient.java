package com.rusefi.openblt;

import java.io.IOException;
import java.util.ArrayDeque;
import java.util.Arrays;

/**
 * Synchronous XCP 1.0 client for the rusEFI OpenBLT bootloader over CAN.
 *
 * Protocol notes (verified against ext/openblt/Target/Source/xcp.c):
 *  - one XCP packet per 8-byte CAN frame, no multi-frame assembly
 *  - addresses/lengths little-endian
 *  - SET_MTA / PROGRAM_CLEAR / BUILD_CHECKSUM read data[4..7], so their
 *    frames MUST carry DLC = 8
 *  - PROGRAM_MAX programs 7 bytes at the current MTA and auto-increments it
 *  - PROGRAM programs up to 6 bytes; PROGRAM_RESET resets into the new app
 *    and (by design) does not send a response
 */
public class XcpClient {
    /**
     * Max un-acknowledged PROGRAM_MAX frames allowed in flight (pipelining).
     *
     * Hard limit on this hardware: the ChibiOS CANv1 driver has NO software
     * RX buffer - frames sit in the bxCAN hardware FIFO0+FIFO1 (3+3 slots).
     * The bootloader's ACK transmits fill its 3 TX mailboxes and the 4th ACK
     * blocks the main loop (canTransmitTimeout), so while a burst is on the
     * wire the ECU stores at most 3 already-processed + 1 in-progress + 6 in
     * the FIFOs = 10 frames before dropping. 8 leaves 2 slots of margin, and
     * the host-side pacing (620 us/frame) keeps the real in-flight depth at
     * 2-4 frames anyway - the window only caps the initial fill and covers
     * the ~1 ms PCAN-USB latency of the first ACKs.
     */
    public static final int PIPELINE_WINDOW = 8;
    private final CanLink link;
    private final int txId;
    private final boolean extended;
    private final int rxId;
    private final int defaultTimeoutMs;
    /** Optional frame tracer (e.g. --verbose diagnostics). */
    private final java.util.function.Consumer<String> frameLog;

    // ---- per-frame round-trip timing (flash-speed diagnostics) -------
    // Measured from the request write until the matching XCP reply arrives.
    // The histogram separates the program-frame hot path (<2 ms) from slow
    // replies (erase, connect) so the next flash run shows WHERE the time
    // goes: host polling vs ECU-side processing vs bus.
    private long rttCount;
    private long rttSumNanos;
    private long rttMaxNanos;
    /** buckets: <1, 1-2, 2-3, 3-4, 4-5, 5-7, 7-10, >=10 ms */
    private final long[] rttBuckets = new long[8];

    // ---- pipelined PROGRAM_MAX bookkeeping -----------------------------
    // Send timestamps of frames that were written but not yet acknowledged.
    // The bootloader ACKs in order, so the head of the queue belongs to the
    // next expected reply; the queue depth is the number of frames in flight.
    private final ArrayDeque<Long> inflightSendTimes = new ArrayDeque<>();

    /** Number of PROGRAM_MAX frames sent but not yet acknowledged. */
    public int getInFlight() {
        return inflightSendTimes.size();
    }

    /** Total replies received so far (pipelined + single-frame). */
    public long rttCount() {
        return rttCount;
    }

    private void recordRtt(long startNanos) {
        long rtt = System.nanoTime() - startNanos;
        rttCount++;
        rttSumNanos += rtt;
        if (rtt > rttMaxNanos) {
            rttMaxNanos = rtt;
        }
        long ms = rtt / 1_000_000L;
        int bucket = ms < 1 ? 0 : ms < 2 ? 1 : ms < 3 ? 2 : ms < 4 ? 3
                : ms < 5 ? 4 : ms < 7 ? 5 : ms < 10 ? 6 : 7;
        rttBuckets[bucket]++;
    }

    public String rttStats() {
        if (rttCount == 0) {
            return "XCP RTT: no frames";
        }
        return String.format(
                "XCP RTT: %d frames, avg %.2f ms, max %.2f ms; <1/1-2/2-3/3-4/4-5/5-7/7-10/>=10 ms: %s",
                rttCount, rttSumNanos / rttCount / 1_000_000.0, rttMaxNanos / 1_000_000.0,
                Arrays.toString(rttBuckets));
    }

    public XcpClient(CanLink link, int txId, boolean extended, int rxId, int defaultTimeoutMs) {
        this(link, txId, extended, rxId, defaultTimeoutMs, null);
    }

    public XcpClient(CanLink link, int txId, boolean extended, int rxId, int defaultTimeoutMs,
                     java.util.function.Consumer<String> frameLog) {
        this.link = link;
        this.txId = txId;
        this.extended = extended;
        this.rxId = rxId;
        this.defaultTimeoutMs = defaultTimeoutMs;
        this.frameLog = frameLog;
    }

    public int getTxId() {
        return txId;
    }

    public int getRxId() {
        return rxId;
    }

    public boolean isExtended() {
        return extended;
    }

    /** CONNECT with connect mode 0 (normal). This frame doubles as the
     *  app-side "jump to bootloader" trigger: it is DLC=2, which is exactly
     *  what the running firmware listens for when canOpenBLT is enabled. */
    public XcpResponse connect(int connectMode) throws IOException {
        return connect(connectMode, defaultTimeoutMs);
    }

    public XcpResponse connect(int connectMode, int timeoutMs) throws IOException {
        return request(new byte[]{(byte) XcpConstants.CMD_CONNECT, (byte) connectMode}, timeoutMs);
    }

    public XcpResponse disconnect() throws IOException {
        return request(new byte[]{(byte) XcpConstants.CMD_DISCONNECT}, defaultTimeoutMs);
    }

    public XcpResponse getStatus() throws IOException {
        return request(new byte[]{(byte) XcpConstants.CMD_GET_STATUS}, defaultTimeoutMs);
    }

    public XcpResponse getStatus(int timeoutMs) throws IOException {
        return request(new byte[]{(byte) XcpConstants.CMD_GET_STATUS}, timeoutMs);
    }

    /** GET_ID response reports the station-id length at data[4..7] (LE). */
    public XcpResponse getId() throws IOException {
        return request(new byte[]{(byte) XcpConstants.CMD_GET_ID}, defaultTimeoutMs);
    }

    /** The '?' probe: bootloader answers with its plain-text version banner
     *  when the frame is exactly one byte. */
    public XcpResponse probeVersion() throws IOException {
        return request(new byte[]{'?'}, defaultTimeoutMs);
    }

    public XcpResponse setMta(int address) throws IOException {
        byte[] cmd = new byte[XcpConstants.CTO_LEN];
        cmd[0] = (byte) XcpConstants.CMD_SET_MTA;
        putLe32(cmd, 4, address);
        return request(cmd, defaultTimeoutMs);
    }

    /** Erases len bytes starting at the current MTA. Call setMta() first. */
    public XcpResponse programClear(int len, int timeoutMs) throws IOException {
        byte[] cmd = new byte[XcpConstants.CTO_LEN];
        cmd[0] = (byte) XcpConstants.CMD_PROGRAM_CLEAR;
        putLe32(cmd, 4, len);
        return request(cmd, timeoutMs);
    }

    /** Programs 7 bytes at the current MTA; the target auto-increments MTA. */
    public XcpResponse programMax(byte[] data7) throws IOException {
        writeProgramMax(data7);
        return readProgramAck(XcpConstants.PROGRAM_TIMEOUT_MS);
    }

    /**
     * Sends one PROGRAM_MAX frame without waiting for the reply. Combined with
     * {@link #readProgramAck(int)} this builds a pipelined transfer: the caller
     * keeps at most {@link #PIPELINE_WINDOW} frames in flight and drains the
     * in-order acknowledgements.
     */
    public void writeProgramMax(byte[] data7) throws IOException {
        if (data7.length != XcpConstants.PROGRAM_MAX_PAYLOAD) {
            throw new IllegalArgumentException("PROGRAM_MAX requires exactly 7 data bytes");
        }
        byte[] cmd = new byte[XcpConstants.CTO_LEN];
        cmd[0] = (byte) XcpConstants.CMD_PROGRAM_MAX;
        System.arraycopy(data7, 0, cmd, 1, data7.length);
        if (frameLog != null) {
            frameLog.accept("TX " + new CanFrame(txId, extended, cmd));
        }
        inflightSendTimes.addLast(System.nanoTime());
        link.write(new CanFrame(txId, extended, cmd));
    }

    /**
     * Waits for the next pipelined reply (bootloader ACKs PROGRAM_MAX frames
     * in order). Call only while {@link #getInFlight()} is non-zero.
     *
     * @return the response, or null when the timeout expired.
     */
    public XcpResponse readProgramAck(int timeoutMs) throws IOException {
        long deadline = System.currentTimeMillis() + timeoutMs;
        while (System.currentTimeMillis() < deadline) {
            int remaining = (int) Math.min(50, deadline - System.currentTimeMillis());
            if (remaining <= 0) {
                break;
            }
            CanFrame frame = link.readFrame(remaining);
            XcpResponse res = matchReply(frame);
            if (res != null) {
                return res;
            }
        }
        return null;
    }

    /**
     * One immediate receive attempt for a pipelined reply; returns null when
     * nothing is queued. Cheap (no spin, no timeout) - safe to call from the
     * TX pacing loop on every frame slot.
     */
    public XcpResponse pollProgramAck() throws IOException {
        return matchReply(link.pollFrame());
    }

    /** Interprets a received frame as a pipelined reply, or null. */
    private XcpResponse matchReply(CanFrame frame) throws IOException {
        if (frame == null) {
            return null;
        }
        if (frameLog != null) {
            frameLog.accept("RX " + frame);
        }
        // Skip anything that is not a reply from the bootloader on its TX id
        // (the still-running app may be broadcasting on other ids).
        if (frame.id() != rxId || frame.extended() != extended) {
            return null;
        }
        byte[] data = frame.data();
        if (data.length == 0) {
            return null;
        }
        int pid = data[0] & 0xFF;
        if (pid != XcpConstants.PID_RES && pid != XcpConstants.PID_ERR) {
            // Unknown packet id: ignore.
            return null;
        }
        Long sentAt = inflightSendTimes.pollFirst();
        if (sentAt != null) {
            recordRtt(sentAt);
        }
        return pid == XcpConstants.PID_RES
                ? XcpResponse.ok(data)
                : XcpResponse.error(data);
    }

    /** Programs 1..6 bytes at the current MTA; the target auto-increments MTA. */
    public XcpResponse program(byte[] data) throws IOException {
        if (data.length == 0 || data.length > XcpConstants.PROGRAM_PAYLOAD) {
            throw new IllegalArgumentException("PROGRAM requires 1.." + XcpConstants.PROGRAM_PAYLOAD + " data bytes");
        }
        byte[] cmd = new byte[2 + data.length];
        cmd[0] = (byte) XcpConstants.CMD_PROGRAM;
        cmd[1] = (byte) data.length;
        System.arraycopy(data, 0, cmd, 2, data.length);
        return request(cmd, XcpConstants.PROGRAM_TIMEOUT_MS);
    }

    /** PROGRAM with size 0: the bootloader calls NvmDone()/FlashDone(), which
     *  flushes the last partial flash line on platforms that buffer writes
     *  (H7 ECC lines, AT32 one-shot words). */
    public XcpResponse programDone() throws IOException {
        return request(new byte[]{(byte) XcpConstants.CMD_PROGRAM, 0x00}, XcpConstants.PROGRAM_TIMEOUT_MS);
    }

    // ---- rusEFI batch-programming extension ------------------------------

    /** Declares a deferred-ACK batch of len bytes at the current MTA. */
    public XcpResponse programBatchStart(int len) throws IOException {
        if (len <= 0 || len > XcpConstants.BATCH_MAX) {
            throw new IllegalArgumentException("batch length must be 1.." + XcpConstants.BATCH_MAX);
        }
        byte[] cmd = new byte[XcpConstants.CTO_LEN];
        cmd[0] = (byte) XcpConstants.CMD_PROGRAM_BATCH;
        putLe32(cmd, 4, len);
        return request(cmd, XcpConstants.PROGRAM_TIMEOUT_MS);
    }

    /**
     * PROGRAM_MAX without per-frame acknowledgement bookkeeping: used inside
     * a batch, where only ONE response arrives when the whole batch completed.
     */
    public void writeProgramMaxBatch(byte[] data7) throws IOException {
        if (data7.length != XcpConstants.PROGRAM_MAX_PAYLOAD) {
            throw new IllegalArgumentException("PROGRAM_MAX requires exactly 7 data bytes");
        }
        byte[] cmd = new byte[XcpConstants.CTO_LEN];
        cmd[0] = (byte) XcpConstants.CMD_PROGRAM_MAX;
        System.arraycopy(data7, 0, cmd, 1, data7.length);
        if (frameLog != null) {
            frameLog.accept("TX " + new CanFrame(txId, extended, cmd));
        }
        link.write(new CanFrame(txId, extended, cmd));
    }

    /**
     * Waits for the single batch acknowledgement (the bootloader responds once
     * per batch, after programming the whole buffer). RTT is measured from the
     * batch start.
     */
    public XcpResponse readBatchAck(int timeoutMs, long batchStartNanos) throws IOException {
        long deadline = System.currentTimeMillis() + timeoutMs;
        while (System.currentTimeMillis() < deadline) {
            int remaining = (int) Math.min(50, deadline - System.currentTimeMillis());
            if (remaining <= 0) {
                break;
            }
            CanFrame frame = link.readFrame(remaining);
            if (frame == null) {
                continue;
            }
            if (frameLog != null) {
                frameLog.accept("RX " + frame);
            }
            if (frame.id() != rxId || frame.extended() != extended) {
                continue;
            }
            byte[] data = frame.data();
            if (data.length == 0) {
                continue;
            }
            int pid = data[0] & 0xFF;
            if (pid == XcpConstants.PID_RES || pid == XcpConstants.PID_ERR) {
                recordRtt(batchStartNanos);
                return pid == XcpConstants.PID_RES
                        ? XcpResponse.ok(data)
                        : XcpResponse.error(data);
            }
        }
        return null;
    }

    /**
     * rusEFI extension: requests a CAN baudrate switch. The bootloader answers
     * at the CURRENT speed and switches ~5 ms later; the caller must then
     * reconfigure the adapter (see CanLink.setBaudrate).
     *
     * @param rate XcpConstants.BAUD_500K or BAUD_1M
     */
    public XcpResponse setCanBaudrate(int rate) throws IOException {
        if (rate != XcpConstants.BAUD_500K && rate != XcpConstants.BAUD_1M) {
            throw new IllegalArgumentException("baudrate code must be 0 (500k) or 1 (1M)");
        }
        return request(new byte[]{(byte) XcpConstants.CMD_SET_CAN_BAUDRATE, (byte) rate},
                XcpConstants.COMMAND_TIMEOUT_MS);
    }

    /** Resets into the freshly programmed application. No response expected:
     *  the target jumps to the user program before replying. */
    public void programReset() throws IOException {
        link.write(new CanFrame(txId, extended, new byte[]{(byte) XcpConstants.CMD_PROGRAM_RESET}));
    }

    /** Additive (ADD11) checksum of len bytes at the current MTA. */
    public XcpResponse buildChecksum(int len) throws IOException {
        byte[] cmd = new byte[XcpConstants.CTO_LEN];
        cmd[0] = (byte) XcpConstants.CMD_BUILD_CHECKSUM;
        putLe32(cmd, 4, len);
        return request(cmd, XcpConstants.COMMAND_TIMEOUT_MS);
    }

    /**
     * Sends one command frame and waits for the matching response.
     *
     * @return the response, or null when the timeout expired.
     */
    public XcpResponse request(byte[] cmd, int timeoutMs) throws IOException {
        CanFrame tx = new CanFrame(txId, extended, cmd);
        if (frameLog != null) {
            frameLog.accept("TX " + tx);
        }
        long rttStart = System.nanoTime();
        link.write(tx);

        long deadline = System.currentTimeMillis() + timeoutMs;
        while (System.currentTimeMillis() < deadline) {
            int remaining = (int) Math.min(50, deadline - System.currentTimeMillis());
            if (remaining <= 0) {
                break;
            }
            CanFrame frame = link.readFrame(remaining);
            if (frame == null) {
                continue;
            }
            if (frameLog != null) {
                frameLog.accept("RX " + frame);
            }
            // Skip anything that is not a reply from the bootloader on its TX id
            // (the still-running app may be broadcasting on other ids).
            if (frame.id() != rxId || frame.extended() != extended) {
                continue;
            }
            byte[] data = frame.data();
            if (data.length == 0) {
                continue;
            }
            int pid = data[0] & 0xFF;
            if (pid == XcpConstants.PID_RES) {
                recordRtt(rttStart);
                return XcpResponse.ok(data);
            }
            if (pid == XcpConstants.PID_ERR) {
                recordRtt(rttStart);
                return XcpResponse.error(data);
            }
            // Unknown packet id: ignore and keep waiting.
        }
        return null;
    }

    private static void putLe32(byte[] buf, int off, int value) {
        buf[off] = (byte) value;
        buf[off + 1] = (byte) (value >> 8);
        buf[off + 2] = (byte) (value >> 16);
        buf[off + 3] = (byte) (value >> 24);
    }
}
