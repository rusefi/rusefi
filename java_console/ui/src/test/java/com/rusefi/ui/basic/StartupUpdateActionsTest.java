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
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
import com.rusefi.ui.util.UiUtils;
import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import java.time.LocalDate;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.mockito.Mockito.any;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class StartupUpdateActionsTest {
    @Test
    void disconnectedNoIniProbeShowsDetectedIdentityAndPreservesDisabledTunePreference() throws Exception {
        String signature = "rusEFI noini-10540-20261119.2026.11.19.uaefi_pro.1254041395";
        PortResult port = new PortResult("ECU_NO_INI", SerialPortType.Ecu);
        PortResult otherPort = new PortResult("ECU_OTHER", SerialPortType.Ecu);
        AvailableHardware hardware = new AvailableHardware(List.of(port, otherPort), false, false, false);
        PortScanner scanner = mock(PortScanner.class);
        when(scanner.getCurrentHardware()).thenReturn(hardware);
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        connectivity.getMissingIniRecoveryState().record(port.port, signature);

        AtomicReference<FirmwareStatusSummaryPanel> summaryRef = new AtomicReference<>();
        AtomicReference<StartupUpdateActions> actionsRef = new AtomicReference<>();
        AtomicReference<Optional<PortResult>> selectedRef = new AtomicReference<>(Optional.of(port));
        AtomicReference<JComboBox<PortResult>> portsRef = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
            StartupUpdateActions actions = new StartupUpdateActions(connectivity, status,
                new SingleAsyncJobExecutor(status), selectedRef,
                new SoftwareUpdateCoordinator(CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.UP_TO_DATE)),
                ignored -> { }, () -> { }, () -> null, () -> { });
            actions.setDevMetadataProviderForTests((board, branch) -> {
                throw new AssertionError("unsupported branch must not request Dev release metadata");
            });
            JComboBox<PortResult> ports = new JComboBox<>();
            ports.addItem(port);
            ports.addItem(otherPort);
            ports.setSelectedItem(port);
            ProgramSelector selector = new ProgramSelector(connectivity, ports, ignored -> { }, () -> { });
            actions.configureFirmwareSelector(selector);
            actions.onHardwareUpdated();
            selector.apply(hardware);
            summaryRef.set((FirmwareStatusSummaryPanel) actions.getFirmwareSummary());
            actionsRef.set(actions);
            portsRef.set(ports);
        });

        SwingUtilities.invokeAndWait(() -> {
            List<java.awt.Component> components = UiUtils.getAllComponents(summaryRef.get());
            List<String> text = components.stream()
                .filter(component -> component instanceof JTextArea)
                .map(component -> ((JTextArea) component).getText())
                .collect(java.util.stream.Collectors.toList());
            assertTrue(text.stream().anyMatch(value -> value.contains("Detected uaefi_pro (ECU_NO_INI)")));
            assertTrue(text.stream().anyMatch(value -> value.contains("noini-10540-20261119")));
            assertTrue(text.stream().anyMatch(value -> value.contains("matching INI unavailable")));
            assertTrue(text.stream().anyMatch(value -> value.contains("defaults")));
            assertFalse(text.stream().anyMatch(value -> value.toLowerCase().contains("up to date")));
            List<String> labels = components.stream().filter(component -> component instanceof JLabel)
                .map(component -> ((JLabel) component).getText())
                .collect(java.util.stream.Collectors.toList());
            assertTrue(labels.contains("2026.11.19"));
            assertTrue(labels.contains("Not available for this firmware branch"));
            assertFalse(labels.contains("Unknown"));
            JCheckBox keepTune = components.stream().filter(component -> component instanceof JCheckBox)
                .map(component -> (JCheckBox) component).findFirst().orElseThrow(AssertionError::new);
            assertTrue(keepTune.isSelected());
            assertFalse(keepTune.isEnabled());

            // Change the actual program selection; ecuPortToUse is intentionally independent of this UI selection.
            portsRef.get().setSelectedItem(otherPort);
            summaryRef.get().refresh();
            assertFalse(summaryText(summaryRef.get()).contains("Detected uaefi_pro"));

            portsRef.get().setSelectedItem(port);
            summaryRef.get().refresh();
            assertTrue(summaryText(summaryRef.get()).contains("Detected uaefi_pro"));
            connectivity.getMissingIniRecoveryState().clearForPort(port.port);
            summaryRef.get().refresh();
            assertFalse(summaryText(summaryRef.get()).contains("matching INI unavailable"));

            BinaryProtocol recoveredProtocol = mock(BinaryProtocol.class);
            recoveredProtocol.signature = signature;
            LinkManager recoveredLink = mock(LinkManager.class);
            when(recoveredLink.getBinaryProtocol()).thenReturn(recoveredProtocol);
            when(recoveredLink.getLastTriedPort()).thenReturn(port.port);
            actionsRef.get().setSplashLinkManager(recoveredLink);
            List<java.awt.Component> recoveredComponents = UiUtils.getAllComponents(summaryRef.get());
            JCheckBox recoveredKeepTune = recoveredComponents.stream()
                .filter(component -> component instanceof JCheckBox).map(component -> (JCheckBox) component)
                .findFirst().orElseThrow(AssertionError::new);
            assertTrue(recoveredKeepTune.isEnabled());
            assertTrue(recoveredKeepTune.isSelected());
            assertFalse(summaryText(summaryRef.get()).contains("matching INI unavailable"));
            assertTrue(summaryText(summaryRef.get()).contains("uaefi_pro"));
        });
    }

    @Test
    void selectedOpenBltBootloaderShowsBootloaderOnlySummaryAndRecoversOnApplicationReconnect() throws Exception {
        PortResult bootloader = new PortResult("COM8", SerialPortType.OpenBlt, null,
            new OpenbltInfo(true, "rusefi.uaefi_pro", "1.16.0", List.of()));
        AvailableHardware hardware = new AvailableHardware(List.of(bootloader), false, false, false);
        PortScanner scanner = mock(PortScanner.class);
        AtomicReference<AvailableHardware> currentHardware = new AtomicReference<>(hardware);
        when(scanner.getCurrentHardware()).thenAnswer(ignored -> currentHardware.get());
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        AtomicReference<FirmwareStatusSummaryPanel> summaryRef = new AtomicReference<>();
        AtomicReference<StartupUpdateActions> actionsRef = new AtomicReference<>();
        AtomicReference<Optional<PortResult>> ecuPort = new AtomicReference<>(Optional.empty());
        AtomicReference<JComboBox<PortResult>> portsRef = new AtomicReference<>();
        AtomicReference<ProgramSelector> selectorRef = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
            StartupUpdateActions actions = new StartupUpdateActions(connectivity, status,
                new SingleAsyncJobExecutor(status), ecuPort,
                new SoftwareUpdateCoordinator(CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.UP_TO_DATE)),
                ignored -> { }, () -> { }, () -> null, () -> { });
            JComboBox<PortResult> ports = new JComboBox<>();
            ports.addItem(bootloader);
            ports.setSelectedItem(bootloader);
            ProgramSelector selector = new ProgramSelector(connectivity, ports, ignored -> { }, () -> { });
            actions.configureFirmwareSelector(selector);

            // Match startup's independent live-ECU provider: OpenBLT is selected for programming,
            // but is not a connected ECU port.
            actions.onHardwareUpdated();
            selector.apply(hardware);
            summaryRef.set((FirmwareStatusSummaryPanel) actions.getFirmwareSummary());
            actionsRef.set(actions);
            portsRef.set(ports);
            selectorRef.set(selector);
        });

        SwingUtilities.invokeAndWait(() -> {
            FirmwareStatusSummaryPanel summary = summaryRef.get();
            String rendered = summaryText(summary);
            assertTrue(rendered.contains("OpenBLT bootloader v1.16.0 — uaefi_pro (COM8)"), rendered);
            assertTrue(rendered.contains("Unavailable in bootloader mode"), rendered);
            assertTrue(rendered.contains("application branch and firmware dates are unavailable"), rendered);
            assertTrue(rendered.contains("A fresh tune backup is unavailable in bootloader mode"), rendered);
            assertTrue(rendered.contains("Manual updates may restore a tune backed up earlier in this session"), rendered);
            List<String> labels = UiUtils.getAllComponents(summary).stream()
                .filter(component -> component instanceof JLabel)
                .map(component -> ((JLabel) component).getText())
                .collect(java.util.stream.Collectors.toList());
            assertEquals(2, labels.stream().filter("Not available in bootloader mode"::equals).count(),
                "application firmware dates are unavailable in bootloader mode");
            JCheckBox keepTune = UiUtils.getAllComponents(summary).stream()
                .filter(component -> component instanceof JCheckBox).map(component -> (JCheckBox) component)
                .findFirst().orElseThrow(AssertionError::new);
            assertTrue(keepTune.isSelected());
            assertFalse(keepTune.isEnabled());
            assertTrue(keepTune.getToolTipText().contains("independently of this option"));
            assertEquals(Optional.empty(), ecuPort.get());
            assertEquals("COM8 (OpenBLT Bootloader v1.16.0: rusefi.uaefi_pro)", bootloader.toString());
            assertTrue(UiUtils.getAllComponents(selectorRef.get().getControl()).stream()
                .anyMatch(component -> component instanceof JButton
                    && "Manual OpenBLT Update".equals(((JButton) component).getText())),
                "the existing guarded primary action is explicitly labeled for OpenBLT");
        });

        SwingUtilities.invokeAndWait(() -> {
            BinaryProtocol liveEcuA = mock(BinaryProtocol.class);
            liveEcuA.signature = "rusEFI lts-26lima.2026.11.10.live_ecu_a.123";
            LinkManager liveLink = mock(LinkManager.class);
            when(liveLink.getBinaryProtocol()).thenReturn(liveEcuA);
            when(liveLink.getLastTriedPort()).thenReturn("ECU_A");
            actionsRef.get().setSplashLinkManager(liveLink);
            summaryRef.get().refresh();

            String rendered = summaryText(summaryRef.get());
            assertTrue(rendered.contains("OpenBLT bootloader v1.16.0 — uaefi_pro (COM8)"), rendered);
            assertFalse(rendered.contains("live_ecu_a"), rendered);
            JTextArea branch = UiUtils.getAllComponents(summaryRef.get()).stream()
                .filter(component -> component instanceof JTextArea
                    && "Unavailable in bootloader mode".equals(((JTextArea) component).getText()))
                .map(component -> (JTextArea) component).findFirst().orElseThrow(AssertionError::new);
            assertNull(branch.getToolTipText(), "a live ECU on another port must not leak its branch into BLT status");
        });

        SwingUtilities.invokeAndWait(() -> {
            PortResult replacement = new PortResult("COM8", SerialPortType.OpenBlt, null,
                new OpenbltInfo(true, "rusefi.replacement_board", "1.15.0", List.of()));
            AvailableHardware replacementHardware = new AvailableHardware(List.of(replacement), false, false, false);
            currentHardware.set(replacementHardware);
            JComboBox<PortResult> ports = portsRef.get();
            ports.removeAllItems();
            ports.addItem(replacement);
            ports.setSelectedItem(replacement);
            selectorRef.get().apply(replacementHardware);
            summaryRef.get().refresh();
            String rendered = summaryText(summaryRef.get());
            assertTrue(rendered.contains("OpenBLT bootloader v1.15.0 — replacement_board (COM8)"), rendered);
            assertFalse(rendered.contains("uaefi_pro"), rendered);
        });

        SwingUtilities.invokeAndWait(() -> {
            PortResult application = new PortResult("COM8", SerialPortType.EcuWithOpenblt);
            AvailableHardware applicationHardware =
                new AvailableHardware(List.of(application), false, false, false);
            currentHardware.set(applicationHardware);
            JComboBox<PortResult> ports = portsRef.get();
            ports.removeAllItems();
            ports.addItem(application);
            ports.setSelectedItem(application);
            selectorRef.get().apply(applicationHardware);

            BinaryProtocol protocol = mock(BinaryProtocol.class);
            protocol.signature = "rusEFI noini-10540-20261119.2026.11.19.uaefi_pro.1254041395";
            LinkManager linkManager = mock(LinkManager.class);
            when(linkManager.getBinaryProtocol()).thenReturn(protocol);
            when(linkManager.getLastTriedPort()).thenReturn(application.port);
            actionsRef.get().setSplashLinkManager(linkManager);
            actionsRef.get().onHardwareUpdated();
            summaryRef.get().refresh();

            String rendered = summaryText(summaryRef.get());
            assertTrue(rendered.contains("uaefi_pro (COM8)"), rendered);
            assertFalse(rendered.contains("OpenBLT bootloader"), rendered);
            assertFalse(rendered.contains("Unavailable in bootloader mode"), rendered);
            JCheckBox keepTune = UiUtils.getAllComponents(summaryRef.get()).stream()
                .filter(component -> component instanceof JCheckBox).map(component -> (JCheckBox) component)
                .findFirst().orElseThrow(AssertionError::new);
            assertTrue(keepTune.isSelected());
            assertTrue(keepTune.isEnabled(), "the tune option returns when application firmware reconnects");
        });
    }

    @Test
    void oldMasterNoIniIdentityComparesAgainstPublishedLatestMetadataWithoutLiveConnection() throws Exception {
        String signature = "rusEFI master.2024.01.01.uaefi_pro.1254041395";
        PortResult port = new PortResult("ECU_NO_INI", SerialPortType.Ecu);
        AvailableHardware hardware = new AvailableHardware(List.of(port), false, false, false);
        PortScanner scanner = mock(PortScanner.class);
        when(scanner.getCurrentHardware()).thenReturn(hardware);
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        AtomicReference<FirmwareStatusSummaryPanel> summaryRef = new AtomicReference<>();
        AtomicReference<String> metadataBoard = new AtomicReference<>();
        AtomicReference<String> metadataBranch = new AtomicReference<>();
        AtomicInteger metadataCalls = new AtomicInteger();
        AtomicReference<LinkManager.MissingIniListener> missingIniListener = new AtomicReference<>();
        LocalDate latestPublishedDate = LocalDate.now(ZoneOffset.UTC);
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
            AtomicReference<Optional<PortResult>> selected = new AtomicReference<>(Optional.of(port));
            StartupUpdateActions actions = new StartupUpdateActions(connectivity, status,
                new SingleAsyncJobExecutor(status), selected,
                new SoftwareUpdateCoordinator(CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.UP_TO_DATE)),
                ignored -> { }, () -> { }, () -> null, () -> { });
            actions.setDevMetadataProviderForTests((board, branch) -> {
                metadataCalls.incrementAndGet();
                metadataBoard.set(board);
                metadataBranch.set(branch);
                return latestPublishedDate.atStartOfDay(ZoneOffset.UTC).toInstant().toEpochMilli();
            });
            LinkManager linkManager = mock(LinkManager.class);
            doAnswer(invocation -> {
                missingIniListener.set(invocation.getArgument(0));
                return linkManager;
            }).when(linkManager).setMissingIniListener(any(LinkManager.MissingIniListener.class));
            JComboBox<PortResult> ports = new JComboBox<>();
            ports.addItem(port);
            ProgramSelector selector = new ProgramSelector(connectivity, ports, ignored -> { }, () -> { });
            actions.configureFirmwareSelector(selector);
            selector.setLinkManager(linkManager);
            actions.setSplashLinkManager(linkManager);
            actions.onHardwareUpdated();
            selector.apply(hardware);
            summaryRef.set((FirmwareStatusSummaryPanel) actions.getFirmwareSummary());
        });

        assertEquals(0, metadataCalls.get(), "no identity metadata is requested before missing-INI evidence arrives");
        SwingUtilities.invokeAndWait(() -> missingIniListener.get().onMissingIni(signature, port.port));
        String expectedLatest = DateTimeFormatter.ofPattern("yyyy.MM.dd").format(latestPublishedDate);
        AtomicReference<String> actualLatest = new AtomicReference<>();
        long deadline = System.nanoTime() + java.time.Duration.ofSeconds(5).toNanos();
        while (System.nanoTime() < deadline) {
            SwingUtilities.invokeAndWait(() -> actualLatest.set(UiUtils.getAllComponents(summaryRef.get()).stream()
                .filter(component -> component instanceof JLabel
                    && expectedLatest
                        .equals(((JLabel) component).getText()))
                .map(component -> ((JLabel) component).getText()).findFirst().orElse(null)));
            if (expectedLatest.equals(actualLatest.get())) {
                break;
            }
            Thread.sleep(10);
        }
        assertEquals(expectedLatest, actualLatest.get());
        assertEquals("uaefi_pro", metadataBoard.get());
        assertEquals("development", metadataBranch.get());
        assertEquals(1, metadataCalls.get(), "late evidence starts one lookup without another hardware scan");
        assertTrue(FirmwareStatusSummaryPanel.isUpdateAvailableReleaseStatus("Update available"));
        assertFalse(FirmwareStatusSummaryPanel.isUpdateAvailableReleaseStatus(
            "ECU detected; matching INI unavailable. Update available"));
        SwingUtilities.invokeAndWait(() -> {
            List<java.awt.Component> components = UiUtils.getAllComponents(summaryRef.get());
            assertTrue(components.stream().filter(component -> component instanceof JTextArea)
                .map(component -> ((JTextArea) component).getText())
                .anyMatch(value -> value.contains("Update available")
                    && value.contains("matching INI unavailable")));
            assertTrue(components.stream().filter(component -> component instanceof JLabel)
                .map(component -> ((JLabel) component).getText())
                .anyMatch("2024.01.01"::equals));
        });
    }

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

    private static String summaryText(FirmwareStatusSummaryPanel summary) {
        return UiUtils.getAllComponents(summary).stream()
            .filter(component -> component instanceof JTextArea)
            .map(component -> ((JTextArea) component).getText())
            .collect(java.util.stream.Collectors.joining("\n"));
    }
}
