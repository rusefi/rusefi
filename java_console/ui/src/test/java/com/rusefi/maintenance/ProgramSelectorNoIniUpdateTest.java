package com.rusefi.maintenance;

import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.FakePortScanner;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.OpenBltManualJob;
import com.rusefi.ui.basic.FirmwareUpdateCard;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;

import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.util.Collections;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

class ProgramSelectorNoIniUpdateTest {
    private static final String PORT = "COM_NO_INI";
    private static final String SIGNATURE = "rusEFI master.2026.08.30.test-board.1234567890";

    @Test
    void declineDoesNothingAndConfirmedRunningEcuRoutesToNoSettingsJob() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Scenario scenario = new Scenario(SerialPortType.Ecu,
                java.util.Arrays.asList(new PortResult(PORT, SerialPortType.Ecu),
                    new PortResult("COM_ALREADY_BLT", SerialPortType.OpenBlt)));
            assertNotNull(scenario.linkManager.getMissingIniFailure());
            assertEquals(PORT, scenario.linkManager.getMissingIniFailure().port);
            assertNotNull(com.rusefi.core.SignatureHelper.parse(SIGNATURE));
            ProgramSelector selector = scenario.selector();
            SingleAsyncJobExecutor executor = scenario.executor;
            JButton update = findButton(selector.getControl(), "Update firmware (no ini)");

            assertNotNull(update);
            assertTrue(update.isEnabled());
            update.doClick();
            JComponent confirmation = scenario.presentedPanel.get();
            assertNotNull(confirmation);
            assertNotNull(findButton(confirmation, "Update without preserving tune"));
            findButton(confirmation, "Cancel").doClick();
            verify(executor, never()).startJob(any(AsyncJob.class), any(Component.class));

            update.doClick();
            confirmation = scenario.presentedPanel.get();
            assertNotNull(confirmation);
            findButton(confirmation, "Update without preserving tune").doClick();
            ArgumentCaptor<AsyncJob> started = org.mockito.ArgumentCaptor.forClass(AsyncJob.class);
            verify(executor).startJob(started.capture(), any(Component.class));
            assertInstanceOf(OpenBltManualJob.class, started.getValue());
            assertEquals("Update firmware (no ini)", started.getValue().getName());
        });
    }

    @Test
    void unknownAndUnmatchedPortsCannotUseTheNoIniPath() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult unknown = new PortResult(PORT, SerialPortType.EcuUnknown);
            Scenario unknownScenario = new Scenario(SerialPortType.EcuUnknown,
                Collections.singletonList(unknown));
            ProgramSelector unknownSelector = unknownScenario.selector();
            JButton unknownButton = findButton(unknownSelector.getControl(), "Update firmware (no ini)");
            assertNull(unknownButton);
            verify(unknownScenario.executor, never()).startJob(any(AsyncJob.class), any(Component.class));

            PortResult other = new PortResult("COM_OTHER", SerialPortType.Ecu);
            Scenario unmatchedScenario = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(other));
            ProgramSelector unmatchedSelector = unmatchedScenario.selector();
            JButton unmatchedButton = findButton(unmatchedSelector.getControl(), "Update ECU firmware");
            assertNotNull(unmatchedButton);
            assertFalse(unmatchedButton.isEnabled());
            unmatchedButton.doClick();
            assertNull(unmatchedScenario.presentedPanel.get());
            verify(unmatchedScenario.executor, never()).startJob(any(AsyncJob.class), any(Component.class));
        });
    }

    @Test
    void noIniUpdateBypassesTheLtsInterceptorButStillStartsTheNoSettingsJob() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Scenario scenario = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(new PortResult(PORT, SerialPortType.Ecu)));
            ProgramSelector selector = scenario.selector();
            java.util.concurrent.atomic.AtomicInteger intercepted = new java.util.concurrent.atomic.AtomicInteger();
            selector.setTargetedFirmwareUpdateInterceptor((source, port) -> {
                intercepted.incrementAndGet();
                return true;
            });
            JButton update = findButton(selector.getControl(), "Update firmware (no ini)");
            assertNotNull(update);
            update.doClick();

            assertEquals(0, intercepted.get());
            assertNotNull(scenario.presentedPanel.get());
            verify(scenario.executor, never()).startJob(any(AsyncJob.class), any(Component.class));
            findButton(scenario.presentedPanel.get(), "Update without preserving tune").doClick();
            ArgumentCaptor<AsyncJob> started = org.mockito.ArgumentCaptor.forClass(AsyncJob.class);
            verify(scenario.executor).startJob(started.capture(), any(Component.class));
            assertEquals("Update firmware (no ini)", started.getValue().getName());
        });
    }

    @Test
    void noIniConfirmationIsAnInAppPanelWithAnExplicitDefaultsWarning() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Scenario scenario = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(new PortResult(PORT, SerialPortType.Ecu)));
            ProgramSelector selector = scenario.selector();

            JButton update = findButton(selector.getControl(), "Update firmware (no ini)");
            assertNotNull(update);
            update.doClick();

            JComponent panel = scenario.presentedPanel.get();
            assertNotNull(panel, "no-INI consent is presented inside the current firmware tab");
            assertTrue(panel.getPreferredSize().width >= 1000);
            assertTrue(panel.getPreferredSize().height >= 500);
            JTextArea warning = findTextArea(panel);
            assertNotNull(warning);
            assertTrue(warning.getText().contains("cannot back up or restore"));
            assertTrue(warning.getText().contains("firmware defaults"));
            assertTrue(findButton(panel, "Cancel").isEnabled());
            assertTrue(findButton(panel, "Update without preserving tune").isEnabled());
            verify(scenario.executor, never()).startJob(any(AsyncJob.class), any(Component.class));
        });
    }

    @Test
    void noIniConfirmationCancelsOnEvidenceOrTargetChangeAndRechecksBusyState() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Scenario removed = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(new PortResult(PORT, SerialPortType.Ecu)));
            ProgramSelector removedSelector = removed.selector();
            findButton(removedSelector.getControl(), "Update firmware (no ini)").doClick();
            JComponent removedPanel = removed.presentedPanel.get();
            AvailableHardware noHardware = new AvailableHardware(Collections.emptyList(), false, false, false);
            removed.scanner.fireHardwareChange(noHardware);
            removedSelector.apply(noHardware);
            assertEquals(1, removed.closedPanels.get());
            assertFalse(findButton(removedPanel, "Update without preserving tune").isEnabled());
            verify(removed.executor, never()).startJob(any(AsyncJob.class), any(Component.class));

            Scenario recovered = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(new PortResult(PORT, SerialPortType.Ecu)));
            ProgramSelector recoveredSelector = recovered.selector();
            findButton(recoveredSelector.getControl(), "Update firmware (no ini)").doClick();
            JComponent recoveredPanel = recovered.presentedPanel.get();
            recovered.context.getMissingIniRecoveryState().clearForPort(PORT);
            recoveredSelector.apply(recovered.hardware);
            assertEquals(1, recovered.closedPanels.get());
            assertFalse(findButton(recoveredPanel, "Update without preserving tune").isEnabled());
            verify(recovered.executor, never()).startJob(any(AsyncJob.class), any(Component.class));

            Scenario changedTarget = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(new PortResult(PORT, SerialPortType.Ecu)));
            ProgramSelector targetSelector = changedTarget.selector();
            findButton(targetSelector.getControl(), "Update firmware (no ini)").doClick();
            JComponent targetPanel = changedTarget.presentedPanel.get();
            changedTarget.ports.addItem(new PortResult("COM_OTHER", SerialPortType.Ecu));
            changedTarget.ports.setSelectedItem(new PortResult("COM_OTHER", SerialPortType.Ecu));
            assertEquals(1, changedTarget.closedPanels.get());
            assertFalse(findButton(targetPanel, "Update without preserving tune").isEnabled());
            verify(changedTarget.executor, never()).startJob(any(AsyncJob.class), any(Component.class));

            Scenario busy = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(new PortResult(PORT, SerialPortType.Ecu)));
            ProgramSelector busySelector = busy.selector();
            findButton(busySelector.getControl(), "Update firmware (no ini)").doClick();
            JComponent busyPanel = busy.presentedPanel.get();
            when(busy.executor.isNotInProgress()).thenReturn(false);
            findButton(busyPanel, "Update without preserving tune").doClick();
            assertEquals(1, busy.closedPanels.get());
            verify(busy.executor, never()).startJob(any(AsyncJob.class), any(Component.class));
        });
    }

    @Test
    void leavingFirmwareTabCancelsConfirmationAndReturningShowsNormalControls() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Scenario scenario = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(new PortResult(PORT, SerialPortType.Ecu)));
            ProgramSelector selector = scenario.selector();
            JTabbedPane tabs = new JTabbedPane();
            JPanel firmwareTab = new JPanel(new BorderLayout());
            JPanel firmwareContent = new JPanel(new BorderLayout());
            firmwareContent.add(selector.getControl(), BorderLayout.CENTER);
            JPanel views = FirmwareUpdateCard.createFirmwareTabContent(
                firmwareContent, selector, tabs, firmwareTab);
            firmwareTab.add(views, BorderLayout.CENTER);
            tabs.addTab("Firmware", firmwareTab);
            tabs.addTab("Connect", new JPanel());

            findButton(selector.getControl(), "Update firmware (no ini)").doClick();
            JPanel confirmationHost = (JPanel) views.getComponent(1);
            JComponent confirmation = (JComponent) confirmationHost.getComponent(0);
            assertTrue(confirmation.isVisible());
            JButton confirm = findButton(confirmation, "Update without preserving tune");
            assertTrue(confirm.isEnabled());

            tabs.setSelectedIndex(1);
            assertFalse(confirmation.isVisible());
            assertFalse(confirm.isEnabled(), "hidden consent actions remain invalidated");
            tabs.setSelectedIndex(0);
            assertTrue(firmwareContent.isVisible(), "returning restores the normal firmware controls");
            assertTrue(findButton(selector.getControl(), "Update firmware (no ini)").isEnabled());
            verify(scenario.executor, never()).startJob(any(AsyncJob.class), any(Component.class));
        });
    }

    @Test
    void displayableFirmwareTabCharacterizesConfirmationInvalidatedWhileAddingItsCard() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Scenario scenario = new Scenario(SerialPortType.Ecu,
                Collections.singletonList(new PortResult(PORT, SerialPortType.Ecu)));
            ProgramSelector selector = scenario.selector();
            JTabbedPane tabs = new JTabbedPane();
            JPanel firmwareTab = new JPanel(new BorderLayout());
            JPanel firmwareContent = new JPanel(new BorderLayout());
            firmwareContent.add(selector.getControl(), BorderLayout.CENTER);
            JPanel views = FirmwareUpdateCard.createFirmwareTabContent(
                firmwareContent, selector, tabs, firmwareTab);
            firmwareTab.add(views, BorderLayout.CENTER);
            tabs.addTab("Firmware", firmwareTab);
            tabs.addTab("Connect", new JPanel());
            JPanel root = new JPanel(new BorderLayout());
            root.add(tabs, BorderLayout.CENTER);

            root.addNotify();
            try {
                assertTrue(root.isDisplayable(), "the lightweight root creates a displayable Swing tree");
                assertTrue(views.isShowing(), "the firmware card host is visible before adding consent");

                findButton(selector.getControl(), "Update firmware (no ini)").doClick();
                JPanel confirmationHost = (JPanel) views.getComponent(1);
                JComponent confirmation = (JComponent) confirmationHost.getComponent(0);
                JButton cancel = findButton(confirmation, "Cancel");
                JButton confirm = findButton(confirmation, "Update without preserving tune");

                assertTrue(cancel.isEnabled(), "initial card insertion must not invalidate Cancel");
                assertTrue(confirm.isEnabled(), "initial card insertion must not invalidate confirmation");
                verify(scenario.executor, never()).startJob(any(AsyncJob.class), any(Component.class));
            } finally {
                root.removeNotify();
            }
        });
    }

    @Test
    void normalUpdateShortcutDoesNotTurnDfuRecoveryIntoANoIniUpdate() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU_DEVICE", SerialPortType.Dfu);
            FakePortScanner scanner = new FakePortScanner();
            ConnectivityContext context = new ConnectivityContext(
                scanner, ConnectedEcuTarget.forManualRecovery("test-board"));
            AvailableHardware hardware = new AvailableHardware(
                Collections.singletonList(dfu), true, false, false);
            scanner.fireHardwareChange(hardware);
            JComboBox<PortResult> ports = new JComboBox<>(new PortResult[]{dfu});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            ProgramSelector selector = new ProgramSelector(context, ports, ignored -> {}, () -> {});
            selector.setLinkManager(new LinkManager());
            selector.setJobExecutor(executor);
            selector.apply(hardware, true, false, "test-board", "test-board");
            java.util.concurrent.atomic.AtomicInteger intercepted = new java.util.concurrent.atomic.AtomicInteger();
            selector.setTargetedFirmwareUpdateInterceptor((source, port) -> {
                intercepted.incrementAndGet();
                return true;
            });

            JButton update = findButton(selector.getControl(), "Recover ECU");
            assertNotNull(update);
            // Current master keeps confirmed DFU recovery separate from normal updates.
            selector.triggerUpdateFirmware();

            assertEquals(0, intercepted.get());
            verify(executor, never()).startJob(any(AsyncJob.class), any(Component.class));
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

    private static final class Scenario {
        final FakePortScanner scanner = new FakePortScanner();
        final ConnectivityContext context = new ConnectivityContext(scanner, ConnectedEcuTarget.forManualRecovery("test-board"));
        final LinkManager linkManager = new LinkManager();
        final SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
        final AvailableHardware hardware;
        final JComboBox<PortResult> ports;
        final AtomicReference<JComponent> presentedPanel = new AtomicReference<>();
        final AtomicInteger closedPanels = new AtomicInteger();

        Scenario(SerialPortType selectedType, java.util.List<PortResult> knownPorts) {
            hardware = new AvailableHardware(knownPorts, false, false, false);
            scanner.fireHardwareChange(hardware);
            ports = new JComboBox<>(new PortResult[]{new PortResult(
                knownPorts.get(0).port, selectedType)});
            linkManager.start(PORT, ConnectionStatusLogic.Listener.VOID);
            linkManager.recordMissingIniFailure(SIGNATURE);
            when(executor.isNotInProgress()).thenReturn(true);
        }

        ProgramSelector selector() {
            ProgramSelector selector = new ProgramSelector(context, ports, presentedPanel::set, () -> {});
            selector.setNoIniUpdatePanelHost(presentedPanel::set, () -> {
                closedPanels.incrementAndGet();
                if (presentedPanel.get() != null) {
                    presentedPanel.get().setVisible(false);
                }
            });
            selector.setLinkManager(linkManager);
            selector.setJobExecutor(executor);
            selector.apply(hardware, true, false, "test-board", "test-board");
            return selector;
        }
    }

    private static JTextArea findTextArea(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JTextArea) {
                return (JTextArea) component;
            }
            if (component instanceof Container) {
                JTextArea found = findTextArea((Container) component);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
