package com.rusefi;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.AvailableHardware;
import com.rusefi.FakePortScanner;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.mockito.Mockito.when;

/** End-to-end classification boundary for a running ECU whose calibration probe cannot load its INI. */
class NoIniProbeToSelectorTest {
    private static final String PORT = "COM_PROBE_NO_INI";
    private static final String SIGNATURE = "rusEFI master.2026.08.30.test-board.1234567890";

    @Test
    void scannerProbeCarriesMissingIniIdentityIntoSelectorWithoutManualLinkManagerState() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            EcuHardwareProbes.PortProbe probe = new FailedCalibrationProbe();
            MissingIniRecoveryState recovery = new MissingIniRecoveryState();
            PortResult inspected = EcuHardwareProbes.inspect(PORT, probe, recovery);
            assertEquals(SerialPortType.Ecu, inspected.type);
            assertEquals(PORT, recovery.get().port);
            assertEquals(SIGNATURE, recovery.get().signature);

            FakePortScanner scanner = new FakePortScanner();
            ConnectivityContext context = new ConnectivityContext(
                scanner, ConnectedEcuTarget.forManualRecovery("test-board"), recovery);
            AvailableHardware hardware = new AvailableHardware(
                Collections.singletonList(inspected), false, false, false);
            scanner.fireHardwareChange(hardware);
            JComboBox<PortResult> ports = new JComboBox<>(new PortResult[]{inspected});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            ProgramSelector selector = new ProgramSelector(context, ports, ignored -> {}, () -> {});
            selector.setLinkManager(new LinkManager());
            selector.setJobExecutor(executor);
            selector.apply(hardware);

            assertNotNull(findButton(selector.getControl(), "Update firmware (no ini)"));
            verify(executor, never()).startJob(any(AsyncJob.class), any(Component.class));
        });
    }

    @Test
    void aFreshProbeWithoutTheMissingIniFailureClearsStalePortPairing() {
        MissingIniRecoveryState recovery = new MissingIniRecoveryState();
        recovery.record(PORT, SIGNATURE);

        PortResult result = EcuHardwareProbes.inspect(PORT, new NoEcuProbe(), recovery);

        assertEquals(SerialPortType.Unknown, result.type);
        assertEquals(null, recovery.get());
    }

    @Test
    void successfulProbeClearsNoIniEligibilityDespiteStaleLinkManagerFailure() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            FakePortScanner scanner = new FakePortScanner();
            ConnectivityContext context = new ConnectivityContext(
                scanner, ConnectedEcuTarget.forManualRecovery("test-board"));
            PortResult ecu = new PortResult(PORT, SerialPortType.Ecu);
            AvailableHardware hardware = new AvailableHardware(
                Collections.singletonList(ecu), false, false, false);
            scanner.fireHardwareChange(hardware);
            JComboBox<PortResult> ports = new JComboBox<>(new PortResult[]{ecu});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            ProgramSelector selector = new ProgramSelector(context, ports, ignored -> {}, () -> {});
            LinkManager linkManager = new LinkManager();
            linkManager.start(PORT, com.rusefi.io.ConnectionStatusLogic.Listener.VOID);
            selector.setLinkManager(linkManager);
            selector.setJobExecutor(executor);
            linkManager.recordMissingIniFailure(SIGNATURE);
            selector.apply(hardware);

            assertNotNull(findButton(selector.getControl(), "Update firmware (no ini)"));
            assertEquals(SerialPortType.Ecu,
                EcuHardwareProbes.inspect(PORT, new SuccessfulEcuProbe(), context.getMissingIniRecoveryState()).type);
            assertNull(context.getMissingIniRecoveryState().get());
            selector.apply(hardware);

            assertNull(findButton(selector.getControl(), "Update firmware (no ini)"),
                "fresh shared discovery evidence is authoritative over the stale LinkManager field");
        });
    }

    private static JButton findButton(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && text.equals(((JButton) component).getText())) {
                return (JButton) component;
            }
            if (component instanceof Container) {
                JButton found = findButton((Container) component, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static final class SuccessfulEcuProbe implements EcuHardwareProbes.PortProbe {
        @Override
        public OpenbltInfo getOpenbltInfo(String port) {
            return new OpenbltInfo(false, null);
        }

        @Override
        public PortResult inspectRunningEcu(String port) {
            return new PortResult(port, SerialPortType.Ecu);
        }

        @Override
        public boolean ecuHasOpenblt(String port) {
            return false;
        }

        @Override
        public void sleepBetweenAttempts() {
        }
    }

    private static final class FailedCalibrationProbe implements EcuHardwareProbes.PortProbe {
        @Override
        public OpenbltInfo getOpenbltInfo(String port) {
            return new OpenbltInfo(false, null);
        }

        @Override
        public PortResult inspectRunningEcu(String port) {
            return null;
        }

        @Override
        public PortResult inspectRunningEcu(String port, LinkManager.MissingIniListener missingIniListener) {
            missingIniListener.onMissingIni(SIGNATURE, port);
            return null;
        }

        @Override
        public boolean ecuHasOpenblt(String port) {
            return false;
        }

        @Override
        public void sleepBetweenAttempts() {
        }
    }

    private static final class NoEcuProbe implements EcuHardwareProbes.PortProbe {
        @Override
        public OpenbltInfo getOpenbltInfo(String port) {
            return new OpenbltInfo(false, null);
        }

        @Override
        public PortResult inspectRunningEcu(String port) {
            return null;
        }

        @Override
        public boolean ecuHasOpenblt(String port) {
            return false;
        }

        @Override
        public void sleepBetweenAttempts() {
        }
    }
}
