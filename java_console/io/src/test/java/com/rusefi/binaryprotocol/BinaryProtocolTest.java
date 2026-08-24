package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileModel;
import com.opensr5.io.DataListener;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.tcp.TcpIoStream;
import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.spy;

public class BinaryProtocolTest {
    private static final int LUA_PAGE = 0x0400;
    private static final int LUA_PAGE_SIZE = 48000;
    private static final int TARGET_BLOCKING_FACTOR = 1024;
    private static final int OVERRIDDEN_BLOCKING_FACTOR = 4080;

    private static class TestStream extends AbstractIoStream {
        private final IncomingDataBuffer dataBuffer = new IncomingDataBuffer("test", getStreamStats());

        @Override
        public void setInputListener(DataListener listener) {
        }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return dataBuffer;
        }
    }

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

    @Test
    public void issue10134PageReadHonorsTargetBlockingFactor() {
        TestStream stream = new TestStream();
        BinaryProtocol protocol = createProtocol(stream);
        List<byte[]> requests = new ArrayList<>();
        doAnswer(invocation -> {
            byte[] request = invocation.getArgument(1);
            requests.add(request.clone());
            return new byte[unsignedShort(request, 4) + 1];
        }).when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());

        try {
            assertNotNull(protocol.readFromPage(LUA_PAGE, 0, LUA_PAGE_SIZE));
            assertIssue10134TargetSafeChunks(requests);
        } finally {
            stream.close();
        }
    }

    @Test
    public void issue10134PageWriteHonorsTargetBlockingFactor() {
        TestStream stream = new TestStream();
        BinaryProtocol protocol = createProtocol(stream);
        List<byte[]> requests = new ArrayList<>();
        doAnswer(invocation -> {
            requests.add(((byte[]) invocation.getArgument(1)).clone());
            return new byte[]{Integration.TS_RESPONSE_OK};
        }).when(protocol).executeCommand(eq(Integration.TS_CHUNK_WRITE_COMMAND), any(byte[].class), anyString());

        try {
            protocol.writeInBlocks(new byte[LUA_PAGE_SIZE], 0, 0, LUA_PAGE_SIZE, LUA_PAGE);
            assertIssue10134TargetSafeChunks(requests);
        } finally {
            stream.close();
        }
    }

    private static BinaryProtocol createProtocol(TestStream stream) {
        return createProtocol(stream, OVERRIDDEN_BLOCKING_FACTOR);
    }

    private static BinaryProtocol createProtocol(IoStream stream, int blockingFactorOverride) {
        IniFileModel iniFile = mock(IniFileModel.class);
        doReturn(TARGET_BLOCKING_FACTOR).when(iniFile).getBlockingFactor();

        BinaryProtocol protocol = spy(new BinaryProtocol(new LinkManager(), stream, blockingFactorOverride));
        doReturn(iniFile).when(protocol).getIniFile();
        doReturn(false).when(protocol).isSinglePageController();
        return protocol;
    }

    private static void assertIssue10134TargetSafeChunks(List<byte[]> requests) {
        assertEquals(47, requests.size());
        for (int i = 0; i < requests.size(); i++) {
            byte[] request = requests.get(i);
            int expectedSize = i < 46 ? TARGET_BLOCKING_FACTOR : 896;
            assertEquals(LUA_PAGE, unsignedShort(request, 0));
            assertEquals(i * TARGET_BLOCKING_FACTOR, unsignedShort(request, 2));
            assertEquals(expectedSize, unsignedShort(request, 4));
        }
    }

    @Test
    public void blockingFactorOverrideRemainsAvailableForTcp() {
        List<Runnable> closeListeners = new ArrayList<>();
        TcpIoStream stream = mock(TcpIoStream.class);
        doAnswer(invocation -> {
            closeListeners.add(invocation.getArgument(0));
            return null;
        }).when(stream).addCloseListener(any(Runnable.class));

        BinaryProtocol protocol = createProtocol(stream, 32000);
        List<byte[]> requests = new ArrayList<>();
        doAnswer(invocation -> {
            byte[] request = invocation.getArgument(1);
            requests.add(request.clone());
            return new byte[unsignedShort(request, 4) + 1];
        }).when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());

        try {
            assertNotNull(protocol.readFromPage(LUA_PAGE, 0, LUA_PAGE_SIZE));
            assertEquals(2, requests.size());
            assertEquals(32000, unsignedShort(requests.get(0), 4));
            assertEquals(32000, unsignedShort(requests.get(1), 2));
            assertEquals(16000, unsignedShort(requests.get(1), 4));
        } finally {
            closeListeners.forEach(Runnable::run);
        }
    }

    private static int unsignedShort(byte[] bytes, int offset) {
        return ByteBuffer.wrap(bytes, offset, Short.BYTES)
            .order(ByteOrder.LITTLE_ENDIAN)
            .getShort() & 0xffff;
    }
}
