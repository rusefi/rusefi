package com.rusefi.io;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.UnsupportedEcuInfo;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class LinkManagerCompatibilityListenerTest {
    @Test
    public void canTransportsCanReconnectWithoutAppearingAsSerialPorts() {
        assertTrue(LinkManager.isCanPort("SLCAN:COM7"));
        assertTrue(LinkManager.isSpecialNotSerial("SLCAN:COM7"));
        assertTrue(LinkManager.isPortAvailableForReconnect("SLCAN:COM7", Collections.emptySet()));
        assertTrue(LinkManager.isPortAvailableForReconnect(
            LinkManager.SOCKET_CAN, Collections.emptySet()));
        assertTrue(LinkManager.isPortAvailableForReconnect(
            LinkManager.PCAN, Collections.emptySet()));
        assertFalse(LinkManager.isPortAvailableForReconnect(
            "COM7", Collections.emptySet()));
        assertTrue(LinkManager.isPortAvailableForReconnect(
            "COM7", Collections.singleton("COM7")));
    }

    @Test
    public void unsupportedEventCarriesPortAndIdentity() {
        AtomicReference<String> reportedPort = new AtomicReference<>();
        AtomicReference<UnsupportedEcuInfo> reportedInfo = new AtomicReference<>();
        UnsupportedEcuInfo info = new UnsupportedEcuInfo("board-a", "universal");

        try (LinkManager linkManager = new LinkManager().setEcuCompatibilityListener((port, value) -> {
            reportedPort.set(port);
            reportedInfo.set(value);
        })) {
            linkManager.start(LinkManager.LOG_VIEWER, ConnectionStatusLogic.Listener.VOID);
            linkManager.reportUnsupportedEcu(info);
        }

        assertEquals(LinkManager.LOG_VIEWER, reportedPort.get());
        assertSame(info, reportedInfo.get());
    }

    @Test
    public void compatibleEventRetainsPortAcrossClose() {
        List<String> events = new ArrayList<>();
        LinkManager linkManager = new LinkManager().setEcuCompatibilityListener(
            new LinkManager.EcuCompatibilityListener() {
                @Override
                public void onUnsupportedEcu(String port, UnsupportedEcuInfo info) {
                }

                @Override
                public void onCompatibleEcu(String port, RusEfiSignature signature) {
                    events.add("compatible:" + port + ":" + signature.getBundleTarget());
                }
            });
        linkManager.start(LinkManager.LOG_VIEWER, ConnectionStatusLogic.Listener.VOID);

        linkManager.close();
        linkManager.reportCompatibleEcu(
            SignatureHelper.parse("rusEFI master.2026.07.30.board-b.123456"));

        assertEquals(java.util.Collections.singletonList(
            "compatible:log viewer:board-b"), events);
    }

    @Test
    public void newConnectionAttemptClearsMissingIniPairing() {
        LinkManager linkManager = new LinkManager().setNotifyGlobalStatusOnClose(false);
        AtomicReference<String> failure = new AtomicReference<>();
        linkManager.setMissingIniListener((signature, port) -> failure.set(port + ":" + signature));
        linkManager.start("COM_TEST", ConnectionStatusLogic.Listener.VOID);
        linkManager.recordMissingIniFailure("signature-a");

        assertEquals("COM_TEST", linkManager.getMissingIniFailure().port);
        assertEquals("signature-a", linkManager.getMissingIniFailure().signature);
        assertEquals("COM_TEST:signature-a", failure.get(), "the exact probe target must be propagated");

        linkManager.close();
        linkManager.start("COM_OTHER", ConnectionStatusLogic.Listener.VOID);

        assertEquals(null, linkManager.getMissingIniFailure());
        linkManager.close();
    }

    @Test
    public void iniResolutionStartUsesTheExistingProbeListener() {
        List<String> events = new ArrayList<>();
        LinkManager linkManager = new LinkManager()
            .setNotifyGlobalStatusOnClose(false)
            .setMissingIniListener(new LinkManager.MissingIniListener() {
                @Override
                public void onMissingIni(String signature, String port) {
                }

                @Override
                public void onIniResolutionStarted(String port) {
                    events.add("started:" + port);
                }

            });
        linkManager.onIniResolutionStarted("COM_TEST");

        assertEquals(java.util.Collections.singletonList("started:COM_TEST"), events);
        linkManager.close();
    }
}
