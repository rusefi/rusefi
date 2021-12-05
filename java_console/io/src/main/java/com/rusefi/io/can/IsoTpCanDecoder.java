package com.rusefi.io.can;

import com.devexperts.logging.Logging;
import com.rusefi.io.IoStream;

import java.util.Arrays;

// CAN multiframe decoder state
class IsoTpCanDecoder {
    private static Logging log = Logging.getLogging(IsoTpCanDecoder.class);

    static {
        log.configureDebugEnabled(false);
    }

    private final static int ISO_TP_FRAME_FLOW_CONTROL = 3;
    final static int ISO_TP_FRAME_SINGLE = 0;
    final static int ISO_TP_FRAME_FIRST = 1;
    final static int ISO_TP_FRAME_CONSECUTIVE = 2;

    public int waitingForNumBytes = 0;
    public int waitingForFrameIndex = 0;

    public byte[] decodePacket(byte[] data) throws Exception {
        int frameType = (data[0] >> 4) & 0xf;
        int numBytesAvailable;
        int frameIdx;
        int dataOffset;
        switch (frameType) {
            case ISO_TP_FRAME_SINGLE:
                numBytesAvailable = data[0] & 0xf;
                dataOffset = 1;
                this.waitingForNumBytes = 0;
                break;
            case ISO_TP_FRAME_FIRST:
                this.waitingForNumBytes = ((data[0] & 0xf) << 8) | data[1];
                if (log.debugEnabled())
                    log.debug("Total expected: " + waitingForNumBytes);
                this.waitingForFrameIndex = 1;
                numBytesAvailable = Math.min(this.waitingForNumBytes, 6);
                waitingForNumBytes -= numBytesAvailable;
                dataOffset = 2;
                break;
            case ISO_TP_FRAME_CONSECUTIVE:
                frameIdx = data[0] & 0xf;
                if (this.waitingForNumBytes < 0 || this.waitingForFrameIndex != frameIdx) {
                    throw new Exception("ISO_TP_FRAME_CONSECUTIVE: That's an abnormal situation, and we probably should react?");
                }
                this.waitingForFrameIndex = (this.waitingForFrameIndex + 1) & 0xf;
                numBytesAvailable = Math.min(this.waitingForNumBytes, 7);
                dataOffset = 1;
                waitingForNumBytes -= numBytesAvailable;
                if (log.debugEnabled())
                    log.debug("ISO_TP_FRAME_CONSECUTIVE Got " + numBytesAvailable + ", still expecting: " + waitingForNumBytes);
                break;
            case ISO_TP_FRAME_FLOW_CONTROL:
                throw new Exception("ISO_TP_FRAME_FLOW_CONTROL: should we just ignore the FC frame?");
            default:
                throw new Exception("Unknown frame type");
        }
        byte[] bytes = Arrays.copyOfRange(data, dataOffset, dataOffset + numBytesAvailable);
        if (log.debugEnabled())
            log.debug(numBytesAvailable + " bytes(s) arrived in this packet:" + IoStream.printHexBinary(bytes));
        return bytes;
    }
}
