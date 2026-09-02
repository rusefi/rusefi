package com.rusefi.io.can;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class CanAddressTest {
    @Test
    void acceptsTheLimitsForBothAddressFormats() {
        assertEquals(0x7ff, new CanAddress(0x7ff, false).getId());
        assertEquals(0x1fffffff, new CanAddress(0x1fffffff, true).getId());
        assertFalse(new CanAddress(1, false).isExtended());
        assertTrue(new CanAddress(1, true).isExtended());
    }

    @Test
    void rejectsIdsOutsideTheSelectedAddressFormat() {
        assertThrows(IllegalArgumentException.class, () -> new CanAddress(-1, false));
        assertThrows(IllegalArgumentException.class, () -> new CanAddress(0x800, false));
        assertThrows(IllegalArgumentException.class, () -> new CanAddress(0x20000000, true));
    }
}
