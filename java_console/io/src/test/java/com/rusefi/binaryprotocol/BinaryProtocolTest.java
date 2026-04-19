package com.rusefi.binaryprotocol;

import com.rusefi.config.generated.Integration;
import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import org.junit.jupiter.api.Test;

public class BinaryProtocolTest {
    @Test
    public void test() {
        byte[] packet = BinaryProtocol.smartPacketPrefix2(2000, 1000, false);
        byte[] fullRequest = BinaryProtocol.getFullRequest((byte) Integration.TS_READ_COMMAND, packet);
        assertArrayEquals(new byte[]{
                'R',
                0, 0, // page
                (byte) 0xD0, 0x07, // offset 2000
                (byte) 0xE8, 0x03 // size 1000
        }, fullRequest);
    }

    @Test
    public void testSinglePage() {
        byte[] packet = BinaryProtocol.smartPacketPrefix2(2000, 1000, true);
        byte[] fullRequest = BinaryProtocol.getFullRequest((byte) Integration.TS_READ_COMMAND, packet);
        assertArrayEquals(new byte[]{
                'R',
                (byte) 0xD0, 0x07, // offset 2000
                (byte) 0xE8, 0x03 // size 1000
        }, fullRequest);
    }

    @Test
    public void testCrcCheck() {
        byte[] packet = BinaryProtocol.smartPacketPrefix2(0, 21000, false);
        byte[] fullRequest = BinaryProtocol.getFullRequest((byte) Integration.TS_CRC_CHECK_COMMAND, packet);
        assertArrayEquals(new byte[]{
                'k',
                0, 0, // page
                0, 0, // offset
                (byte) 0x08, 0x52 // size 21000
        }, fullRequest);
    }

    @Test
    public void testBurnCommand() {
        byte[] fullRequest = BinaryProtocol.getFullRequest((byte) Integration.TS_BURN_COMMAND, new byte[]{0, 0});
        assertArrayEquals(new byte[]{
                'B',
                0, 0,
        }, fullRequest);
    }

    @Test
    public void testChunkWrite() {
        int offset = 123;
        byte[] content = new byte[]{0x11, 0x22};
        byte[] header = BinaryProtocol.smartPacketPrefix2(offset, content.length, false);
        byte[] packet = new byte[header.length + content.length];
        System.arraycopy(header, 0, packet, 0, header.length);
        System.arraycopy(content, 0, packet, header.length, content.length);

        byte[] fullRequest = BinaryProtocol.getFullRequest((byte) Integration.TS_CHUNK_WRITE_COMMAND, packet);
        assertArrayEquals(new byte[]{
                'C',
                0, 0, // page
                0x7B, 0, // offset 123
                0x02, 0, // size 2
                0x11, 0x22 // content
        }, fullRequest);
    }
}
