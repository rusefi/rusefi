package com.rusefi.ui.basic;

import com.rusefi.ui.widgets.StatusPanel;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class TuneOperationStatusPanelTest {
    @Test
    void showsProgressAndPersistentResults() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean continued = new AtomicBoolean();
            TuneOperationStatusPanel panel = new TuneOperationStatusPanel(
                new StatusPanel(100), () -> continued.set(true));

            panel.showProgress("Loading tune...");
            assertEquals("Loading tune...", panel.getResultTextForUnitTest());
            assertTrue(panel.isProgressVisibleForUnitTest());
            assertFalse(panel.isContinueVisibleForUnitTest());

            panel.showResult("Tune loaded successfully", "Tune load failed", false);
            assertEquals("Tune loaded successfully", panel.getResultTextForUnitTest());
            assertFalse(panel.isProgressVisibleForUnitTest());
            assertFalse(panel.isCopyLogVisibleForUnitTest());
            assertTrue(panel.isContinueVisibleForUnitTest());
            panel.continueForUnitTest();
            assertTrue(continued.get());

            panel.showProgress("Importing tune...");
            panel.showResult("Tune imported successfully", "Tune import failed", true);
            assertEquals("Tune import failed", panel.getResultTextForUnitTest());
            assertTrue(panel.isCopyLogVisibleForUnitTest());
            assertTrue(panel.isContinueVisibleForUnitTest());
            assertEquals(panel.getCopyLogSizeForUnitTest(), panel.getContinueSizeForUnitTest());
        });
    }
}
