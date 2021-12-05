package com.rusefi.io.can;

import java.util.Arrays;

// CAN multiframe decoder state
class IsoTpCanDecoder {
    private final static int ISO_TP_FRAME_FLOW_CONTROL = 3;
    final static int ISO_TP_FRAME_SINGLE = 0;
    final static int ISO_TP_FRAME_FIRST = 1;
    final static int ISO_TP_FRAME_CONSECUTIVE = 2;

    public int waitingForNumBytes = 0;
    public int waitingForFrameIndex = 0;

    public byte[] decodePacket(byte[] data) throws Exception {
        int frameType = (data[0] >> 4) & 0xf;
        int numBytesAvailable, frameIdx;
        int dataOffset;
        switch (frameType) {
            case ISO_TP_FRAME_SINGLE:
                numBytesAvailable = data[0] & 0xf;
                dataOffset = 1;
                this.waitingForNumBytes = 0;
                break;
            case ISO_TP_FRAME_FIRST:
                this.waitingForNumBytes = ((data[0] & 0xf) << 8) | data[1];
                this.waitingForFrameIndex = 1;
                numBytesAvailable = Math.min(this.waitingForNumBytes, 6);
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
                break;
            case ISO_TP_FRAME_FLOW_CONTROL:
                throw new Exception("ISO_TP_FRAME_FLOW_CONTROL: should we just ignore the FC frame?");
            default:
                throw new Exception("Unknown frame type");
        }

        return Arrays.copyOfRange(data, dataOffset, dataOffset + numBytesAvailable);
    }
}
