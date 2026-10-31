package com.rusefi.composite.test;

import com.rusefi.stream.VcdStreamFile;
import com.rusefi.composite.CompositeEvent;
import com.rusefi.composite.CompositeParser;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringWriter;
import java.util.Date;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class CompositeParserTest {
    @Test
    public void testCurrentFirmwareRecords() {
        // Firmware reverses each entire uint64 record: ACR, injectors, coils, flags, timestamp (BE).
        byte[] input = {0,
                0, 0x21, 0x12, 9, 1, 2, 3, 4,
                1, 2, 1, 6, 1, 2, 3, 5,
                0, 0, 0, 0x30, 1, 2, 3, 6};

        List<CompositeEvent> events = CompositeParser.parse(input);

        // PR #10575: decode the reversed 8-byte firmware record, not the legacy layout.
        assertEquals(3, events.size());
        assertEquals(0x01020304L, events.get(0).getTimestamp() & 0xFFFFFFFFL);
        assertEquals(1, events.get(1).getTimestamp() - events.get(0).getTimestamp());
        assertEquals(1, events.get(2).getTimestamp() - events.get(1).getTimestamp());
        assertEquals(1, events.get(0).isPrimaryTriggerAsInt());
        assertEquals(1, events.get(0).isSyncAsInt());
        assertEquals(0, events.get(1).isPrimaryTriggerAsInt());
        assertEquals(1, events.get(1).isSecondaryTriggerAsInt());
        assertEquals(1, events.get(0).isCoil(1));
        assertEquals(1, events.get(0).isCoil(4));
        assertEquals(1, events.get(0).isInjector(0));
        assertEquals(1, events.get(0).isInjector(5));
        assertEquals(0, events.get(2).isCoil());
        assertEquals(0, events.get(2).isInjector());
    }

    @Test
    public void testLegacyFiveByteRecords() {
        // Historical format is selected explicitly, never inferred from response length.
        byte[] input = {0, 0, 0, 0, 1, 1, 0, 0, 0, 2, 2, 0, 0, 0, 3, 4};

        List<CompositeEvent> events = CompositeParser.parseLegacy(input);

        assertEquals(3, events.size());
        assertEquals(1, events.get(0).isPrimaryTriggerAsInt());
        assertEquals(1, events.get(1).isSecondaryTriggerAsInt());
    }

    @Test
    public void testParseAndExportToVCD() throws IOException {
        byte[] input = {0, 1, 110, -101, 53, 58, 1, 110, -86, -43, 42, 1, 110, -78, 14, 10, 1, 110, -74, -67, 8, 1, 110, -23, -30, 9, 1};

        List<CompositeEvent> events = CompositeParser.parseLegacy(input);

        StringWriter writer = new StringWriter();
        //FileWriter writer = new FileWriter("rusEFI.vcd");
        VcdStreamFile.writeVCD(events, writer, new Date(1590847552574L));
    }
}
