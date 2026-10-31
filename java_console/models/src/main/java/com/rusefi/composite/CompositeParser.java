package com.rusefi.composite;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.config.generated.Integration;

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

        while (ptr + Integration.COMPOSITE_PACKET_SIZE <= response.length) {

            // Convert the int to a long since java can't do unsigned ints,
            // so we have to use a long (but still only want 32 bits read)
            long timestamp = Integer.toUnsignedLong (byteBuffer.getInt(ptr));
            byte flags = byteBuffer.get(ptr + 4);
//            log.debug(timestamp + " " + flags);

            boolean primaryTrigger = ConfigurationImage.getBit(flags, 0);
            boolean secondaryTrigger = ConfigurationImage.getBit(flags, 1);
            boolean trg = ConfigurationImage.getBit(flags, 2);
            boolean sync = ConfigurationImage.getBit(flags, 3);
            boolean coil = ConfigurationImage.getBit(flags, 4);
            boolean injector  = ConfigurationImage.getBit(flags, 5);

            ptr += Integration.COMPOSITE_PACKET_SIZE;

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
