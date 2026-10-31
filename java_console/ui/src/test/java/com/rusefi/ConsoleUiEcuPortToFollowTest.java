package com.rusefi;

import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;

/**
 * Tests {@link ConsoleUI#ecuPortToFollow} — the follow-a-renumbered-ECU-across-reboot rule behind
 * the OpenBLT/DFU round-trip recovery (#9771): repoint the live session only when the port we were
 * on has vanished AND the scanner sees exactly one candidate ECU on a different port.
 * [tag:better_ux_for_flashing]
 */
public class ConsoleUiEcuPortToFollowTest {
    private static final String OLD_PORT = "/dev/ttyACM0";
    private static final String NEW_PORT = "/dev/ttyACM1";

    private static final Set<String> NO_PORTS = Collections.emptySet();

    private static List<PortResult> ecus(final String... ports) {
        return Arrays.stream(ports).map(p -> new PortResult(p, SerialPortType.Ecu))
            .collect(java.util.stream.Collectors.toList());
    }

    @Test
    public void followsTheSingleEcuOnANewPortOnceTheOldPortVanished() {
        final Optional<String> follow = ConsoleUI.ecuPortToFollow(OLD_PORT, NO_PORTS, ecus(NEW_PORT), false);

        assertEquals(Optional.of(NEW_PORT), follow);
    }

    @Test
    public void ecuWithOpenbltCountsAsAConnectableCandidate() {
        final List<PortResult> candidate = Collections.singletonList(
            new PortResult(NEW_PORT, SerialPortType.EcuWithOpenblt));

        assertEquals(Optional.of(NEW_PORT), ConsoleUI.ecuPortToFollow(OLD_PORT, NO_PORTS, candidate, false));
    }

    @Test
    public void disconnectedByUserNeverFollowsEvenWithAPerfectCandidate() {
        assertFalse(ConsoleUI.ecuPortToFollow(OLD_PORT, NO_PORTS, ecus(NEW_PORT), true).isPresent());
    }

    @Test
    public void staysPutWhileTheCurrentPortIsStillPresent() {
        // the watchdog's same-name restart() owns the reconnect while the OS node exists
        final Set<String> commPorts = Collections.singleton(OLD_PORT);

        assertFalse(ConsoleUI.ecuPortToFollow(OLD_PORT, commPorts, ecus(NEW_PORT), false).isPresent());
    }

    @Test
    public void neverConnectedSessionHasNothingToFollow() {
        assertFalse(ConsoleUI.ecuPortToFollow(null, NO_PORTS, ecus(NEW_PORT), false).isPresent());
    }

    @Test
    public void zeroCandidatesMeansKeepWaiting() {
        assertFalse(ConsoleUI.ecuPortToFollow(OLD_PORT, NO_PORTS, ecus(), false).isPresent());
    }

    @Test
    public void multipleCandidatesAreAmbiguousSoStayPut() {
        // two ECUs on the bus — we cannot know which one is our board, do not hijack a stranger
        assertFalse(ConsoleUI.ecuPortToFollow(OLD_PORT, NO_PORTS, ecus(NEW_PORT, "/dev/ttyACM2"), false).isPresent());
    }

    @Test
    public void candidateEqualToTheCurrentPortIsNotARenumbering() {
        // scanner snapshot and OS port list can be momentarily inconsistent: the scanner still
        // reports our port as an ECU while the OS list already dropped it — no port to follow
        assertFalse(ConsoleUI.ecuPortToFollow(OLD_PORT, NO_PORTS, ecus(OLD_PORT), false).isPresent());
    }
}
