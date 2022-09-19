package com.rusefi.composite;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.generated.Fields;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

public class CompositeParser {
    private static final Logging log = Logging.getLogging(CompositeParser.class);

	private static long prevTime = 0;
	private static long timeAdder = 0;

    public static List<CompositeEvent> parse(byte[] response) {
        ByteBuffer byteBuffer = ByteBuffer.wrap(response);
        byteBuffer.order(ByteOrder.BIG_ENDIAN);
        int ptr = 1;

        List<CompositeEvent> events = new ArrayList<>();

        while (ptr + Fields.COMPOSITE_PACKET_SIZE <= response.length) {

            // Convert the int to a long since java can't do unsigned ints,
            // so we have to use a long (but still only want 32 bits read)
            long timestamp = Integer.toUnsignedLong (byteBuffer.getInt(ptr));
            byte flags = byteBuffer.get(ptr + 4);
//            log.debug(timestamp + " " + flags);

            boolean primaryTrigger = EnumIniField.getBit(flags, 0);
            boolean secondaryTrigger = EnumIniField.getBit(flags, 1);
            boolean trg = EnumIniField.getBit(flags, 2);
            boolean sync = EnumIniField.getBit(flags, 3);
            boolean coil = EnumIniField.getBit(flags, 4);
            boolean injector  = EnumIniField.getBit(flags, 5);

            ptr += Fields.COMPOSITE_PACKET_SIZE;

            // If the timestamp went down, that means we just witnessed an integer overflow
            if (timestamp < prevTime) {
                // Add the maximum value of a uint32_t, plus one
                timeAdder += 0xFFFFFFFFL + 1;
            }

            prevTime = timestamp;

            long curTime = timeAdder + timestamp;

            events.add(new CompositeEvent(curTime, primaryTrigger, secondaryTrigger, trg, sync, coil, injector));
        }
        return events;
    }
}
