package com.rusefi.openblt;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class XcpClientTest {

    private FakeCanLink link;
    private XcpClient client;

    @BeforeEach
    void setUp() throws Exception {
        link = new FakeCanLink();
        client = new XcpClient(link, XcpConstants.TX_ID, XcpConstants.EXTENDED,
                XcpConstants.RX_ID, 200);
        // The bootloader ignores every command until CONNECT - do that once,
        // then clear the traffic log so each test sees only its own frames.
        assertTrue(client.connect(0, 500).isOk());
        link.clearSent();
    }

    @Test
    void connectSendsDlc2FrameAndParsesResponse() throws Exception {
        XcpResponse res = client.connect(0, 500);

        assertTrue(res.isOk());
        assertEquals(8, res.data().length);

        List<CanFrame> sent = link.sent();
        assertEquals(1, sent.size());
        CanFrame frame = sent.get(0);
        assertEquals(XcpConstants.TX_ID, frame.id());
        assertTrue(frame.extended());
        // DLC=2 is load-bearing: the running app jumps to the bootloader on
        // exactly this frame when canOpenBLT=yes.
        assertArrayEquals(new byte[]{(byte) 0xFF, 0x00}, frame.data());
    }

    @Test
    void setMtaUsesLittleEndianAddress() throws Exception {
        XcpResponse res = client.setMta(0x08008000);

        assertTrue(res.isOk());
        CanFrame frame = link.sent().get(0);
        assertArrayEquals(new byte[]{(byte) 0xF6, 0, 0, 0, 0x00, (byte) 0x80, 0x00, 0x08}, frame.data());
    }

    @Test
    void programClearCarriesLittleEndianLength() throws Exception {
        client.setMta(0x08008000);
        XcpResponse res = client.programClear(0x123456, 500);

        assertTrue(res.isOk());
        CanFrame frame = link.sent().get(link.sent().size() - 1);
        assertArrayEquals(new byte[]{(byte) 0xD1, 0, 0, 0, 0x56, 0x34, 0x12, 0x00}, frame.data());
    }

    @Test
    void programMaxCarriesSevenPayloadBytes() throws Exception {
        byte[] payload = {1, 2, 3, 4, 5, 6, 7};
        client.setMta(0x08008000);
        XcpResponse res = client.programMax(payload);

        assertTrue(res.isOk());
        CanFrame frame = link.sent().get(link.sent().size() - 1);
        assertArrayEquals(new byte[]{(byte) 0xC9, 1, 2, 3, 4, 5, 6, 7}, frame.data());
    }

    @Test
    void programCarriesSizeAndData() throws Exception {
        byte[] payload = {(byte) 0xAA, (byte) 0xBB, (byte) 0xCC};
        client.setMta(0x08008000);
        XcpResponse res = client.program(payload);

        assertTrue(res.isOk());
        CanFrame frame = link.sent().get(link.sent().size() - 1);
        assertArrayEquals(new byte[]{(byte) 0xD0, 0x03, (byte) 0xAA, (byte) 0xBB, (byte) 0xCC}, frame.data());
    }

    @Test
    void reportsErrorResponses() throws Exception {
        // The fake bootloader answers unknown commands with ERR/CMD_UNKNOWN.
        XcpResponse res = client.request(new byte[]{(byte) 0x01}, 500);

        assertFalse(res.isOk());
        assertEquals(XcpConstants.ERR_CMD_UNKNOWN, res.errorCode());
    }

    @Test
    void ignoresForeignFramesWhileWaiting() throws Exception {
        // App chatter on unrelated ids must not be mistaken for the response.
        link.injectFrame(new CanFrame(0x720, false, new byte[]{0x01, 0x02, 0x03}));
        link.injectFrame(new CanFrame(XcpConstants.RX_ID, false, new byte[]{0x01, 0x02})); // right id, wrong format

        XcpResponse res = client.getStatus();

        assertTrue(res.isOk());
        // Both injected frames were skipped; only the real response was used.
        assertEquals(1, link.sent().size());
    }

    @Test
    void timesOutWhenBootloaderIsSilent() throws Exception {
        link.setMute(true);

        XcpResponse res = client.connect(0, 150);

        assertNull(res);
    }

    @Test
    void programResetSendsFireAndForget() throws Exception {
        client.programReset();

        CanFrame frame = link.sent().get(0);
        assertArrayEquals(new byte[]{(byte) 0xCF}, frame.data());
        assertEquals(1, link.programResetCount());
    }

    @Test
    void buildChecksumParsesAdditiveSum() throws Exception {
        // Write a known pattern then ask the bootloader for its ADD11 sum.
        byte[] data = {10, 20, 30};
        client.setMta(0x08008000);
        client.program(data);
        client.setMta(0x08008000);

        XcpResponse res = client.buildChecksum(3);

        assertTrue(res.isOk());
        assertEquals(XcpConstants.CS_ADD11, res.data()[1] & 0xFF);
        int sum = (res.data()[4] & 0xFF)
                | ((res.data()[5] & 0xFF) << 8)
                | ((res.data()[6] & 0xFF) << 16)
                | ((res.data()[7] & 0xFF) << 24);
        assertEquals(60, sum);
    }
}
