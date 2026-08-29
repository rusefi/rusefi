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
    private TPCANBaudrate currentBaudrate;

    public PcanLink(TPCANHandle channel) {
        this.channel = channel;
    }

    @Override
    public void open() throws IOException {
        if (!can.initializeAPI()) {
            throw new IOException("PCANBasic initializeAPI failed - is libpcanbasic_jni.dylib on java.library.path?");
        }
        init(TPCANBaudrate.PCAN_BAUD_500K);
    }

    private void init(TPCANBaudrate rate) throws IOException {
        TPCANStatus status = can.Initialize(channel, rate,
                TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            throw new IOException("PCAN Initialize(" + channel + ") failed: " + status);
        }
        currentBaudrate = rate;
    }

    @Override
    public void setBaudrate(int rateCode) throws IOException {
        TPCANBaudrate rate = rateCode == XcpConstants.BAUD_1M
                ? TPCANBaudrate.PCAN_BAUD_1M
                : TPCANBaudrate.PCAN_BAUD_500K;
        if (rate == currentBaudrate) {
            return;
        }
        // Re-Initialize reconfigures the controller baudrate; the previous
        // connection state is dropped (the XCP session survives - the
        // bootloader keeps the session across its own switch).
        TPCANStatus status = can.Uninitialize(channel);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            throw new IOException("PCAN Uninitialize failed: " + status);
        }
        init(rate);
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
        // MacCAN's Read does not block. The reply to a freshly sent XCP frame
        // normally lands well under ~1 ms, so poll the queue tightly for the
        // first 1 ms of every wait - checking the queue every few tens of us
        // - and only fall back to 0.1 ms parks for slow replies (erase,
        // connect) to keep the CPU idle. The previous loop spun BLIND for a
        // fixed 1 ms between queue checks, which guaranteed ~1 ms dead time
        // per XCP frame (~95k frames per 670 KB image).
        long hotSpinUntil = System.nanoTime() + 1_000_000L; // 1 ms hot window
        TPCANMsg msg = new TPCANMsg();
        TPCANTimestamp ts = new TPCANTimestamp();
        while (System.currentTimeMillis() < deadline) {
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
            if (System.nanoTime() < hotSpinUntil) {
                // Hot window: tight poll, onSpinWait keeps the pause hint so
                // the MacCAN receive-pump thread keeps its CPU share.
                Thread.onSpinWait();
            } else {
                // Slow reply: poll in 0.1 ms slices instead of burning a core.
                LockSupport.parkNanos(100_000L);
            }
        }
        return null;
    }

    @Override
    public CanFrame pollFrame() throws IOException {
        TPCANMsg msg = new TPCANMsg();
        TPCANTimestamp ts = new TPCANTimestamp();
        TPCANStatus status = can.Read(channel, msg, ts);
        if (status == TPCANStatus.PCAN_ERROR_QRCVEMPTY) {
            return null;
        }
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            throw new IOException("PCAN Read failed: " + status);
        }
        int rawId = msg.getID();
        boolean extended = (rawId & 0x80000000) != 0
                || (msg.getType() & 0xFF) == (TPCANMessageType.PCAN_MESSAGE_EXTENDED.getValue() & 0xFF);
        int id = rawId & 0x1FFFFFFF;
        int len = msg.getLength() & 0xFF;
        byte[] data = Arrays.copyOf(msg.getData(), Math.min(len, msg.getData().length));
        return new CanFrame(id, extended, data);
    }

    @Override
    public void close() {
        can.Uninitialize(channel);
    }
}
