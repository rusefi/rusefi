package com.rusefi.ui.basic;

import com.rusefi.core.io.BundleInfo;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.io.LinkManager;
import com.rusefi.ConnectivityContext;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.util.Optional;
import java.util.Properties;
import java.util.Collections;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import javax.swing.SwingUtilities;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.mockito.Mockito.*;

class FirmwareRollbackControllerTest {
    @Test
    void reconnectingTheSameLimaEcuRediscoversItsHistory() throws Exception {
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "true");
        try {
            PortResult port = new PortResult("TEST_PORT", SerialPortType.Ecu);
            LinkManager lm = mock(LinkManager.class);
            BinaryProtocol bp = mock(BinaryProtocol.class);
            bp.signature = "rusEFI lts-26lima.2026.11.11.test_board.123";
            when(lm.getBinaryProtocol()).thenReturn(bp);
            when(lm.getLastTriedPort()).thenReturn(port.port);
            AtomicReference<FirmwareRollbackController> controller = new AtomicReference<>();
            Semaphore completed = new Semaphore(0);
            AtomicInteger discoveries = new AtomicInteger();
            SwingUtilities.invokeAndWait(() -> {
                FirmwareRollbackController value = new FirmwareRollbackController(
                    mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
                    new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY),
                    () -> Optional.of(port), () -> true,
                    () -> {
                        if (controller.get() != null && !controller.get().isBusy()) {
                            completed.release();
                        }
                    }, ignored -> {}, () -> {});
                controller.set(value);
                value.setCatalogLoaderForTests((board, branch) -> {
                    discoveries.incrementAndGet();
                    return Collections.emptyList();
                });
                value.setLinkManager(lm);
                value.refresh(port);
            });
            assertTrue(completed.tryAcquire(5, TimeUnit.SECONDS));
            assertEquals(1, discoveries.get());
            SwingUtilities.invokeAndWait(() -> {
                controller.get().setLinkManager(null);
                controller.get().refresh(port);
                controller.get().setLinkManager(lm);
                controller.get().refresh(port);
            });
            assertTrue(completed.tryAcquire(5, TimeUnit.SECONDS));
            assertEquals(2, discoveries.get());
        } finally {
            System.clearProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        }
    }

    @Test
    void historyUsesLiveEcuBranchAndDoesNotGuessWhenDisconnected() {
        BundleInfo lima = FirmwareRollbackController.connectedFirmware(
            "rusEFI lts-26lima.2026.11.11.test_board.123");
        assertEquals("lts-26lima", lima.getBranchName());
        assertEquals("test_board", lima.getTarget());
        assertTrue(FirmwareRollbackController.isRollbackButtonVisible(true, lima));
        assertFalse(FirmwareRollbackController.isRollbackButtonVisible(true,
            FirmwareRollbackController.connectedFirmware("rusEFI master.2026.11.11.test_board.456")));
        assertNull(FirmwareRollbackController.connectedFirmware(null));
        assertNull(FirmwareRollbackController.connectedFirmware("invalid"));
    }

    @Test
    void serialFirmwareUpdateRejectsSocketCanEcu() {
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult(LinkManager.SOCKET_CAN, SerialPortType.Ecu)));
        assertTrue(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult("COM7", SerialPortType.Ecu)));
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult("COM7", SerialPortType.OpenBlt)));
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult(LinkManager.PCAN, SerialPortType.Ecu)));
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult(LinkManager.PCAN, SerialPortType.EcuWithOpenblt)));
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult("SLCAN:COM7", SerialPortType.Ecu)));
    }

    @Test
    void latestUpdateCannotReplaceConfirmedBWithStaleProviderAOrAnotherConnection() {
        PortResult a = new PortResult("ECU_A", SerialPortType.Ecu);
        PortResult b = new PortResult("ECU_B", SerialPortType.Ecu);
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b, a, true, b.port),
            "startup's independent provider must not override the confirmed selection");
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b, b, true, a.port));
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b,
            new PortResult(b.port, SerialPortType.EcuWithOpenblt), true, b.port));
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b, null, true, b.port));
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b, b, false, b.port));
        assertTrue(FirmwareRollbackController.matchesExpectedUpdateTarget(b,
            new PortResult(b.port, b.type), true, b.port));
        for (String transport : new String[]{LinkManager.PCAN, LinkManager.SOCKET_CAN, "SLCAN:COM7"}) {
            PortResult can = new PortResult(transport, SerialPortType.Ecu);
            assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(can, can, true, can.port));
        }
    }

    @Test
    void enablesHistoryOnlyForLtsBundles() {
        BundleInfo lts = new BundleInfo("lts-26-test", null, "uaefi");
        BundleInfo development = new BundleInfo("development", null, "uaefi");
        BundleInfo release = new BundleInfo("release", null, "uaefi");
        assertTrue(FirmwareRollbackController.isLtsBundle(lts));
        assertFalse(FirmwareRollbackController.isLtsBundle(development));
        assertFalse(FirmwareRollbackController.isLtsBundle(release));
        assertTrue(FirmwareRollbackController.isRollbackButtonVisible(true, lts));
        assertFalse(FirmwareRollbackController.isRollbackButtonVisible(false, lts));
        assertFalse(FirmwareRollbackController.isRollbackButtonVisible(true, development));
    }

    @Test
    void rollbackFeatureIsDefaultOffAndBundleConfigurable() {
        Properties properties = new Properties();
        assertFalse(FirmwareRollbackController.isFirmwareRollbackEnabled(properties));

        properties.setProperty("firmware_rollback_enabled", "true");
        assertTrue(FirmwareRollbackController.isFirmwareRollbackEnabled(properties));
    }

    @Test
    void rollbackFeatureAllowsLocalJvmOverride() {
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "true");
        try {
            assertTrue(FirmwareRollbackController.isFirmwareRollbackEnabled(new Properties()));
        } finally {
            System.clearProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        }
    }

    @Test
    void successfulSessionFlashOverridesStaleScannedHash() {
        assertEquals("new-sha", FirmwareRollbackController.selectCurrentFirmwareSha(
            " new-sha ", Optional.of("old-sha"), "persisted-sha"));
        assertEquals("ecu-sha", FirmwareRollbackController.selectCurrentFirmwareSha(
            null, Optional.of(" ecu-sha "), "persisted-sha"));
        assertEquals("persisted-sha", FirmwareRollbackController.selectCurrentFirmwareSha(
            null, Optional.empty(), "persisted-sha"));
    }
}
