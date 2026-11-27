package com.rusefi.veautotune

import org.junit.Assert.assertEquals
import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Test

class BadFrameTrackerTest {

    @Test
    fun `not dead until maxConsecutive failures in a row`() {
        val tracker = BadFrameTracker(maxConsecutive = 3)

        assertFalse(tracker.recordFailure())
        assertEquals(1, tracker.consecutiveFailures)

        assertFalse(tracker.recordFailure())
        assertEquals(2, tracker.consecutiveFailures)

        assertTrue(tracker.recordFailure())
        assertEquals(3, tracker.consecutiveFailures)
    }

    @Test
    fun `a success resets the streak`() {
        val tracker = BadFrameTracker(maxConsecutive = 3)

        tracker.recordFailure()
        tracker.recordFailure()
        tracker.recordSuccess()
        assertEquals(0, tracker.consecutiveFailures)

        // After a reset, it takes a full new streak of maxConsecutive to go dead.
        assertFalse(tracker.recordFailure())
        assertFalse(tracker.recordFailure())
        assertTrue(tracker.recordFailure())
    }

    @Test
    fun `single bad frame never trips a tracker with max greater than one`() {
        val tracker = BadFrameTracker(maxConsecutive = 5)
        assertFalse(tracker.recordFailure())
        assertEquals(1, tracker.consecutiveFailures)
    }
}
