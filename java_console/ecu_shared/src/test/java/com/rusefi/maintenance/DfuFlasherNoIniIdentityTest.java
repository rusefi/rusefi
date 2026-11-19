package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

class DfuFlasherNoIniIdentityTest {
    @Test
    void signatureMismatchRejectsBeforeTheRebootCommandCanRun() {
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        AtomicInteger rebootCalls = new AtomicInteger();

        assertNull(DfuFlasher.rebootAfterSignatureCheck("expected-signature", "different-signature",
            callbacks, signature -> {
                rebootCalls.incrementAndGet();
                return true;
            }));

        assertEquals(0, rebootCalls.get());
        verify(callbacks).error();
    }

    @Test
    void exactSignatureAllowsTheRebootCommand() {
        AtomicInteger rebootCalls = new AtomicInteger();

        assertTrue(DfuFlasher.rebootAfterSignatureCheck("same-signature", "same-signature",
            UpdateOperationCallbacks.DUMMY, signature -> {
                rebootCalls.incrementAndGet();
                return true;
            }).get());

        assertEquals(1, rebootCalls.get());
    }
}
