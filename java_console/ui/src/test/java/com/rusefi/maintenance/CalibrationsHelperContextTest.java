package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.CalibrationsHelper.isUiContext;
import static org.junit.jupiter.api.Assertions.*;

public class CalibrationsHelperContextTest {

    @Test
    public void testIsUiContext_withDummyCallbacks() {
        assertFalse(isUiContext(UpdateOperationCallbacks.DUMMY), "DUMMY callbacks should not be considered UI context");
    }

    @Test
    public void testIsUiContext_withLoggerCallbacks() {
        assertFalse(isUiContext(UpdateOperationCallbacks.LOGGER), "LOGGER callbacks should not be considered UI context");
    }

    @Test
    public void testIsUiContext_withConsoleCallbacks() {
        assertTrue(isUiContext(UpdateOperationCallbacks.CONSOLE), "CONSOLE callbacks should be considered UI context");
    }

    @Test
    public void testIsUiContext_withCustomCallbacks() {
        UpdateOperationCallbacks customCallbacks = new UpdateOperationCallbacks() {
            @Override
            public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {
            }

            @Override
            public void done() {
            }

            @Override
            public void warning() {
            }

            @Override
            public void error() {
            }

            @Override
            public void clear() {
            }
        };

        assertTrue(isUiContext(customCallbacks), "Custom callbacks should be considered UI context");
    }
}
