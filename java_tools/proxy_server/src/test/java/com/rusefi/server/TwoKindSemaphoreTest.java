package com.rusefi.server;

import org.junit.Test;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class TwoKindSemaphoreTest {
    @Test
    public void testTwoKindSemaphore() {
        TwoKindSemaphore twoKindSemaphore = new TwoKindSemaphore();

        assertFalse(twoKindSemaphore.isUsed());
        assertTrue(twoKindSemaphore.acquireForShortTermUsage());
        assertFalse(twoKindSemaphore.isUsed());
        twoKindSemaphore.releaseFromShortTermUsage();


        assertTrue(twoKindSemaphore.acquireForLongTermUsage());
        assertTrue(twoKindSemaphore.isUsed());
        //
        assertFalse(twoKindSemaphore.acquireForLongTermUsage(1));
        assertFalse(twoKindSemaphore.acquireForShortTermUsage());
    }
}
