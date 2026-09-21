package com.rusefi;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class UiPropertiesTest {
    @Test
    void canOpenBltSystemPropertyOverridesTheResource() {
        String previous = System.getProperty(UiProperties.SHOW_CAN_OPENBLT);
        try {
            System.setProperty(UiProperties.SHOW_CAN_OPENBLT, "false");
            assertFalse(UiProperties.isCanOpenBltEnabled());

            System.setProperty(UiProperties.SHOW_CAN_OPENBLT, "true");
            assertTrue(UiProperties.isCanOpenBltEnabled());
        } finally {
            if (previous == null) {
                System.clearProperty(UiProperties.SHOW_CAN_OPENBLT);
            } else {
                System.setProperty(UiProperties.SHOW_CAN_OPENBLT, previous);
            }
        }
    }
}
