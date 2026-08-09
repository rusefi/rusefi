package com.rusefi.io.can.isotp;

import com.devexperts.logging.Logging;
import com.rusefi.util.HexBinary;
import org.jetbrains.annotations.NotNull;

/**
 * @see IsoTpCanDecoder for RX operations
 */
public abstract class IsoTpConnector {
    private final static Logging log = Logging.getLogging(IsoTpConnector.class);

    private final int canId;

    protected IsoTpConnector(int canId) {
        this.canId = canId;
    }

    public int canId() {
        return canId;
    }

    public static void sendStrategy(byte[] bytes, IsoTpConnector connector) {
        log.info("-------sendBytesToCan " + bytes.length + " byte(s):");

        log.info(HexBinary.printHexBinary(bytes));


        // 1 frame
        if (bytes.length <= 7) {
            connector.sendCanFrame((IsoTpConstants.ISO_TP_FRAME_SINGLE << 4) | bytes.length, bytes, 0, bytes.length);
            return;
        }

        // multiple frames
        // send the first header frame
        connector.sendCanFrame((IsoTpConstants.ISO_TP_FRAME_FIRST << 4) | ((bytes.length >> 8) & 0x0f), bytes.length & 0xff, bytes, 0, 6);
        // wait for the receiver's flow control frame before bursting the consecutive frames.
        // default implementation is a no-op; hardware streams may override it to pace the burst
        // (see PCanIoStream) so the receiver's FIFO does not overflow.
        connector.receiveData();

        // send the rest of the data
        int idx = 1;
        int offset = 6;
        int remaining = bytes.length - 6;
        while (remaining > 0) {
            int len = Math.min(remaining, 7);
            // send the consecutive frames
            connector.sendCanFrame((IsoTpConstants.ISO_TP_FRAME_CONSECUTIVE << 4) | ((idx++) & 0x0f), bytes, offset, len);
            offset += len;
            remaining -= len;
        }
    }

    @NotNull
    public static byte[] combineArrays(byte[] hdr, byte[] data, int dataOffset, int dataLength) {
        byte[] total = new byte[hdr.length + dataLength];
        System.arraycopy(hdr, 0, total, 0, hdr.length);
        System.arraycopy(data, dataOffset, total, hdr.length, dataLength);
        return total;
    }

    public void sendCanFrame(int hdr0, byte[] data, int offset, int dataLength) {
        sendCanData(new byte[]{(byte) hdr0}, data, offset, dataLength);
    }

    public void sendCanFrame(int hdr0, int hdr1, byte[] data, int dataOffset, int dataLength) {
        sendCanData(new byte[]{(byte) hdr0, (byte) hdr1}, data, dataOffset, dataLength);
    }

    private void sendCanData(byte[] hdr, byte[] data, int dataOffset, int dataLength) {
        byte[] total = combineArrays(hdr, data, dataOffset, dataLength);
        sendCanData(total);
    }

    public abstract void sendCanData(byte[] total);

    /**
     * Called by {@link #sendStrategy} between the FIRST frame and the CONSECUTIVE burst of a
     * multi-frame ISO-TP packet. Override to wait for the receiver's flow control (FC) frame;
     * the default implementation does not wait (keeps the historical burst behavior).
     */
    public void receiveData() {
    }
}
