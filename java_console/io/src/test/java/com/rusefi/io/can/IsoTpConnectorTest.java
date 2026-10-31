package com.rusefi.io.can;

import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.io.can.isotp.IsoTpConnector;
import com.rusefi.util.HexBinary;
import org.junit.jupiter.api.Test;

import java.io.UncheckedIOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import static org.junit.jupiter.api.Assertions.*;

public class IsoTpConnectorTest {
    private static final String PACING_PROPERTY = "isotp_consecutive_frame_delay_ms";

    private static class RecordingConnector extends IsoTpConnector {
        final List<String> events = new ArrayList<>();

        RecordingConnector() {
            super(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID);
        }

        public void sendCanData(byte[] bytes) {
            events.add("frame:" + (bytes[0] & 0xff));
        }

        public void receiveData() {
            events.add("flow-control");
        }

        // Test seam for recording delays without depending on OS sleep accuracy.
        protected void pauseBetweenConsecutiveFrames(int milliseconds) {
            events.add("delay:" + milliseconds);
        }
    }

    private static void withPacing(String value, Runnable test) {
        Properties properties = PropertiesHolder.INSTANCE.getProperties();
        Object old = properties.remove(PACING_PROPERTY);
        if (value != null) {
            properties.setProperty(PACING_PROPERTY, value);
        }
        try {
            test.run();
        } finally {
            properties.remove(PACING_PROPERTY);
            if (old != null) {
                properties.put(PACING_PROPERTY, old);
            }
        }
    }

    @Test
    void configuredPacing() {
        withPacing("1", () -> {
            RecordingConnector connector = new RecordingConnector();
            IsoTpConnector.sendStrategy(new byte[30], connector);
            // Delay only between CFs: no extra delay before CF1 or after the final CF.
            assertEquals(List.of("frame:16", "flow-control", "frame:33", "delay:1", "frame:34",
                    "delay:1", "frame:35", "delay:1", "frame:36"), connector.events);
        });
    }

    @Test
    void defaultAndExplicitZeroRemainUnpaced() {
        for (String setting : new String[]{null, "0"}) {
            withPacing(setting, () -> {
                RecordingConnector connector = new RecordingConnector();
                IsoTpConnector.sendStrategy(new byte[30], connector);
                assertEquals(List.of("frame:16", "flow-control", "frame:33", "frame:34",
                        "frame:35", "frame:36"), connector.events);
            });
        }
    }

    @Test
    void shortPacketsNeverSleep() {
        withPacing(" 7 ", () -> {
            RecordingConnector connector = new RecordingConnector();
            IsoTpConnector.sendStrategy(new byte[7], connector);
            assertEquals(List.of("frame:7"), connector.events);
            connector.events.clear();
            IsoTpConnector.sendStrategy(new byte[13], connector);
            assertEquals(List.of("frame:16", "flow-control", "frame:33"), connector.events);
            connector.events.clear();
            IsoTpConnector.sendStrategy(new byte[14], connector);
            assertEquals(List.of("frame:16", "flow-control", "frame:33", "delay:7", "frame:34"),
                    connector.events);
        });
    }

    @Test
    void invalidConfigurationFailsBeforeSending() {
        for (String setting : new String[]{"-1", "abc", "1.5", "2147483648", ""}) {
            withPacing(setting, () -> {
                RecordingConnector connector = new RecordingConnector();
                IllegalArgumentException error = assertThrows(IllegalArgumentException.class,
                        () -> IsoTpConnector.sendStrategy(new byte[30], connector));
                assertTrue(error.getMessage().contains(PACING_PROPERTY));
                assertTrue(connector.events.isEmpty());
            });
        }
    }

    @Test
    void interruptionAbortsTheRemainingFrames() {
        withPacing("1", () -> {
            List<Integer> frames = new ArrayList<>();
            IsoTpConnector connector = new IsoTpConnector(0x710) {
                public void sendCanData(byte[] data) {
                    frames.add(data[0] & 0xff);
                    if (data[0] == 0x21) {
                        Thread.currentThread().interrupt();
                    }
                }
            };
            try {
                assertThrows(UncheckedIOException.class,
                        () -> IsoTpConnector.sendStrategy(new byte[30], connector));
                assertTrue(Thread.currentThread().isInterrupted());
                assertEquals(List.of(0x10, 0x21), frames);
            } finally {
                Thread.interrupted();
            }
        });
    }

    @Test
    public void testConnector() {
        byte[] crcWrappedCrcRequest = new byte[]{
                0, 5, 107, 0, 0, 80, 95, 105, -81, -96, 112};

        List<String> packets = new ArrayList<>();

        IsoTpConnector testConnector = new IsoTpConnector(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID) {
            @Override
            public void sendCanData(byte[] total) {
                String packetAsString = HexBinary.printHexBinary(total);
                packets.add(packetAsString);
            }
        };

        IsoTpConnector.sendStrategy(crcWrappedCrcRequest, testConnector);

        assertEquals(2, packets.size());
        assertEquals("10 0B 00 05 6B 00 00 50 ", packets.get(0));
        assertEquals("21 5F 69 AF A0 70 ", packets.get(1));
    }
}
