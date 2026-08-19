package com.rusefi.openblt;

import java.io.IOException;

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
    private final CanLink link;
    private final int txId;
    private final boolean extended;
    private final int rxId;
    private final int defaultTimeoutMs;
    /** Optional frame tracer (e.g. --verbose diagnostics). */
    private final java.util.function.Consumer<String> frameLog;

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
        if (data7.length != XcpConstants.PROGRAM_MAX_PAYLOAD) {
            throw new IllegalArgumentException("PROGRAM_MAX requires exactly 7 data bytes");
        }
        byte[] cmd = new byte[XcpConstants.CTO_LEN];
        cmd[0] = (byte) XcpConstants.CMD_PROGRAM_MAX;
        System.arraycopy(data7, 0, cmd, 1, data7.length);
        return request(cmd, XcpConstants.PROGRAM_TIMEOUT_MS);
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
                return XcpResponse.ok(data);
            }
            if (pid == XcpConstants.PID_ERR) {
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
