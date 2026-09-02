package com.rusefi.io.can;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class ClassicCanFrameTest {
    @Test
    void defensivelyCopiesPayloadAtBothBoundaries() {
        byte[] payload = {1, 2};
        ClassicCanFrame frame = new ClassicCanFrame(new CanAddress(0x123, false), payload);

        payload[0] = 3;
        byte[] received = frame.getPayload();
        received[1] = 4;

        assertArrayEquals(new byte[]{1, 2}, frame.getPayload());
    }

    @Test
    void rejectsPayloadLargerThanClassicCan() {
        assertThrows(IllegalArgumentException.class,
            () -> new ClassicCanFrame(new CanAddress(0x123, false), new byte[9]));
    }
}
