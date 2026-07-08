package com.rusefi;

import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.HashSet;
import java.util.Optional;

import static java.util.Arrays.asList;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Direct tests for the scanner's port cache — previously covered only indirectly through
 * {@code SerialPortScannerTest}. The cache is what prevents re-probing a known ECU port every
 * 300ms scan cycle, so eviction semantics matter. [tag:better_ux_for_flashing]
 */
public class SerialPortCacheTest {

    @Test
    public void getOfUnknownPortReturnsEmpty() {
        SerialPortCache cache = new SerialPortCache();
        assertFalse(cache.get("COM1").isPresent());
    }

    @Test
    public void putThenGetReturnsCachedResult() {
        SerialPortCache cache = new SerialPortCache();
        PortResult ecu = new PortResult("COM1", SerialPortType.Ecu);
        cache.put(ecu);
        Optional<PortResult> cached = cache.get("COM1");
        assertTrue(cached.isPresent());
        assertSame(ecu, cached.get());
    }

    @Test
    public void putOverwritesPreviousEntryForSamePortName() {
        SerialPortCache cache = new SerialPortCache();
        cache.put(new PortResult("COM1", SerialPortType.Unknown));
        PortResult reInspected = new PortResult("COM1", SerialPortType.Ecu);
        cache.put(reInspected);
        assertSame(reInspected, cache.get("COM1").get());
    }

    @Test
    public void retainAllEvictsOnlyDisappearedPorts() {
        SerialPortCache cache = new SerialPortCache();
        cache.put(new PortResult("COM1", SerialPortType.Ecu));
        cache.put(new PortResult("COM2", SerialPortType.OpenBlt));
        cache.put(new PortResult("COM3", SerialPortType.Ecu));

        // COM2 got unplugged / renumbered: only it must be evicted
        cache.retainAll(new HashSet<>(asList("COM1", "COM3")));

        assertTrue(cache.get("COM1").isPresent());
        assertFalse(cache.get("COM2").isPresent(), "disappeared port must be evicted so a replug re-inspects it");
        assertTrue(cache.get("COM3").isPresent());
    }

    @Test
    public void retainAllWithEmptySetEvictsEverything() {
        SerialPortCache cache = new SerialPortCache();
        cache.put(new PortResult("COM1", SerialPortType.Ecu));
        cache.retainAll(Collections.emptySet());
        assertFalse(cache.get("COM1").isPresent());
    }

    @Test
    public void invalidateRemovesSinglePortOnly() {
        SerialPortCache cache = new SerialPortCache();
        cache.put(new PortResult("COM1", SerialPortType.Ecu));
        cache.put(new PortResult("COM2", SerialPortType.Ecu));

        cache.invalidate("COM1");

        assertFalse(cache.get("COM1").isPresent());
        assertTrue(cache.get("COM2").isPresent());
        assertEquals(SerialPortType.Ecu, cache.get("COM2").get().type);
    }
}
