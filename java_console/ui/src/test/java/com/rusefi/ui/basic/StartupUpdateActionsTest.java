package com.rusefi.ui.basic;

import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.PortScanner;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.ui.util.UiUtils;
import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import java.time.LocalDate;
import java.time.ZoneOffset;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicReference;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class StartupUpdateActionsTest {
    @Test
    void attachingLiveSplashConnectionRefreshesPreviouslyDisconnectedReleaseIdentity() throws Exception {
        PortResult port = new PortResult("ECU_DEV", SerialPortType.EcuWithOpenblt);
        AvailableHardware hardware = new AvailableHardware(List.of(port), false, false, false);
        PortScanner scanner = mock(PortScanner.class);
        when(scanner.getCurrentHardware()).thenReturn(hardware);
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        String today = LocalDate.now(ZoneOffset.UTC).toString();
        protocol.signature = "rusEFI master." + today.replace('-', '.') + ".uaefi_pro.build";
        when(linkManager.getBinaryProtocol()).thenReturn(protocol);
        when(linkManager.getLastTriedPort()).thenReturn(port.port);

        AtomicReference<StartupUpdateActions> actionsRef = new AtomicReference<>();
        AtomicReference<FirmwareStatusSummaryPanel> summaryRef = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
            AtomicReference<Optional<PortResult>> selected = new AtomicReference<>(Optional.of(port));
            StartupUpdateActions actions = new StartupUpdateActions(connectivity, status,
                new SingleAsyncJobExecutor(status), selected,
                new SoftwareUpdateCoordinator(
                    CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.UP_TO_DATE)),
                ignored -> { }, () -> { }, () -> null, () -> { });
            actions.setDevMetadataProviderForTests((board, branch) -> LocalDate.now(ZoneOffset.UTC)
                .atStartOfDay(ZoneOffset.UTC).toInstant().toEpochMilli());
            JComboBox<PortResult> ports = new JComboBox<>();
            ports.addItem(port);
            ports.setSelectedItem(port);
            ProgramSelector selector = new ProgramSelector(connectivity, ports, ignored -> { }, () -> { });
            actions.configureFirmwareSelector(selector);

            // Match startup ordering: scanner selection is known before the live LinkManager is handed off.
            actions.onHardwareUpdated();
            actions.setSplashLinkManager(linkManager);
            actionsRef.set(actions);
            summaryRef.set((FirmwareStatusSummaryPanel) actions.getFirmwareSummary());
        });

        String expected = "No newer build date found.";
        AtomicReference<String> actual = new AtomicReference<>();
        long deadline = System.nanoTime() + java.time.Duration.ofSeconds(5).toNanos();
        while (System.nanoTime() < deadline) {
            SwingUtilities.invokeAndWait(() -> actual.set(releaseStatus(summaryRef.get())));
            if (expected.equals(actual.get())) {
                break;
            }
            Thread.sleep(10);
        }
        assertEquals(expected, actual.get());
    }

    @Test
    void completedFailureEnablesManualRetry() throws Exception {
        JButton button = new JButton();
        button.setVisible(false);
        button.setEnabled(false);
        JLabel status = new JLabel();
        status.setVisible(false);
        JButton later = hiddenButton();
        StartupUpdateActions.applySoftwareUpdateOutcome(Autoupdate.UpdateOutcome.FAILED,
            button, status, later);

        assertTrue(button.isVisible());
        assertTrue(button.isEnabled());
        assertTrue(status.isVisible());
        assertFalse(later.isVisible());
        assertEquals("Update application", button.getText());
        assertEquals("Automatic software update check failed. You can retry.", status.getText());
    }

    @Test
    void availableUpdateOffersManualInstall() {
        JButton button = new JButton();
        button.setVisible(false);
        button.setEnabled(false);
        JLabel status = new JLabel();
        status.setVisible(false);
        JButton later = hiddenButton();
        StartupUpdateActions.applySoftwareUpdateOutcome(Autoupdate.UpdateOutcome.AVAILABLE,
            button, status, later);

        assertTrue(button.isVisible());
        assertTrue(button.isEnabled());
        assertTrue(status.isVisible());
        assertTrue(later.isVisible());
        assertEquals("Update now", button.getText());
        assertEquals("Application update available.", status.getText());
    }

    @Test
    void completedNoOpDoesNotOfferRetry() {
        JButton button = new JButton();
        button.setVisible(false);
        button.setEnabled(false);
        JLabel status = new JLabel();
        status.setVisible(false);
        JButton later = hiddenButton();
        StartupUpdateActions.applySoftwareUpdateOutcome(Autoupdate.UpdateOutcome.UP_TO_DATE,
            button, status, later);

        assertFalse(button.isVisible());
        assertFalse(button.isEnabled());
        assertFalse(status.isVisible());
        assertFalse(later.isVisible());
    }

    @Test
    void installedUpdateOffersRestartWithoutClosingApplication() {
        JButton button = hiddenButton();
        JLabel status = new JLabel();
        status.setVisible(false);
        JButton later = hiddenButton();
        StartupUpdateActions.applySoftwareRestartRequired(button, status, later);

        assertTrue(button.isVisible());
        assertEquals("Restart now", button.getText());
        assertTrue(status.isVisible());
        assertEquals("Update ready. Restart the application to finish updating.", status.getText());
        assertTrue(later.isVisible());
        assertEquals("Restart later", later.getText());
    }

    @Test
    void pendingUpdateOnLaunchRestoresRestartOffer() {
        JButton button = hiddenButton();
        JLabel status = new JLabel();
        status.setVisible(false);
        JButton later = hiddenButton();
        StartupUpdateActions.applySoftwareUpdateOutcome(Autoupdate.UpdateOutcome.UPDATED,
            button, status, later);

        assertTrue(button.isVisible());
        assertEquals("Restart now", button.getText());
        assertTrue(later.isVisible());
        assertEquals("Restart later", later.getText());
    }

    @Test
    void popupChoicesMapToUpdateActionsAndClosingMeansLater() {
        assertEquals(ApplicationUpdatePrompt.Choice.LATER, ApplicationUpdatePrompt.choice(0));
        assertEquals(ApplicationUpdatePrompt.Choice.UPDATE_NOW, ApplicationUpdatePrompt.choice(1));
        assertEquals(ApplicationUpdatePrompt.Choice.LATER,
            ApplicationUpdatePrompt.choice(JOptionPane.CLOSED_OPTION));
    }

    private static JButton hiddenButton() {
        JButton button = new JButton();
        button.setVisible(false);
        return button;
    }

    private static String releaseStatus(FirmwareStatusSummaryPanel summary) {
        return UiUtils.getAllComponents(summary).stream()
            .filter(component -> component instanceof JTextArea
                && !((JTextArea) component).getText().equals("Dev"))
            .map(component -> ((JTextArea) component).getText())
            .findFirst().orElseThrow(AssertionError::new);
    }
}
