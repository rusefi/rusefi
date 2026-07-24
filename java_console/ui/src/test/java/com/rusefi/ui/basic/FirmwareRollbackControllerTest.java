package com.rusefi.ui.basic;

import com.rusefi.core.io.BundleInfo;
import org.junit.jupiter.api.Test;

import java.util.Properties;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class FirmwareRollbackControllerTest {
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
}
