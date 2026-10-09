package com.rusefi.io.can.slcan;

import com.rusefi.io.can.CanAddress;
import com.rusefi.io.can.ClassicCanFrame;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class SlcanCodecTest {
    @Test
    void encodesAndDecodesDataOnEveryBus() {
        for (int bus = 0; bus < 3; bus++) {
            for (boolean extended : new boolean[]{false, true}) {
                for (int length = 0; length <= 8; length++) {
                    byte[] data = new byte[length];
                    for (int i = 0; i < length; i++) {
                        data[i] = (byte) (0xf0 + i);
                    }
                    ClassicCanFrame original = new ClassicCanFrame(
                        new CanAddress(extended ? 0x1fffffff : 0x7ff, extended), data);
                    SlcanCodec.Frame decoded = SlcanCodec.decode(SlcanCodec.encode(original, bus));
                    assertNotNull(decoded);
                    assertEquals(bus, decoded.busIndex);
                    assertEquals(original.getAddress(), decoded.address);
                    assertEquals(length, decoded.dlc);
                    assertFalse(decoded.rtr);
                    assertNull(decoded.timestamp);
                    assertArrayEquals(data, decoded.data);
                }
            }
        }
        assertEquals("&t7E030102FF", SlcanCodec.encode(new ClassicCanFrame(
            new CanAddress(0x7e0, false), new byte[]{1, 2, (byte) 0xff}), 1));
    }

    @Test
    void acceptsRemoteFramesAndHexTimestamps() {
        for (int bus = 0; bus < 3; bus++) {
            String prefix = new String[]{"", "&", "$"}[bus];
            for (String line : new String[]{"r1238", "R000001238"}) {
                SlcanCodec.Frame frame = SlcanCodec.decode(prefix + line + "aBcD");
                assertNotNull(frame);
                assertTrue(frame.rtr);
                assertEquals(bus, frame.busIndex);
                assertEquals(8, frame.dlc);
                assertEquals(0, frame.data.length);
                assertEquals("aBcD", frame.timestamp);
            }
        }
        SlcanCodec.Frame data = SlcanCodec.decode("t1231ff0123");
        assertNotNull(data);
        assertEquals("0123", data.timestamp);
        assertArrayEquals(new byte[]{(byte) 0xff}, data.data);
    }

    @Test
    void rejectsMalformedAndNonFrameLines() {
        for (String line : new String[]{null, "", "&", "$", "z", "V1220", "F00", "\u0007",
                "d1230", "tFFF0", "T200000000", "TFFFFFFFF0", "t1239", "r1239",
                "t1232AA", "t1231GG", "t123100123", "t123100ZZZZ", "r123801",
                "&&t1230", "$&t1230", "t+120", "t1230\r"}) {
            assertNull(SlcanCodec.decode(line), line);
        }
    }

    @Test
    void rejectsInvalidBusIndices() {
        ClassicCanFrame frame = new ClassicCanFrame(new CanAddress(0x123, false), new byte[0]);
        assertThrows(IllegalArgumentException.class, () -> SlcanCodec.encode(frame, -1));
        assertThrows(IllegalArgumentException.class, () -> SlcanCodec.encode(frame, 3));
    }
}
