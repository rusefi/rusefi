package com.rusefi.io.can.slcan;

import org.junit.jupiter.api.Test;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import static org.junit.jupiter.api.Assertions.*;

class SlcanSetupTest {
    @Test
    void onlyRecognizedWeActGetsAutomaticRetransmission() throws Exception {
        for (String version : new String[]{null, "V1220", "WeAct Studio",
                "16e7497-dirty github.com/normaldotcom/canable2.git"}) {
            List<String> commands = new ArrayList<>();
            SlcanSetup.openClosedChannel(version, 6, commands::add, message -> {});
            assertEquals(Arrays.asList("S6", "O"), commands, version);
        }
        for (String version : new String[]{"WeAct Studio V1.0.0.3_bb264e71",
                "WeAct Studio V1.0.0.6_4fa52575"}) {
            List<String> commands = new ArrayList<>();
            SlcanSetup.openClosedChannel(version, 5, commands::add, message -> {});
            assertEquals(Arrays.asList("S5", "A1", "O"), commands);
        }
    }

    @Test
    void setupFailurePropagatesWithoutOpeningOrRetrying() {
        for (String rejected : new String[]{"S6", "A1", "O"}) {
            List<String> commands = new ArrayList<>();
            IOException failure = new IOException("rejected " + rejected);
            assertSame(failure, assertThrows(IOException.class, () ->
                SlcanSetup.openClosedChannel("WeAct Studio V1.0.0.6_4fa52575", 6, command -> {
                    commands.add(command);
                    if (command.equals(rejected)) {
                        throw failure;
                    }
                }, message -> {})));
            List<String> sequence = Arrays.asList("S6", "A1", "O");
            assertEquals(sequence.subList(0, sequence.indexOf(rejected) + 1), commands);
        }
    }

    @Test
    void invalidBitrateSendsNothing() {
        List<String> commands = new ArrayList<>();
        for (int bitrate : new int[]{-1, 9}) {
            assertThrows(IllegalArgumentException.class, () ->
                SlcanSetup.openClosedChannel("V1220", bitrate, commands::add, message -> {}));
        }
        assertTrue(commands.isEmpty());
    }
}
