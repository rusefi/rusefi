package com.rusefi.io;

import org.junit.jupiter.api.Test;
import static org.mockito.Mockito.*;

class DoubleCallbacksTest {
    @Test
    void preservesFirmwareLifecycleNotificationsForBothConsumers() {
        UpdateOperationCallbacks first = mock(UpdateOperationCallbacks.class);
        UpdateOperationCallbacks second = mock(UpdateOperationCallbacks.class);
        DoubleCallbacks callbacks = new DoubleCallbacks(first, second);

        callbacks.firmwareUpdateBlocked("Intermediate update required");
        callbacks.firmwareHandoffStarted();

        verify(first).firmwareUpdateBlocked("Intermediate update required");
        verify(second).firmwareUpdateBlocked("Intermediate update required");
        verify(first).firmwareHandoffStarted();
        verify(second).firmwareHandoffStarted();
        verifyNoMoreInteractions(first, second);
    }
}
