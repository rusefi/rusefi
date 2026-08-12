package com.rusefi.io.commands;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;

import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * Wire-format coverage for the page-aware burn command: firmware routes e.g. TS_PAGE_LUA
 * to burnExtraFlashPage, so the page payload byte order must match what the read/write
 * commands send ({@link ByteRange#packPageOffsetAndSize}).
 */
public class BurnCommandTest {
    private static final byte BURN_OK = (byte) Integration.TS_RESPONSE_BURN_OK;
    private static final int TS_PAGE_LUA = 0x0400;

    private static BinaryProtocol mockBp(boolean singlePage, byte[] response) {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        when(bp.isSinglePageController()).thenReturn(singlePage);
        when(bp.executeCommand(eq(Integration.TS_BURN_COMMAND), any(), eq("burn"))).thenReturn(response);
        return bp;
    }

    private static byte[] capturePayload(BinaryProtocol bp) {
        ArgumentCaptor<byte[]> payload = ArgumentCaptor.forClass(byte[].class);
        verify(bp).executeCommand(eq(Integration.TS_BURN_COMMAND), payload.capture(), eq("burn"));
        return payload.getValue();
    }

    @Test
    public void multiPageControllerBurnsPageZero() {
        BinaryProtocol bp = mockBp(false, new byte[]{BURN_OK});
        assertTrue(BurnCommand.execute(bp));
        assertArrayEquals(new byte[]{0, 0}, capturePayload(bp));
    }

    @Test
    public void pageEncodingMatchesReadWriteCommands() {
        BinaryProtocol bp = mockBp(false, new byte[]{BURN_OK});
        assertTrue(BurnCommand.execute(bp, TS_PAGE_LUA));

        byte[] payload = capturePayload(bp);
        // TS protocol is little-endian on the wire: page identifier 0x0400 -> 00 04
        assertArrayEquals(new byte[]{0x00, 0x04}, payload);

        // the invariant: burn must address pages exactly like read/write chunk commands do
        byte[] readWritePacket = new byte[6];
        ByteRange.packPageOffsetAndSize(TS_PAGE_LUA, 0, 0, readWritePacket);
        assertArrayEquals(Arrays.copyOf(readWritePacket, 2), payload);
    }

    @Test
    public void singlePageControllerSendsNoPagePayload() {
        BinaryProtocol bp = mockBp(true, new byte[]{BURN_OK});
        assertTrue(BurnCommand.execute(bp, TS_PAGE_LUA));
        assertNull(capturePayload(bp));
    }

    @Test
    public void nullResponseFails() {
        assertFalse(BurnCommand.execute(mockBp(false, null)));
    }

    @Test
    public void wrongResponseCodeFails() {
        assertFalse(BurnCommand.execute(mockBp(false, new byte[]{(byte) Integration.TS_RESPONSE_CRC_FAILURE})));
    }

    @Test
    public void wrongResponseLengthFails() {
        assertFalse(BurnCommand.execute(mockBp(false, new byte[]{BURN_OK, 0})));
    }
}
