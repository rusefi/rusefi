package com.rusefi.composite;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

public class CompositeParser {
    private static final Logging log = Logging.getLogging(CompositeParser.class);

	private static long prevTime = 0;
	private static long timeAdder = 0;

    /** Response-code byte followed by the current firmware's reversed 8-byte records. */
    public static List<CompositeEvent> parse(byte[] response) {
        return parse(response, false);
    }

    /** Historical timestamp-then-flags records, not the current firmware wire format. */
    public static List<CompositeEvent> parseLegacy(byte[] response) {
        return parse(response, true);
    }

    private static List<CompositeEvent> parse(byte[] response, boolean legacy) {
        ByteBuffer byteBuffer = ByteBuffer.wrap(response);
        byteBuffer.order(ByteOrder.BIG_ENDIAN);
        int ptr = 1;
        // tooth_logger.cpp reverses the whole 8-byte composite_logger_s, not just its timestamp.
        int recordSize = legacy ? 5 : 8;

        List<CompositeEvent> events = new ArrayList<>();

        while (ptr + recordSize <= response.length) {

            // Convert the int to a long since java can't do unsigned ints,
            // so we have to use a long (but still only want 32 bits read)
            long timestamp = Integer.toUnsignedLong(byteBuffer.getInt(ptr + (legacy ? 0 : 4)));
            byte flags = byteBuffer.get(ptr + (legacy ? 4 : 3));
//            log.debug(timestamp + " " + flags);

            boolean primaryTrigger = ConfigurationImage.getBit(flags, 0);
            boolean secondaryTrigger = ConfigurationImage.getBit(flags, 1);
            boolean trg = ConfigurationImage.getBit(flags, 2);
            boolean sync = ConfigurationImage.getBit(flags, 3);
            // CompositeEvent currently exposes six coil/injector channels.
            boolean[] coils = new boolean[6];
            boolean[] injectors = new boolean[6];
            if (legacy) {
                coils[0] = ConfigurationImage.getBit(flags, 4);
                injectors[0] = ConfigurationImage.getBit(flags, 5);
            } else {
                for (int i = 0; i < coils.length; i++) {
                    coils[i] = ConfigurationImage.getBit(byteBuffer.get(ptr + 2), i);
                    injectors[i] = ConfigurationImage.getBit(byteBuffer.get(ptr + 1), i);
                }
            }

            ptr += recordSize;

            // If the timestamp went down, that means we just witnessed an integer overflow
            if (timestamp < prevTime) {
                // Add the maximum value of a uint32_t, plus one
                timeAdder += 0xFFFFFFFFL + 1;
            }

            prevTime = timestamp;

            long curTime = timeAdder + timestamp;

            events.add(new CompositeEvent(curTime, primaryTrigger, secondaryTrigger, trg, sync, coils, injectors));
        }
        return events;
    }
}
