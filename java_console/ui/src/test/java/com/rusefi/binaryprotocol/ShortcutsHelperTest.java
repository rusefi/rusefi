package com.rusefi.binaryprotocol;

import org.junit.jupiter.api.Test;

import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;

class ShortcutsHelperTest {
    @Test
    void connectionActionRunsOnlyWhenAllowed() {
        AtomicInteger calls = new AtomicInteger();

        ShortcutsHelper.runIfAllowed(() -> false, calls::incrementAndGet);
        ShortcutsHelper.runIfAllowed(() -> true, calls::incrementAndGet);

        assertEquals(1, calls.get());
    }
}
