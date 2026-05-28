package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.lang.reflect.Method;

import static org.junit.jupiter.api.Assertions.*;

public class CalibrationsHelperContextTest {

    @Test
    public void testIsUiContext_withDummyCallbacks() throws Exception {
        Method method = CalibrationsHelper.class.getDeclaredMethod("isUiContext", UpdateOperationCallbacks.class);
        method.setAccessible(true);

        boolean result = (boolean) method.invoke(null, UpdateOperationCallbacks.DUMMY);
        assertFalse(result, "DUMMY callbacks should not be considered UI context");
    }

    @Test
    public void testIsUiContext_withLoggerCallbacks() throws Exception {
        Method method = CalibrationsHelper.class.getDeclaredMethod("isUiContext", UpdateOperationCallbacks.class);
        method.setAccessible(true);

        boolean result = (boolean) method.invoke(null, UpdateOperationCallbacks.LOGGER);
        assertFalse(result, "LOGGER callbacks should not be considered UI context");
    }

    @Test
    public void testIsUiContext_withConsoleCallbacks() throws Exception {
        Method method = CalibrationsHelper.class.getDeclaredMethod("isUiContext", UpdateOperationCallbacks.class);
        method.setAccessible(true);

        boolean result = (boolean) method.invoke(null, UpdateOperationCallbacks.CONSOLE);
        assertTrue(result, "CONSOLE callbacks should be considered UI context");
    }

    @Test
    public void testIsUiContext_withCustomCallbacks() throws Exception {
        Method method = CalibrationsHelper.class.getDeclaredMethod("isUiContext", UpdateOperationCallbacks.class);
        method.setAccessible(true);

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

        boolean result = (boolean) method.invoke(null, customCallbacks);
        assertTrue(result, "Custom callbacks should be considered UI context");
    }
}
