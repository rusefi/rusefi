package com.rusefi.openblt;

import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANBaudrate;
import peak.can.basic.TPCANHandle;
import peak.can.basic.TPCANMessageType;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANStatus;
import peak.can.basic.TPCANTimestamp;
import peak.can.basic.TPCANType;

import java.io.IOException;
import java.util.Arrays;
import java.util.concurrent.locks.LockSupport;

/**
 * CanLink over PCANBasic. On macOS this loads libpcanbasic_jni.dylib, which
 * bridges to MacCAN-Core (libPCBUSB); on Windows it is the official PEAK
 * driver. The PCANBasic convention marks extended frames by setting the MSB
 * of the id in addition to the MSGTYPE flag; both are handled here.
 */
public class PcanLink implements CanLink {
    private final TPCANHandle channel;
    private final PCANBasic can = new PCANBasic();

    public PcanLink(TPCANHandle channel) {
        this.channel = channel;
    }

    @Override
    public void open() throws IOException {
        if (!can.initializeAPI()) {
            throw new IOException("PCANBasic initializeAPI failed - is libpcanbasic_jni.dylib on java.library.path?");
        }
        TPCANStatus status = can.Initialize(channel, TPCANBaudrate.PCAN_BAUD_500K,
                TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            throw new IOException("PCAN Initialize(" + channel + ") failed: " + status);
        }
    }

    @Override
    public void write(CanFrame frame) throws IOException {
        // PCANBasic: the MSB of the id marks an extended frame.
        int wireId = frame.extended() ? (frame.id() | 0x80000000) : frame.id();
        byte type = frame.extended()
                ? TPCANMessageType.PCAN_MESSAGE_EXTENDED.getValue()
                : TPCANMessageType.PCAN_MESSAGE_STANDARD.getValue();
        TPCANMsg msg = new TPCANMsg(wireId, type, (byte) frame.length(), frame.data());
        TPCANStatus status = can.Write(channel, msg);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            throw new IOException("PCAN Write failed: " + status);
        }
    }

    @Override
    public CanFrame readFrame(int timeoutMs) throws IOException {
        long deadline = System.currentTimeMillis() + timeoutMs;
        while (System.currentTimeMillis() < deadline) {
            TPCANMsg msg = new TPCANMsg();
            TPCANTimestamp ts = new TPCANTimestamp();
            TPCANStatus status = can.Read(channel, msg, ts);
            if (status == TPCANStatus.PCAN_ERROR_OK) {
                int rawId = msg.getID();
                boolean extended = (rawId & 0x80000000) != 0
                        || (msg.getType() & 0xFF) == (TPCANMessageType.PCAN_MESSAGE_EXTENDED.getValue() & 0xFF);
                int id = rawId & 0x1FFFFFFF;
                int len = msg.getLength() & 0xFF;
                byte[] data = Arrays.copyOf(msg.getData(), Math.min(len, msg.getData().length));
                return new CanFrame(id, extended, data);
            }
            if (status != TPCANStatus.PCAN_ERROR_QRCVEMPTY) {
                throw new IOException("PCAN Read failed: " + status);
            }
            // MacCAN's Read does not block: poll at fine granularity instead
            // of a coarse Thread.sleep(1). The 1 ms granularity costs ~2 ms
            // per XCP frame (~100k frames per 670 KB image = 4+ min); the ECU
            // reply normally lands ~0.5-1 ms after the request, so busy-spin
            // that window and fall back to short parks only for slower replies
            // (erase, connect).
            long spinUntil = System.nanoTime() + 1_000_000L; // 1 ms busy spin
            while (System.nanoTime() < spinUntil) {
                Thread.onSpinWait();
            }
            LockSupport.parkNanos(100_000L); // 0.1 ms slices for slow replies
        }
        return null;
    }

    @Override
    public void close() {
        can.Uninitialize(channel);
    }
}
