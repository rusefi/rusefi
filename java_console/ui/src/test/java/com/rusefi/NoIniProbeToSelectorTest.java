package com.rusefi;

import com.opensr5.ini.IniLocator;
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
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
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
    void fastAutomaticIniHitCancelsTheDelayedIndicatorWithoutShowingIt() throws Exception {
        AtomicReference<StartupFrame.IniLookupProgressDisplay> display = new AtomicReference<>();
        AtomicReference<JPanel> area = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            JPanel progressArea = new JPanel();
            JLabel label = new JLabel();
            progressArea.setVisible(false);
            area.set(progressArea);
            display.set(new StartupFrame.IniLookupProgressDisplay(progressArea, label, () -> {
                throw new AssertionError("a fast lookup must not reveal startup controls");
            }));

            display.get().accept(IniLocator.LookupProgress.stage(IniLocator.LookupStage.BUNDLED_INI));
            display.get().accept(IniLocator.LookupProgress.stage(IniLocator.LookupStage.COMPLETE));

            assertFalse(progressArea.isVisible());
            assertFalse(display.get().isTimerRunningForTests());
        });
        assertFalse(area.get().isVisible());
    }

    @Test
    void delayedIndicatorShowsLatestStageAboveTabsWithoutChangingSelection() throws Exception {
        CountDownLatch shown = new CountDownLatch(1);
        AtomicReference<StartupFrame.IniLookupProgressDisplay> display = new AtomicReference<>();
        AtomicReference<JPanel> area = new AtomicReference<>();
        AtomicReference<JLabel> label = new AtomicReference<>();
        AtomicReference<JTabbedPane> tabs = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            JPanel progressArea = new JPanel();
            JLabel progressLabel = new JLabel();
            JTabbedPane outerTabs = new JTabbedPane();
            outerTabs.addTab("Update ECU firmware", new JPanel());
            outerTabs.addTab("Connect", new JPanel());
            outerTabs.setSelectedIndex(1);
            JPanel startup = new JPanel(new java.awt.BorderLayout());
            startup.add(progressArea, java.awt.BorderLayout.NORTH);
            startup.add(outerTabs, java.awt.BorderLayout.CENTER);
            progressArea.setVisible(false);
            area.set(progressArea);
            label.set(progressLabel);
            tabs.set(outerTabs);
            display.set(new StartupFrame.IniLookupProgressDisplay(
                progressArea, progressLabel, shown::countDown));

            display.get().accept(IniLocator.LookupProgress.stage(IniLocator.LookupStage.BUNDLED_INI));
            display.get().accept(new IniLocator.LookupProgress(
                IniLocator.LookupStage.DRIVE_ROOTS, 5, 12, 2, 2));
            assertFalse(progressArea.isVisible(), "the indicator waits for its display delay");
            assertEquals(1, outerTabs.getSelectedIndex());
        });

        assertTrue(shown.await(2, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {
            assertTrue(area.get().isVisible());
            assertEquals("Searching drive roots… — 5 of 12 completed (2 active)", label.get().getText());
            assertEquals(1, tabs.get().getSelectedIndex());
            display.get().accept(IniLocator.LookupProgress.stage(IniLocator.LookupStage.COMPLETE));
            assertFalse(area.get().isVisible(), "terminal completion hides the shared indicator");
        });
    }

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
    void historicalMissingIniFailureDoesNotExtendTheScannerDeadline() {
        MissingIniRecoveryState recovery = new MissingIniRecoveryState();
        recovery.record(PORT, SIGNATURE);

        assertFalse(recovery.shouldWaitForResolution(PORT),
            "only an actively running INI resolution qualifies for the extended scanner wait");
        recovery.resolutionStarted(PORT);
        assertTrue(recovery.shouldWaitForResolution(PORT));
        recovery.resolutionFinished(PORT);
        assertFalse(recovery.shouldWaitForResolution(PORT));
    }

    @Test
    void identifiedEcuIniResolutionIsVisibleToTheScannerUntilItFinishes() throws Exception {
        MissingIniRecoveryState recovery = new MissingIniRecoveryState();
        CountDownLatch resolutionStarted = new CountDownLatch(1);
        CountDownLatch releaseResolution = new CountDownLatch(1);
        EcuHardwareProbes.PortProbe probe = new SuccessfulEcuProbe() {
            @Override
            public PortResult inspectRunningEcu(String port, LinkManager.MissingIniListener listener) {
                listener.onIniResolutionStarted(port);
                resolutionStarted.countDown();
                try {
                    releaseResolution.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                return new PortResult(port, SerialPortType.Ecu);
            }
        };
        FutureTask<PortResult> inspection = new FutureTask<>(
            () -> EcuHardwareProbes.inspect(PORT, probe, recovery));
        Thread worker = new Thread(inspection, "test ECU INI resolution");
        worker.setDaemon(true);
        worker.start();

        try {
            assertTrue(resolutionStarted.await(1, TimeUnit.SECONDS));
            assertTrue(recovery.shouldWaitForResolution(PORT),
                "only this identified ECU should qualify for the scanner's extended wait");
            assertFalse(recovery.shouldWaitForResolution("UNKNOWN_SIBLING"));
            releaseResolution.countDown();
            assertEquals(SerialPortType.Ecu, inspection.get(1, TimeUnit.SECONDS).type);
            assertFalse(recovery.shouldWaitForResolution(PORT));
        } finally {
            releaseResolution.countDown();
        }
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

    private static class SuccessfulEcuProbe implements EcuHardwareProbes.PortProbe {
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
