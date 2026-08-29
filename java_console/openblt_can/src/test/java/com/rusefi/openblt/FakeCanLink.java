package com.rusefi.openblt;

import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Queue;

/**
 * In-memory simulation of the rusEFI OpenBLT bootloader's XCP-over-CAN
 * behavior, mirroring ext/openblt/Target/Source/xcp.c for the commands the
 * flasher uses: CONNECT, GET_STATUS, GET_ID, SET_MTA, PROGRAM_CLEAR,
 * PROGRAM, PROGRAM_MAX, BUILD_CHECKSUM, PROGRAM_RESET.
 *
 * Also records every frame the host sent, so tests can assert the exact wire
 * traffic. Extras the real ECU does not have:
 *  - {@link #setMute(boolean)} - stop answering (connect-timeout tests)
 *  - {@link #setIgnoredConnects(int)} - swallow the first N CONNECTs
 *  - {@link #corrupt(int, int)} - flip a byte in flash (verify-failure tests)
 */
public class FakeCanLink implements CanLink {
    public static final int FLASH_SIZE = 2 * 1024 * 1024;

    private final byte[] flash = new byte[FLASH_SIZE];
    private final Queue<CanFrame> outgoing = new ArrayDeque<>();
    private final List<CanFrame> sent = new ArrayList<>();

    private int mta;
    private boolean connected;
    private boolean mute;
    private int ignoredConnects;
    private int programResetCount;
    private int programClearCount;
    private boolean sawProgramSizeZero;
    private int dropProgramMaxResponses;
    private int programMaxCount;
    private int batchCount;
    private int setBaudrateCount;
    private int lastBaudrate = XcpConstants.BAUD_500K;
    // ECU-side link speed: the bootloader reboots into the requested rate
    // after answering SET_CAN_BAUDRATE (the switch is a reboot on real
    // hardware), and falls back to 500k on its own when no valid traffic
    // arrives - modeled by setBaudrate() below.
    private int ecuBaudrate = XcpConstants.BAUD_500K;
    /** Rate for which the adapter's setBaudrate throws (simulates MacCAN's
     *  lack of 1 Mbit support). */
    private int failSetBaudrate = -1;
    /** Rate at which the ECU swallows CONNECTs (simulates an ECU that never
     *  hears the host at that speed). */
    private int dropConnectsAt = -1;

    // rusEFI batch-programming extension state (mirrors xcp.c)
    private final byte[] batchBuffer = new byte[XcpConstants.BATCH_MAX];
    private int batchLen;
    private int batchOffset;
    private int batchAddr;
    private boolean batchActive;

    public FakeCanLink() {
        Arrays.fill(flash, (byte) 0xFF);
    }

    // ------------------------------------------------------------------ hooks

    public void setMute(boolean mute) {
        this.mute = mute;
    }

    public void setIgnoredConnects(int n) {
        this.ignoredConnects = n;
    }

    /**
     * Simulates lost PROGRAM_MAX frames: the next N PROGRAM_MAX requests are
     * processed (flash written, MTA advanced) but NOT acknowledged - exactly
     * what a dropped CAN frame looks like from the host's point of view.
     */
    public void dropProgramMaxResponses(int n) {
        this.dropProgramMaxResponses = n;
    }

    /** The adapter refuses the given rate (MacCAN cannot do 1 Mbit). */
    public void failSetBaudrate(int rateCode) {
        this.failSetBaudrate = rateCode;
    }

    /** The ECU never answers CONNECT while its link is at the given rate. */
    public void dropConnectsAt(int rateCode) {
        this.dropConnectsAt = rateCode;
    }

    public int ecuBaudrate() {
        return ecuBaudrate;
    }

    public int programMaxCount() {
        return programMaxCount;
    }

    public int batchCount() {
        return batchCount;
    }

    public int setBaudrateCount() {
        return setBaudrateCount;
    }

    public int lastBaudrate() {
        return lastBaudrate;
    }

    public void corrupt(int address, int value) {
        flash[address - XcpConstants.APP_BASE] = (byte) value;
    }

    public byte[] flashAt(int address, int len) {
        return Arrays.copyOfRange(flash, address - XcpConstants.APP_BASE, address - XcpConstants.APP_BASE + len);
    }

    public List<CanFrame> sent() {
        return new ArrayList<>(sent);
    }

    public void clearSent() {
        sent.clear();
    }

    public int programResetCount() {
        return programResetCount;
    }

    public int programClearCount() {
        return programClearCount;
    }

    public boolean sawProgramSizeZero() {
        return sawProgramSizeZero;
    }

    // ------------------------------------------------------------- CanLink

    @Override
    public void open() {
    }

    @Override
    public void write(CanFrame frame) {
        sent.add(frame);
        if (mute) {
            return;
        }
        // A frame sent at a speed the ECU is not listening on is bus garbage:
        // record it (tests assert the traffic) but do not process it.
        if (lastBaudrate != ecuBaudrate) {
            return;
        }
        process(frame);
    }

    @Override
    public CanFrame readFrame(int timeoutMs) {
        return outgoing.poll();
    }

    @Override
    public CanFrame pollFrame() {
        return outgoing.poll();
    }

    @Override
    public void setBaudrate(int rateCode) throws IOException {
        if (rateCode == failSetBaudrate) {
            throw new IOException("simulated adapter failure: no support for rate code " + rateCode);
        }
        lastBaudrate = rateCode;
        // Model the ECU-side self-recovery: a bootloader left at 1 Mbit with
        // no valid traffic reboots back to 500k by itself after 5 s (or exits
        // into the app at 500k after its backdoor window). The host always
        // re-points the adapter at 500k before reconnecting, and by then the
        // ECU is guaranteed to be back at 500k.
        if (ecuBaudrate == XcpConstants.BAUD_1M && rateCode == XcpConstants.BAUD_500K) {
            ecuBaudrate = XcpConstants.BAUD_500K;
            connected = false;
        }
    }

    @Override
    public void close() {
    }

    // -------------------------------------------------------- bootloader core

    private void process(CanFrame frame) {
        byte[] data = frame.data();
        if (data.length == 0) {
            return;
        }
        int cmd = data[0] & 0xFF;

        if (cmd == XcpConstants.CMD_CONNECT) {
            if (ecuBaudrate == dropConnectsAt) {
                return; // the ECU never hears frames at this speed
            }
            if (ignoredConnects > 0) {
                ignoredConnects--;
                return; // simulate the app still running / not yet in bootloader
            }
            connected = true;
            respond(new byte[]{(byte) XcpConstants.PID_RES, 0x08, 0x00, 0x08, 0x00, 0x08, 0x01, 0x01});
            return;
        }
        if (!connected) {
            return; // xcp.c ignores everything until connected
        }

        switch (cmd) {
            case XcpConstants.CMD_GET_STATUS -> respond(new byte[]{(byte) XcpConstants.PID_RES, 0, 0, 0, 0, 0});
            case XcpConstants.CMD_GET_ID -> respond(new byte[]{
                    (byte) XcpConstants.PID_RES, 0, 0, 0,
                    (byte) "rusefi.m74_9".length(), 0, 0, 0});
            case XcpConstants.CMD_SET_MTA -> {
                if (data.length >= 8) {
                    mta = le32(data, 4);
                }
                respond(new byte[]{(byte) XcpConstants.PID_RES});
            }
            case XcpConstants.CMD_PROGRAM_CLEAR -> {
                if (data.length < 8) {
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_OUT_OF_RANGE});
                    return;
                }
                int len = le32(data, 4);
                if (mta < XcpConstants.APP_BASE || mta + len > XcpConstants.APP_BASE + FLASH_SIZE) {
                    // mirrors FlashErase: no permission below the app base
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_GENERIC});
                    return;
                }
                Arrays.fill(flash, mta - XcpConstants.APP_BASE, mta - XcpConstants.APP_BASE + len, (byte) 0xFF);
                programClearCount++;
                respond(new byte[]{(byte) XcpConstants.PID_RES});
            }
            case XcpConstants.CMD_PROGRAM_BATCH -> {
                if (data.length < 8) {
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_OUT_OF_RANGE});
                    return;
                }
                int len = le32(data, 4);
                if (len <= 0 || len > XcpConstants.BATCH_MAX) {
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_OUT_OF_RANGE});
                    return;
                }
                batchLen = len;
                batchOffset = 0;
                batchAddr = mta;
                batchActive = true;
                batchCount++;
                respond(new byte[]{(byte) XcpConstants.PID_RES});
            }
            case XcpConstants.CMD_SET_CAN_BAUDRATE -> {
                setBaudrateCount++;
                respond(new byte[]{(byte) XcpConstants.PID_RES});
                // The real bootloader reboots into the requested rate right
                // after answering: the XCP session dies and the ECU comes up
                // at the new speed (fresh MTA, no batch state).
                ecuBaudrate = (data.length > 1) ? (data[1] & 0xFF) : XcpConstants.BAUD_500K;
                connected = false;
                mta = 0;
                batchActive = false;
            }
            case XcpConstants.CMD_PROGRAM_MAX -> {
                if (data.length < 8) {
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_OUT_OF_RANGE});
                    return;
                }
                int len = XcpConstants.PROGRAM_MAX_PAYLOAD;
                if (mta < XcpConstants.APP_BASE || mta + len > XcpConstants.APP_BASE + FLASH_SIZE) {
                    // mirrors FlashWrite: no permission below the app base
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_GENERIC});
                    return;
                }
                programMaxCount++;
                if (batchActive) {
                    // batch mode: buffer, program the whole batch at the end,
                    // respond only on the last frame (mirrors xcp.c)
                    int chunk = Math.min(len, batchLen - batchOffset);
                    System.arraycopy(data, 1, batchBuffer, batchOffset, chunk);
                    batchOffset += chunk;
                    mta += len;
                    if (batchOffset < batchLen) {
                        return; // intermediate frame: no response
                    }
                    System.arraycopy(batchBuffer, 0, flash, batchAddr - XcpConstants.APP_BASE, batchLen);
                    batchActive = false;
                } else {
                    System.arraycopy(data, 1, flash, mta - XcpConstants.APP_BASE, len);
                    mta += len;
                }
                if (dropProgramMaxResponses > 0) {
                    dropProgramMaxResponses--;
                    return; // frame lost on the wire: no ACK
                }
                respond(new byte[]{(byte) XcpConstants.PID_RES});
            }
            case XcpConstants.CMD_PROGRAM -> {
                if (data.length < 2) {
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_OUT_OF_RANGE});
                    return;
                }
                int len = data[1] & 0xFF;
                if (len > XcpConstants.PROGRAM_PAYLOAD) {
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_OUT_OF_RANGE});
                    return;
                }
                if (len == 0) {
                    sawProgramSizeZero = true; // mirrors NvmDone()
                } else {
                    if (mta < XcpConstants.APP_BASE || mta + len > XcpConstants.APP_BASE + FLASH_SIZE) {
                        // mirrors FlashWrite: no permission below the app base
                        respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_GENERIC});
                        return;
                    }
                    System.arraycopy(data, 2, flash, mta - XcpConstants.APP_BASE, len);
                    mta += len;
                }
                respond(new byte[]{(byte) XcpConstants.PID_RES});
            }
            case XcpConstants.CMD_BUILD_CHECKSUM -> {
                if (data.length < 8) {
                    respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_OUT_OF_RANGE});
                    return;
                }
                int len = le32(data, 4);
                int sum = 0;
                for (int i = 0; i < len; i++) {
                    sum = (sum + (flash[mta - XcpConstants.APP_BASE + i] & 0xFF)) & 0xFF;
                }
                respond(new byte[]{
                        (byte) XcpConstants.PID_RES, (byte) XcpConstants.CS_ADD11, 0, 0,
                        (byte) sum, 0, 0, 0});
            }
            case XcpConstants.CMD_PROGRAM_RESET -> {
                programResetCount++;
                // The real bootloader jumps to the app before answering: no response.
            }
            default -> respond(new byte[]{(byte) XcpConstants.PID_ERR, (byte) XcpConstants.ERR_CMD_UNKNOWN});
        }
    }

    private void respond(byte[] data) {
        outgoing.add(new CanFrame(XcpConstants.RX_ID, XcpConstants.EXTENDED, data));
    }

    private static int le32(byte[] buf, int off) {
        return (buf[off] & 0xFF)
                | ((buf[off + 1] & 0xFF) << 8)
                | ((buf[off + 2] & 0xFF) << 16)
                | ((buf[off + 3] & 0xFF) << 24);
    }

    // test convenience
    public void injectFrame(CanFrame frame) {
        outgoing.add(frame);
    }

    public void drainOutgoing() {
        outgoing.clear();
    }
}
