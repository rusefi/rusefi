package com.rusefi.io.can.isotp;

import com.devexperts.logging.Logging;
import com.rusefi.util.HexBinary;

import java.util.Arrays;

import static com.rusefi.util.HexBinary.printHexBinary;

/**
 * ISO 15765-2 or ISO-TP (Transport Layer) CAN multi-frame decoder state
 *
 * @see IsoTpConnector
 */
public abstract class IsoTpCanDecoder {
    public static final byte[] FLOW_CONTROL = {0x30, 0, 0, 0, 0, 0, 0, 0};
    private static final Logging log = Logging.getLogging(IsoTpCanDecoder.class);

    static {
        log.configureDebugEnabled(false);
    }

    private final static int ISO_TP_FRAME_FLOW_CONTROL = 3;

    private final static int FC_ContinueToSend = 0;
    private final int isoHeaderByteIndex;

    private int waitingForNumBytes = 0;
    private int waitingForFrameIndex = 0;
    private boolean isComplete;

    public IsoTpCanDecoder() {
        this(0);
    }

    public IsoTpCanDecoder(int isoHeaderByteIndex) {
        this.isoHeaderByteIndex = isoHeaderByteIndex;
    }

    @Deprecated // warning: PCAN driver is an example of larger data buffer
    public byte[] decodePacket(byte[] data) {
        return decodePacket(data, data.length);
    }

   public byte[] decodePacket(byte[] data, int dataSize) {
//        log.info("Decoding " + printHexBinary(data));
        int frameType = (data[isoHeaderByteIndex] >> 4) & 0xf;
        int numBytesAvailable;
        int frameIdx;
        int dataOffset;
        switch (frameType) {
            case IsoTpConstants.ISO_TP_FRAME_SINGLE:
                numBytesAvailable = data[isoHeaderByteIndex] & 0xf;
                dataOffset = isoHeaderByteIndex + 1;
                this.waitingForNumBytes = 0;
                if (log.debugEnabled())
                    log.debug("ISO_TP_FRAME_SINGLE " + numBytesAvailable);
                setComplete(true);
                break;
            case IsoTpConstants.ISO_TP_FRAME_FIRST:
                this.waitingForNumBytes = ((data[isoHeaderByteIndex] & 0xf) << 8) | data[isoHeaderByteIndex + 1];
                if (log.debugEnabled())
                    log.debug("Total expected: " + waitingForNumBytes);
                this.waitingForFrameIndex = 1;
                numBytesAvailable = Math.min(this.waitingForNumBytes, dataSize - 2 - isoHeaderByteIndex);
                waitingForNumBytes -= numBytesAvailable;
                dataOffset = isoHeaderByteIndex + 2;
                onTpFirstFrame();
                break;
            case IsoTpConstants.ISO_TP_FRAME_CONSECUTIVE:
                frameIdx = data[isoHeaderByteIndex] & 0xf;
                if (this.waitingForNumBytes < 0 || this.waitingForFrameIndex != frameIdx) {
                    throw new IllegalStateException("ISO_TP_FRAME_CONSECUTIVE: That's an abnormal situation, and we probably should react? waitingForNumBytes=" + waitingForNumBytes + " waitingForFrameIndex=" + waitingForFrameIndex + " frameIdx=" + frameIdx);
                }
                this.waitingForFrameIndex = (this.waitingForFrameIndex + 1) & 0xf;
                numBytesAvailable = Math.min(this.waitingForNumBytes, dataSize - 1 - isoHeaderByteIndex);
                dataOffset = isoHeaderByteIndex + 1;
                waitingForNumBytes -= numBytesAvailable;
                if (log.debugEnabled())
                    log.debug("ISO_TP_FRAME_CONSECUTIVE Got " + numBytesAvailable + " byte(s), still expecting: " + waitingForNumBytes + " byte(s)");
                setComplete(waitingForNumBytes == 0);
                break;
            case ISO_TP_FRAME_FLOW_CONTROL:
                int flowStatus = data[isoHeaderByteIndex] & 0xf;
                int blockSize = data[isoHeaderByteIndex + 1];
                int separationTime = data[isoHeaderByteIndex + 2];
                if (flowStatus == FC_ContinueToSend && blockSize == 0 && separationTime == 0)
                    return new byte[0];
                throw new IllegalStateException("ISO_TP_FRAME_FLOW_CONTROL: should we just ignore the FC frame? " + flowStatus + " " + blockSize + " " + separationTime);
            default:
                throw new IllegalStateException("Unknown frame type");
        }
        byte[] bytes = Arrays.copyOfRange(data, dataOffset, dataOffset + numBytesAvailable);
        if (log.debugEnabled())
            log.debug(numBytesAvailable + " bytes(s) arrived in this packet: " + HexBinary.printByteArray(bytes));
        return bytes;
    }

    protected abstract void onTpFirstFrame();

    public void setComplete(boolean isComplete) {
        this.isComplete = isComplete;
    }

    public boolean isComplete() {
        return isComplete;
    }
}
