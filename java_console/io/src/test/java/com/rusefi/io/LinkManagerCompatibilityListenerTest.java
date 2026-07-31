package com.rusefi.io;

import com.rusefi.core.io.UnsupportedEcuInfo;
import org.junit.jupiter.api.Test;

import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;

public class LinkManagerCompatibilityListenerTest {
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
}
