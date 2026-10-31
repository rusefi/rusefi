package com.rusefi.core;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class SensorSubscriptionTest {
    @Test
    public void testSubscription() {
        SensorSubscription sub = new SensorSubscription("RPM", "Coolant");
        assertTrue(sub.isInterestedIn("rpm"));
        assertTrue(sub.isInterestedIn("COOLANT"));
        assertFalse(sub.isInterestedIn("MAP"));
    }

    @Test
    public void testEmptySubscription() {
        SensorSubscription sub = new SensorSubscription();
        assertTrue(sub.isInterestedIn("any"));
    }
}
