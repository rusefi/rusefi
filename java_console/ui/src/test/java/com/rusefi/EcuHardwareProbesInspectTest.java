package com.rusefi;

import com.fazecast.jSerialComm.SerialPortInvalidPortException;
import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.core.io.UnsupportedEcuInfo;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
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
import static org.mockito.Mockito.verify;
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
        /** one entry per ECU inspection: Optional calibrations, an explicit PortResult, or an exception */
        final Deque<Object> ecuReadOutcomes = new ArrayDeque<>();

        int ecuReadCalls;
        int hasOpenbltCalls;
        int sleepCalls;

        @Override
        public OpenbltInfo getOpenbltInfo(String port) {
            if (openbltPortThrowsStaleNode) {
                throw new SerialPortInvalidPortException("stale OS node", null);
            }
            return new OpenbltInfo(openbltPort, null);
        }

        @SuppressWarnings("unchecked")
        @Override
        public PortResult inspectRunningEcu(String port) {
            ecuReadCalls++;
            Object outcome = ecuReadOutcomes.removeFirst();
            if (outcome instanceof RuntimeException) {
                throw (RuntimeException) outcome;
            }
            if (outcome instanceof PortResult) {
                return (PortResult) outcome;
            }
            return ((Optional<CalibrationsInfo>) outcome)
                .map(c -> new PortResult(port, SerialPortType.Ecu, c))
                .orElse(null);
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

    private static class FakeSocketCanProbe implements EcuHardwareProbes.SocketCanProbe {
        boolean supported = true;
        RuntimeException openFailure;
        String signature;
        final IoStream stream = mock(IoStream.class);
        int openCalls;

        @Override
        public boolean isSupported() {
            return supported;
        }

        @Override
        public IoStream open() {
            openCalls++;
            if (openFailure != null) {
                throw openFailure;
            }
            return stream;
        }

        @Override
        public String readSignature(IoStream stream) {
            return signature;
        }
    }

    @AfterEach
    public void clearInterruptFlag() {
        // the interrupt-during-backoff path re-interrupts the thread; never leak that into other tests
        Thread.interrupted();
    }

    @Test
    public void unsupportedPlatformDoesNotOpenSocketCan() {
        FakeSocketCanProbe probe = new FakeSocketCanProbe();
        probe.supported = false;

        assertNull(EcuHardwareProbes.inspectSocketCan(probe));
        assertEquals(0, probe.openCalls);
    }

    @Test
    public void missingSocketCanInterfaceIsUnavailable() {
        FakeSocketCanProbe probe = new FakeSocketCanProbe();
        probe.openFailure = new IllegalStateException("no can0");

        assertNull(EcuHardwareProbes.inspectSocketCan(probe));
        assertEquals(1, probe.openCalls);
    }

    @Test
    public void socketCanWithoutEcuReplyReportsTheInterfaceOnly() {
        FakeSocketCanProbe probe = new FakeSocketCanProbe();

        PortResult result = EcuHardwareProbes.inspectSocketCan(probe);

        assertEquals(new PortResult(LinkManager.SOCKET_CAN, SerialPortType.CAN), result);
        verify(probe.stream).close();
    }

    @Test
    public void socketCanWithValidSignatureReportsAnEcu() {
        FakeSocketCanProbe probe = new FakeSocketCanProbe();
        probe.signature = "rusEFI master.2026.09.02.test.123456";

        PortResult result = EcuHardwareProbes.inspectSocketCan(probe);

        assertEquals(new PortResult(LinkManager.SOCKET_CAN, SerialPortType.Ecu), result);
        verify(probe.stream).close();
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
    public void unsupportedEcuIsReturnedWithoutRetryOrBootloaderProbe() {
        FakePortProbe probe = new FakePortProbe();
        UnsupportedEcuInfo info = new UnsupportedEcuInfo("hellen121nissan", "universal");
        probe.ecuReadOutcomes.add(PortResult.unsupportedEcu(PORT, info));

        PortResult result = EcuHardwareProbes.inspect(PORT, probe);

        assertEquals(SerialPortType.UnsupportedEcu, result.type);
        assertSame(info, result.getUnsupportedEcuInfo());
        assertEquals(1, probe.ecuReadCalls);
        assertEquals(0, probe.sleepCalls);
        assertEquals(0, probe.hasOpenbltCalls);
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
