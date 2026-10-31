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
    public void testKansasFiveByteRecords() {
        // Characterizes the Lima-stride regression: Kansas records are 5 bytes each.
        byte[] input = {0, 0, 0, 0, 1, 1, 0, 0, 0, 2, 2, 0, 0, 0, 3, 4};

        List<CompositeEvent> events = CompositeParser.parse(input);

        assertEquals(3, events.size());
        assertEquals(1, events.get(0).isPrimaryTriggerAsInt());
        assertEquals(1, events.get(1).isSecondaryTriggerAsInt());
    }

    @Test
    public void testParseAndExportToVCD() throws IOException {
        byte[] input = {0, 1, 110, -101, 53, 58, 1, 110, -86, -43, 42, 1, 110, -78, 14, 10, 1, 110, -74, -67, 8, 1, 110, -23, -30, 9, 1};

        List<CompositeEvent> events = CompositeParser.parse(input);

        StringWriter writer = new StringWriter();
        //FileWriter writer = new FileWriter("rusEFI.vcd");
        VcdStreamFile.writeVCD(events, writer, new Date(1590847552574L));
    }
}
