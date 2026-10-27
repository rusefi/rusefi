package com.rusefi.ui.basic;

import com.rusefi.core.io.BundleInfo;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;

import java.util.Optional;
import java.util.Properties;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class FirmwareRollbackControllerTest {
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
