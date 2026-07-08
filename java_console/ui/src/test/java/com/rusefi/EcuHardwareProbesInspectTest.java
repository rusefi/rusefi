package com.rusefi;

import com.fazecast.jSerialComm.SerialPortInvalidPortException;
import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.maintenance.CalibrationsInfo;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Tests for the port classification/retry decision tree in {@link EcuHardwareProbes#inspect}, driven
 * through a scripted {@link EcuHardwareProbes.PortProbe} — no serial ports, no real 200ms backoff.
 * This tree decides what the whole console sees in the ports list (OpenBLT-first probing, the
 * 3-attempt ECU retry, stale-node dropping), so its edges are where detection bugs live.
 * [tag:better_ux_for_flashing]
 */
public class EcuHardwareProbesInspectTest {
    private static final String PORT = "COM7";

    /** Scripted probe: per-call ECU-read outcomes, fixed OpenBLT answers, counted sleeps. */
    private static class FakePortProbe implements EcuHardwareProbes.PortProbe {
        boolean openbltPort;
        boolean openbltPortThrowsStaleNode;
        boolean ecuHasOpenblt;
        boolean interruptOnSleep;
        /** one entry per readEcuCalibrations call: an Optional to return, or a RuntimeException to throw */
        final Deque<Object> ecuReadOutcomes = new ArrayDeque<>();

        int ecuReadCalls;
        int hasOpenbltCalls;
        int sleepCalls;

        @Override
        public boolean isPortOpenblt(String port) {
            if (openbltPortThrowsStaleNode) {
                throw new SerialPortInvalidPortException("stale OS node", null);
            }
            return openbltPort;
        }

        @SuppressWarnings("unchecked")
        @Override
        public Optional<CalibrationsInfo> readEcuCalibrations(String port) {
            ecuReadCalls++;
            Object outcome = ecuReadOutcomes.removeFirst();
            if (outcome instanceof RuntimeException) {
                throw (RuntimeException) outcome;
            }
            return (Optional<CalibrationsInfo>) outcome;
        }

        @Override
        public boolean ecuHasOpenblt(String port) {
            hasOpenbltCalls++;
            return ecuHasOpenblt;
        }

        @Override
        public void sleepBetweenAttempts() throws InterruptedException {
            sleepCalls++;
            if (interruptOnSleep) {
                throw new InterruptedException("test interrupt");
            }
        }
    }

    private static CalibrationsInfo fakeCalibrations() {
        // PortResult only dereferences getImage().getMeta() (null meta => unknown signature)
        CalibrationsInfo calibrations = mock(CalibrationsInfo.class);
        when(calibrations.getImage()).thenReturn(mock(ConfigurationImageWithMeta.class));
        return calibrations;
    }

    @AfterEach
    public void clearInterruptFlag() {
        // the interrupt-during-backoff path re-interrupts the thread; never leak that into other tests
        Thread.interrupted();
    }

    @Test
    public void openBltPortIsClassifiedWithoutEcuProbing() {
        FakePortProbe probe = new FakePortProbe();
        probe.openbltPort = true;

        PortResult result = EcuHardwareProbes.inspect(PORT, probe);

        assertEquals(new PortResult(PORT, SerialPortType.OpenBlt), result);
        // A board in the bootloader must not be poked with ECU protocol probes
        assertEquals(0, probe.ecuReadCalls);
        assertEquals(0, probe.hasOpenbltCalls);
    }

    @Test
    public void staleOsNodeIsDroppedEntirely() {
        FakePortProbe probe = new FakePortProbe();
        probe.openbltPortThrowsStaleNode = true;

        // null (not Unknown) so the scan filters the port out of the list
        assertNull(EcuHardwareProbes.inspect(PORT, probe));
        assertEquals(0, probe.ecuReadCalls);
    }

    @Test
    public void portVanishingDuringEcuDetectionIsDropped() {
        FakePortProbe probe = new FakePortProbe();
        probe.ecuReadOutcomes.add(new SerialPortInvalidPortException("unplugged mid-probe", null));

        assertNull(EcuHardwareProbes.inspect(PORT, probe));
    }

    @Test
    public void ecuWithBootloaderSupportKeepsItsCalibrations() {
        FakePortProbe probe = new FakePortProbe();
        CalibrationsInfo calibrations = fakeCalibrations();
        probe.ecuReadOutcomes.add(Optional.of(calibrations));
        probe.ecuHasOpenblt = true;

        PortResult result = EcuHardwareProbes.inspect(PORT, probe);

        assertEquals(SerialPortType.EcuWithOpenblt, result.type);
        assertSame(calibrations, result.getCalibrations(),
            "the calibrations read during detection must ride along on the PortResult");
    }

    @Test
    public void ecuWithoutBootloaderSupportIsPlainEcu() {
        FakePortProbe probe = new FakePortProbe();
        probe.ecuReadOutcomes.add(Optional.of(fakeCalibrations()));
        probe.ecuHasOpenblt = false;

        assertEquals(SerialPortType.Ecu, EcuHardwareProbes.inspect(PORT, probe).type);
    }

    @Test
    public void freshlyRebootedEcuIsFoundOnRetry() {
        // A freshly rebooted/reconnected ECU may not answer the first attempts
        FakePortProbe probe = new FakePortProbe();
        probe.ecuReadOutcomes.add(Optional.empty());
        probe.ecuReadOutcomes.add(Optional.empty());
        probe.ecuReadOutcomes.add(Optional.of(fakeCalibrations()));

        PortResult result = EcuHardwareProbes.inspect(PORT, probe);

        assertEquals(SerialPortType.Ecu, result.type);
        assertEquals(3, probe.ecuReadCalls);
        assertEquals(2, probe.sleepCalls, "backoff runs between attempts only");
    }

    @Test
    public void unknownAfterMaxAttemptsWithBackoffBetweenAttemptsOnly() {
        FakePortProbe probe = new FakePortProbe();
        probe.ecuReadOutcomes.add(Optional.empty());
        probe.ecuReadOutcomes.add(Optional.empty());
        probe.ecuReadOutcomes.add(Optional.empty());

        PortResult result = EcuHardwareProbes.inspect(PORT, probe);

        assertEquals(new PortResult(PORT, SerialPortType.Unknown), result);
        assertEquals(3, probe.ecuReadCalls, "must give the ECU exactly DETECT_MAX_ATTEMPTS chances");
        assertEquals(2, probe.sleepCalls, "no pointless backoff after the last attempt");
        assertEquals(0, probe.hasOpenbltCalls);
    }

    @Test
    public void interruptDuringBackoffGivesUpAsUnknownAndStaysInterrupted() {
        FakePortProbe probe = new FakePortProbe();
        probe.ecuReadOutcomes.add(Optional.empty());
        probe.interruptOnSleep = true;

        PortResult result = EcuHardwareProbes.inspect(PORT, probe);

        assertEquals(SerialPortType.Unknown, result.type, "an interrupted scan reports what it knows");
        assertEquals(1, probe.ecuReadCalls, "interrupt must stop further attempts");
        assertTrue(Thread.interrupted(), "the interrupt flag must be restored for the scan loop");
    }
}
