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
}
