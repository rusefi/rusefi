package com.rusefi;

import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;
import com.rusefi.ui.basic.FirmwareStatusSummaryPanel;
import com.rusefi.ui.basic.FirmwareUpdateCard;
import com.rusefi.ui.basic.SoftwareUpdateCoordinator;
import com.rusefi.ui.basic.StartupUpdateActions;
import com.rusefi.autoupdate.FirmwareRollbackResolver;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.JSplitButton;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Rectangle;
import java.awt.event.ComponentEvent;
import java.time.LocalDate;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;
import java.util.Collections;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.IdentityHashMap;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.when;

import static org.junit.jupiter.api.Assertions.*;

class DevicePaneLayoutTest {
    @Test
    void startupFirmwareTopPanelHidesEmptySecondarySections() throws Exception {
        String previousRollbackFlag = System.getProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "false");
        try {
            AtomicReference<JPanel> panelRef = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> {
                FakePortScanner scanner = new FakePortScanner();
                ConnectivityContext connectivity = new ConnectivityContext(scanner);
                StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
                StartupUpdateActions actions = new StartupUpdateActions(connectivity, status,
                    new SingleAsyncJobExecutor(status), new AtomicReference<>(Optional.empty()),
                    new SoftwareUpdateCoordinator(new CompletableFuture<>()),
                    ignored -> { }, () -> { }, () -> null, () -> { });
                ProgramSelector selector = new ProgramSelector(connectivity, new JComboBox<>(), ignored -> { }, () -> { });
                actions.configureFirmwareSelector(selector);
                panelRef.set(StartupFrame.createFirmwareTopPanel(actions, selector));
            });
            SwingUtilities.invokeAndWait(() -> {
                List<Component> components = UiUtils.getAllComponents(panelRef.get());
                assertFalse(components.stream().anyMatch(component -> component instanceof JLabel
                    && "Tuner application:".equals(((JLabel) component).getText())));
                JLabel firmwareOptions = (JLabel) components.stream().filter(component -> component instanceof JLabel
                    && "Firmware options:".equals(((JLabel) component).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                JButton updateApplication = (JButton) components.stream().filter(component -> component instanceof JButton
                    && "Update application".equals(((JButton) component).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                assertFalse(updateApplication.isVisible());
                assertFalse(firmwareOptions.isVisible());
                assertFalse(firmwareOptions.getParent().isVisible());
            });
        } finally {
            if (previousRollbackFlag == null) {
                System.clearProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
            } else {
                System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", previousRollbackFlag);
            }
        }
    }

    @Test
    void startupApplicationUpdateRowKeepsApplicationAndFirmwareStatusesDistinct() throws Exception {
        String previousRollbackFlag = System.getProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "false");
        try {
            AtomicReference<JPanel> panelRef = new AtomicReference<>();
            AtomicReference<StartupUpdateActions> actionsRef = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> {
                FakePortScanner scanner = new FakePortScanner();
                ConnectivityContext connectivity = new ConnectivityContext(scanner);
                StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
                StartupUpdateActions actions = new StartupUpdateActions(connectivity, status,
                    new SingleAsyncJobExecutor(status), new AtomicReference<>(Optional.empty()),
                    new SoftwareUpdateCoordinator(new CompletableFuture<>()),
                    ignored -> { }, () -> { }, () -> null, () -> { });
                ProgramSelector selector = new ProgramSelector(connectivity, new JComboBox<>(), ignored -> { }, () -> { });
                actions.configureFirmwareSelector(selector);
                panelRef.set(StartupFrame.createFirmwareTopPanel(actions, selector));
                actionsRef.set(actions);

                actions.getUpdateSoftwareStatus().setText("Application update available.");
                actions.getUpdateSoftwareStatus().setVisible(true);
                actions.getUpdateSoftwareStatus().dispatchEvent(
                    new ComponentEvent(actions.getUpdateSoftwareStatus(), ComponentEvent.COMPONENT_SHOWN));
                actions.getUpdateSoftwareButton().setText("Update now");
                actions.getUpdateSoftwareButton().setVisible(true);
                actions.getUpdateSoftwareButton().dispatchEvent(
                    new ComponentEvent(actions.getUpdateSoftwareButton(), ComponentEvent.COMPONENT_SHOWN));
                actions.getRollbackFirmwareButton().setVisible(true);
                actions.getRollbackFirmwareButton().dispatchEvent(
                    new ComponentEvent(actions.getRollbackFirmwareButton(), ComponentEvent.COMPONENT_SHOWN));
            });
            SwingUtilities.invokeAndWait(() -> {
                List<Component> components = UiUtils.getAllComponents(panelRef.get());
                JLabel applicationStatus = actionsRef.get().getUpdateSoftwareStatus();
                JButton updateNow = actionsRef.get().getUpdateSoftwareButton();
                assertTrue(applicationStatus.isVisible());
                assertTrue(applicationStatus.getParent().isVisible(),
                    "showing application status reveals its application-update row");
                assertTrue(updateNow.isVisible());
                assertTrue(updateNow.getParent().getParent().isVisible(),
                    "showing application action reveals its row ancestor");
                assertEquals("Application update available.", applicationStatus.getText());
                assertEquals("Update now", updateNow.getText());
                assertFalse(applicationStatus.getText().contains("firmware"));
                JTextArea firmwareStatus = (JTextArea) components.stream()
                    .filter(component -> component instanceof JTextArea)
                    .map(component -> (JTextArea) component)
                    .filter(area -> area.getText().startsWith("Disconnected"))
                    .findFirst().orElseThrow(AssertionError::new);
                assertFalse(firmwareStatus.getText().contains("Application update"));
                assertFalse(components.stream().anyMatch(component -> component instanceof JLabel
                    && "Tuner application:".equals(((JLabel) component).getText())));
                JLabel firmwareOptions = (JLabel) components.stream().filter(component -> component instanceof JLabel
                    && "Firmware options:".equals(((JLabel) component).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                assertTrue(firmwareOptions.isVisible());
                assertTrue(firmwareOptions.getParent().isVisible(),
                    "showing rollback reveals the firmware-options section");
            });
        } finally {
            if (previousRollbackFlag == null) {
                System.clearProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
            } else {
                System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", previousRollbackFlag);
            }
        }
    }

    @Test
    void disconnectedDevicePaneComparesOldMasterNoIniSignatureWithPublishedLatestDate() throws Exception {
        String signature = "rusEFI master.2024.01.01.uaefi_pro.1254041395";
        PortResult port = new PortResult("ECU_NO_INI", SerialPortType.Ecu);
        AvailableHardware hardware = new AvailableHardware(Collections.singletonList(port), false, false, false);
        LocalDate latestPublishedDate = LocalDate.now(ZoneOffset.UTC);
        DevicePane[] pane = new DevicePane[1];
        AtomicReference<String> metadataBoard = new AtomicReference<>();
        AtomicReference<String> metadataBranch = new AtomicReference<>();
        AtomicReference<LinkManager.MissingIniListener> missingIniListener = new AtomicReference<>();
        AtomicInteger metadataCalls = new AtomicInteger();
        SwingUtilities.invokeAndWait(() -> {
            FakePortScanner scanner = new FakePortScanner();
            ConnectivityContext connectivity = new ConnectivityContext(scanner);
            scanner.fireHardwareChange(hardware);
            LinkManager linkManager = mock(LinkManager.class);
            when(linkManager.getBinaryProtocol()).thenReturn(null);
            when(linkManager.getLastTriedPort()).thenReturn(null);
            doAnswer(invocation -> {
                missingIniListener.set(invocation.getArgument(0));
                return linkManager;
            }).when(linkManager).setMissingIniListener(any(LinkManager.MissingIniListener.class));
            UIContext context = new UIContext() {
                @Override
                public LinkManager getLinkManager() { return linkManager; }
            };
            StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
            DeviceSessionManager sessions = new DeviceSessionManager(connectivity, port, () -> { }, () -> {}) {
                @Override
                public SessionState getState() { return SessionState.DISCONNECTED; }
            };
            pane[0] = new DevicePane(context, connectivity, sessions, new JTabbedPane(),
                new SingleAsyncJobExecutor(status), status, ignored -> { }, () -> { });
            pane[0].setDevMetadataProviderForTests((board, branch) -> {
                metadataCalls.incrementAndGet();
                metadataBoard.set(board);
                metadataBranch.set(branch);
                return latestPublishedDate.atStartOfDay(ZoneOffset.UTC).toInstant().toEpochMilli();
            });
            pane[0].render(SessionState.DISCONNECTED, hardware);
        });
        assertEquals(0, metadataCalls.get(), "discovery has no signature to inspect before evidence arrives");
        SwingUtilities.invokeAndWait(() ->
            missingIniListener.get().onMissingIni(signature, port.port));

        AtomicReference<String> releaseStatus = new AtomicReference<>();
        AtomicReference<String> installedDate = new AtomicReference<>();
        AtomicReference<String> latestDate = new AtomicReference<>();
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        while (System.nanoTime() < deadline && releaseStatus.get() == null) {
            SwingUtilities.invokeAndWait(() -> {
                List<Component> components = UiUtils.getAllComponents((JComponent) pane[0].getContent());
                releaseStatus.set(components.stream().filter(component -> component instanceof JTextArea)
                    .map(component -> ((JTextArea) component).getText())
                    .filter(value -> value.contains("Update available"))
                    .findFirst().orElse(null));
                installedDate.set(components.stream().filter(component -> component instanceof JLabel)
                    .map(component -> ((JLabel) component).getText())
                    .filter("2024.01.01"::equals).findFirst().orElse(null));
                latestDate.set(components.stream().filter(component -> component instanceof JLabel)
                    .map(component -> ((JLabel) component).getText())
                    .filter(DateTimeFormatter.ofPattern("yyyy.MM.dd").format(latestPublishedDate)::equals)
                    .findFirst().orElse(null));
            });
            if (releaseStatus.get() == null) {
                Thread.sleep(10);
            }
        }
        assertNotNull(releaseStatus.get(), "metadata discovery should finish while DevicePane is disconnected");
        assertTrue(releaseStatus.get().contains("Update available"));
        assertTrue(releaseStatus.get().contains("matching INI unavailable"));
        assertEquals("2024.01.01", installedDate.get());
        assertEquals(DateTimeFormatter.ofPattern("yyyy.MM.dd").format(latestPublishedDate), latestDate.get());
        assertEquals("uaefi_pro", metadataBoard.get());
        assertEquals("development", metadataBranch.get());
        assertEquals(1, metadataCalls.get(), "late evidence starts one lookup without another hardware scan");
    }

    @Test
    void eligibleLtsActionsFitInsideNarrowDeviceCardAtHighFontSize() throws Throwable {
        String previousRollbackFlag = System.getProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "true");
        try {
            String latestSha = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
            String installedSha = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
            String previousSha = "cccccccccccccccccccccccccccccccccccccccc";
            LinkManager linkManager = mock(LinkManager.class);
            BinaryProtocol protocol = mock(BinaryProtocol.class);
            protocol.signature = "rusEFI lts-26lima.2026.11.11.uaefi_pro." + installedSha;
            when(linkManager.getBinaryProtocol()).thenReturn(protocol);
            when(linkManager.getLastTriedPort()).thenReturn("ECU_LTS");
            UIContext context = new UIContext() {
                @Override
                public LinkManager getLinkManager() { return linkManager; }
            };
            PortResult port = spy(new PortResult("ECU_LTS", SerialPortType.EcuWithOpenblt));
            doReturn(Optional.of(installedSha)).when(port).getFirmwareHash();
            DevicePane[] pane = new DevicePane[1];
            SwingUtilities.invokeAndWait(() -> {
                FakePortScanner scanner = new FakePortScanner();
                ConnectivityContext connectivity = new ConnectivityContext(scanner);
                scanner.fireHardwareChange(new AvailableHardware(Collections.singletonList(port), false, false, false));
                StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
                DeviceSessionManager sessions = new DeviceSessionManager(connectivity, port, () -> { }, () -> {}) {
                    @Override
                    public SessionState getState() { return SessionState.CONNECTED; }
                };
                pane[0] = new DevicePane(context, connectivity, sessions, new JTabbedPane(),
                    new SingleAsyncJobExecutor(status), status, ignored -> { }, () -> { });
                pane[0].setLtsCatalogProviderForTests((board, branch) -> FirmwareRollbackResolver.parseIndex(
                    "<a href=\"" + latestSha + "/\">build</a> 2026-11-12 10:00 -\n"
                        + "<a href=\"" + installedSha + "/\">build</a> 2026-11-11 10:00 -\n"
                        + "<a href=\"" + previousSha + "/\">build</a> 2026-11-10 10:00 -\n",
                    board, branch, "https://example.test/" + board + "/" + branch + "/"));
                pane[0].render(SessionState.CONNECTED, null);
            });

            boolean[] ready = new boolean[1];
            long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
            while (System.nanoTime() < deadline && !ready[0]) {
                SwingUtilities.invokeAndWait(() -> {
                    List<Component> all = UiUtils.getAllComponents((JComponent) pane[0].getContent());
                    ready[0] = all.stream().anyMatch(c -> c instanceof JButton
                            && "Rollback Firmware".equals(((JButton) c).getText())
                            && c.isVisible() && c.isEnabled())
                        && all.stream().anyMatch(c -> c instanceof JButton
                            && "Switch firmware branch".equals(((JButton) c).getText())
                            && c.isVisible() && c.isEnabled());
                });
                Thread.sleep(10);
            }
            assertTrue(ready[0], "LTS history enables rollback and the live ECU enables branch switching");

            SwingUtilities.invokeAndWait(() -> {
                JComponent content = (JComponent) pane[0].getContent();
                Map<JComponent, Font> originalFonts = new IdentityHashMap<>();
                for (Component component : UiUtils.getAllComponents(content)) {
                    if (component instanceof JComponent && component.getFont() != null) {
                        originalFonts.put((JComponent) component, component.getFont());
                    }
                }
                boolean sawHorizontalActions = false;
                boolean sawStackedActions = false;
                for (int fontScale : new int[]{1, 2}) {
                  for (Map.Entry<JComponent, Font> entry : originalFonts.entrySet()) {
                      entry.getKey().setFont(entry.getValue().deriveFont(entry.getValue().getSize2D() * fontScale));
                      entry.getKey().invalidate();
                  }
                  JSplitButton sizingSplit = (JSplitButton) UiUtils.getAllComponents(content).stream()
                      .filter(c -> c instanceof JSplitButton).findFirst().orElseThrow(AssertionError::new);
                  JButton sizingBranchSwitch = (JButton) UiUtils.getAllComponents(content).stream()
                      .filter(c -> c instanceof JButton && "Switch firmware branch".equals(((JButton) c).getText()))
                      .findFirst().orElseThrow(AssertionError::new);
                  sizingBranchSwitch.setPreferredSize(null);
                  Dimension naturalBranchSize = sizingBranchSwitch.getPreferredSize();
                  sizingBranchSwitch.setPreferredSize(new Dimension(naturalBranchSize.width,
                      sizingSplit.getPreferredSize().height + 1));
                  invalidateLayoutRecursively(content);
                  for (int width : new int[]{2000, 1200, 960, 850, 720, 640, 480, 960, 2000}) {
                    content.setSize(width, 1400);
                    content.invalidate();
                    layoutRecursively(content);
                    layoutRecursively(content);
                    layoutRecursively(content);
                    FirmwareUpdateCard card = (FirmwareUpdateCard) UiUtils.getAllComponents(content).stream()
                        .filter(c -> c instanceof FirmwareUpdateCard).findFirst().orElseThrow(AssertionError::new);
                    JSplitButton split = (JSplitButton) UiUtils.getAllComponents(content).stream()
                        .filter(c -> c instanceof JSplitButton).findFirst().orElseThrow(AssertionError::new);
                    JButton branchSwitch = (JButton) UiUtils.getAllComponents(content).stream()
                        .filter(c -> c instanceof JButton && "Switch firmware branch".equals(((JButton) c).getText()))
                        .findFirst().orElseThrow(AssertionError::new);
                    JButton rollback = (JButton) UiUtils.getAllComponents(content).stream()
                        .filter(c -> c instanceof JButton && "Rollback Firmware".equals(((JButton) c).getText()))
                        .findFirst().orElseThrow(AssertionError::new);
                    List<JButton> actions = List.of((JButton) split.getComponent(0), (JButton) split.getComponent(1),
                        branchSwitch, rollback);
                    assertTrue(actions.stream().allMatch(c -> c.isVisible() && c.isEnabled()));
                    assertEquals(split.getPreferredSize().height + 1, branchSwitch.getPreferredSize().height,
                        "fixture deterministically makes the branch action one pixel taller");
                    assertEquals(split.getHeight(), branchSwitch.getHeight(),
                        "horizontal and stacked actions share the normalized row height");
                    assertTrue(branchSwitch.getPreferredSize().width >= branchSwitch.getFontMetrics(branchSwitch.getFont())
                        .stringWidth(branchSwitch.getText()) + branchSwitch.getInsets().left
                        + branchSwitch.getInsets().right,
                        "branch action has enough natural width for its full label");
                    int rollbackTextWidth = rollback.getFontMetrics(rollback.getFont()).stringWidth(rollback.getText())
                        + rollback.getInsets().left + rollback.getInsets().right;
                    assertTrue(rollback.getWidth() >= rollbackTextWidth,
                        "recovery action shows its full label at font scale " + fontScale + "x: button="
                            + rollback.getBounds() + ", parent=" + rollback.getParent().getBounds()
                            + ", preferred=" + rollback.getPreferredSize()
                            + ", minimum=" + rollback.getMinimumSize()
                            + ", orientationLeftToRight="
                                + rollback.getParent().getComponentOrientation().isLeftToRight()
                            + ", textWidth=" + rollbackTextWidth);
                    JButton primary = (JButton) split.getComponent(0);
                    int primaryTextWidth = primary.getFontMetrics(primary.getFont()).stringWidth(primary.getText());
                    int iconWidth = primary.getIcon() == null ? 0
                        : primary.getIcon().getIconWidth() + primary.getIconTextGap();
                    assertTrue(primary.getWidth() >= primaryTextWidth + iconWidth
                        + primary.getInsets().left + primary.getInsets().right,
                        "primary action shows its full label at font scale " + fontScale + "x: button="
                            + primary.getBounds() + ", textWidth=" + primaryTextWidth + ", iconWidth=" + iconWidth
                            + ", insets=" + primary.getInsets() + ", split=" + split.getBounds());
                    for (JButton action : actions) {
                        Rectangle bounds = SwingUtilities.convertRectangle(action.getParent(), action.getBounds(), card);
                        assertTrue(bounds.x >= 0 && bounds.y >= 0
                            && bounds.x + bounds.width <= card.getWidth()
                            && bounds.y + bounds.height <= card.getHeight(),
                            action.getText() + " is fully reachable inside the card: " + bounds + " card=" + card.getBounds());
                    }
                    JLabel branchLabel = (JLabel) UiUtils.getAllComponents(content).stream()
                        .filter(c -> c instanceof JLabel && "Branch".equals(((JLabel) c).getText()))
                        .findFirst().orElseThrow(AssertionError::new);
                    JLabel installedLabel = (JLabel) UiUtils.getAllComponents(content).stream()
                        .filter(c -> c instanceof JLabel && "Installed firmware".equals(((JLabel) c).getText()))
                        .findFirst().orElseThrow(AssertionError::new);
                    JLabel latestLabel = (JLabel) UiUtils.getAllComponents(content).stream()
                        .filter(c -> c instanceof JLabel && "Latest firmware".equals(((JLabel) c).getText()))
                        .findFirst().orElseThrow(AssertionError::new);
                    FirmwareStatusSummaryPanel summary = (FirmwareStatusSummaryPanel)
                        SwingUtilities.getAncestorOfClass(FirmwareStatusSummaryPanel.class, split);
                    JTextArea branchValue = (JTextArea) UiUtils.getAllComponents(content).stream()
                        .filter(c -> c instanceof JTextArea && "Lima (LTS)".equals(((JTextArea) c).getText()))
                        .findFirst().orElseThrow(AssertionError::new);
                    assertTrue(branchLabel.getWidth() >= branchLabel.getFontMetrics(branchLabel.getFont())
                        .stringWidth(branchLabel.getText()), "Branch label fits without clipping");
                    for (JLabel label : List.of(installedLabel, latestLabel)) {
                        assertTrue(label.getWidth() >= label.getFontMetrics(label.getFont()).stringWidth(label.getText()),
                            label.getText() + " label fits at font scale " + fontScale + "x");
                    }
                    assertSame(installedLabel.getParent(), latestLabel.getParent(),
                        "Installed and latest dates share the four-row identity grid");
                    assertTrue(latestLabel.getY() > installedLabel.getY(),
                        "Latest firmware remains below Installed firmware");
                    List<JLabel> dateValues = java.util.Arrays.stream(installedLabel.getParent().getComponents())
                        .filter(c -> c instanceof JLabel
                            && !List.of("ECU", "Branch", "Installed firmware", "Latest firmware")
                                .contains(((JLabel) c).getText()))
                        .map(c -> (JLabel) c).sorted(java.util.Comparator.comparingInt(Component::getY))
                        .collect(java.util.stream.Collectors.toList());
                    assertEquals(2, dateValues.size(), "Both date values remain separate");
                    JLabel installedDate = dateValues.get(0);
                    JLabel latestDate = dateValues.get(1);
                    assertTrue(installedDate.getWidth() >= installedDate.getFontMetrics(installedDate.getFont())
                        .stringWidth(installedDate.getText()), "installed date value is fully visible");
                    assertTrue(latestDate.getWidth() >= latestDate.getFontMetrics(latestDate.getFont())
                        .stringWidth(latestDate.getText()), "latest date value is fully visible");
                    Rectangle branchBounds = SwingUtilities.convertRectangle(branchValue.getParent(),
                        branchValue.getBounds(), summary);
                    Rectangle switchBounds = SwingUtilities.convertRectangle(branchSwitch.getParent(),
                        branchSwitch.getBounds(), summary);
                    assertSame(split.getParent(), branchSwitch.getParent(),
                        "branch switch shares the primary action row");
                    assertTrue(branchLabel.getY() > 0, "branch remains an informational identity row");
                    assertFalse(branchBounds.intersects(switchBounds),
                        "branch information never overlaps the switch action");
                    Rectangle actionBounds = SwingUtilities.convertRectangle(split.getParent(), split.getBounds(), summary);
                    Rectangle groupedActions = actionBounds.union(switchBounds);
                    assertTrue(Math.abs((groupedActions.x + groupedActions.width / 2) - summary.getWidth() / 2) <= 2,
                        "primary and branch actions are centered as a group at width " + width + ", font " + fontScale + "x");
                    if (branchSwitch.getY() != split.getY()) {
                        sawStackedActions = true;
                        assertFalse(branchSwitch.getBounds().intersects(split.getBounds()),
                            "wrapped actions do not overlap at width " + width + ", font " + fontScale + "x");
                        assertEquals(split.getParent().getWidth() / 2, split.getX() + split.getWidth() / 2,
                            2, "wrapped primary remains centered");
                        assertEquals(split.getParent().getWidth() / 2,
                            branchSwitch.getX() + branchSwitch.getWidth() / 2, 2, "wrapped branch remains centered");
                    } else {
                        sawHorizontalActions = true;
                    }
                    assertTrue(split.getWidth() < summary.getWidth() - 2,
                        "primary firmware action uses natural width instead of stretching across the summary");
                    Rectangle rollbackBounds = SwingUtilities.convertRectangle(
                        rollback.getParent(), rollback.getBounds(), card);
                JCheckBox softwareOption = (JCheckBox) UiUtils.getAllComponents(content).stream()
                    .filter(c -> c instanceof JCheckBox
                        && "Update this console software automatically when a newer version is available"
                            .equals(((JCheckBox) c).getToolTipText()))
                    .findFirst().orElseThrow(AssertionError::new);
                Rectangle softwareBounds = SwingUtilities.convertRectangle(
                    softwareOption.getParent(), softwareOption.getBounds(), card);
                assertTrue(softwareBounds.y + softwareBounds.height <= card.getHeight(),
                    "wrapped software option remains fully visible inside the card at font scale 2x");
                int softwareOptionY = SwingUtilities.convertPoint(softwareOption, 0, 0, card).y;
                assertTrue(rollbackBounds.y + rollbackBounds.height <= softwareOptionY,
                    "compact branch reflow leaves the full recovery button before software options: "
                        + rollbackBounds + ", softwareY=" + softwareOptionY + ", card=" + card.getBounds()
                        + ", rollbackParent=" + rollback.getParent().getBounds() + ", summary="
                        + SwingUtilities.getAncestorOfClass(FirmwareStatusSummaryPanel.class, rollback).getBounds());
                  }
                }
                assertTrue(sawHorizontalActions, "preferred-height mismatch is exercised in the horizontal row");
                assertTrue(sawStackedActions, "preferred-height mismatch is exercised in the stacked row");
            });
        } finally {
            if (previousRollbackFlag == null) {
                System.clearProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
            } else {
                System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", previousRollbackFlag);
            }
        }
    }

    @Test
    void tuneMigrationCheckboxMatchesTheOperationSetting() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            boolean previous = com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded;
            try {
                DevicePane pane = createPane();
                JCheckBox tune = (JCheckBox) UiUtils.getAllComponents((JComponent) pane.getContent()).stream()
                    .filter(c -> c instanceof JCheckBox
                        && "Keep my current tune".equals(((JCheckBox) c).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                // Tune preservation is enabled by default and must be represented faithfully in the UI.
                assertTrue(tune.isSelected());
                assertTrue(com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded);
            } finally {
                com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded = previous;
            }
        });
    }

    @Test
    void tuneMigrationCheckboxControlsTheOperationSetting() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            boolean previous = com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded;
            try {
                DevicePane pane = createPane();
                JCheckBox tune = (JCheckBox) UiUtils.getAllComponents((JComponent) pane.getContent()).stream()
                    .filter(c -> c instanceof JCheckBox
                        && "Keep my current tune".equals(((JCheckBox) c).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                tune.doClick();
                assertFalse(tune.isSelected());
                assertFalse(com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded);
                tune.doClick();
                assertTrue(tune.isSelected());
                assertTrue(com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded);
            } finally {
                com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded = previous;
            }
        });
    }

    /** Builds the pane as ConsoleUI does, with a fake scanner reporting one serial ECU. */
    static DevicePane createPane() {
        return createPane(new UIContext());
    }

    private static DevicePane createPane(UIContext uiContext) {
        return createPane(uiContext, Collections.singletonList(new PortResult("COM4", SerialPortType.Ecu)));
    }

    private static DevicePane createPane(UIContext uiContext, List<PortResult> ports) {
        FakePortScanner scanner = new FakePortScanner();
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        scanner.fireHardwareChange(new AvailableHardware(
            ports, false, false, false));
        StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
        return new DevicePane(uiContext, connectivity,
            new DeviceSessionManager(connectivity, null, () -> { }, () -> { }), new JTabbedPane(),
            new SingleAsyncJobExecutor(status), status, panel -> { }, () -> { });
    }

    /** The live Device wiring must bind the hidden update target to the actual connected session port. */
    @Test
    void liveDeviceSelectsConnectedSessionInsteadOfFirstDetectedEcu() throws Throwable {
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        protocol.signature = "rusEFI master.2026.11.11.uaefi_pro.123456";
        when(linkManager.getBinaryProtocol()).thenReturn(protocol);
        when(linkManager.getLastTriedPort()).thenReturn("ECU_B");
        UIContext uiContext = new UIContext() {
            @Override
            public LinkManager getLinkManager() {
                return linkManager;
            }
        };
        DevicePane[] pane = new DevicePane[1];
        SwingUtilities.invokeAndWait(() -> {
            pane[0] = createPane(uiContext, List.of(
                new PortResult("ECU_A", SerialPortType.Ecu),
                new PortResult("ECU_B", SerialPortType.Ecu)));
            pane[0].setDevMetadataProviderForTests((board, branch) ->
                LocalDate.now(ZoneOffset.UTC).atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli());
            pane[0].render(SessionState.CONNECTED, null);
        });

        SwingUtilities.invokeAndWait(() -> {
            JTextArea releaseStatus = (JTextArea) UiUtils.getAllComponents((JComponent) pane[0].getContent()).stream()
                .filter(c -> c instanceof JTextArea
                    && SwingUtilities.getAncestorOfClass(FirmwareStatusSummaryPanel.class, c) != null)
                .findFirst().orElseThrow(AssertionError::new);
            assertFalse(releaseStatus.getText().contains("Reconnect the ECU"),
                "the production Device selection is bound to ECU_B even though ECU_A was detected first");
            pane[0].render(SessionState.CONNECTED,
                new AvailableHardware(List.of(new PortResult("ECU_A", SerialPortType.Ecu)), false, false, false));
            assertEquals("Couldn't check for updates. Reconnect the ECU and try again.", releaseStatus.getText(),
                "when the live port is missing from the detection snapshot, do not fall back to another ECU");
        });
    }

    @Test
    void connectedUaefiDevCardShowsAvailableUpdateAtWideAndNarrowWidths() throws Throwable {
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        protocol.signature = "rusEFI master.2026.11.11.uaefi_pro.123456";
        when(linkManager.getBinaryProtocol()).thenReturn(protocol);
        when(linkManager.getLastTriedPort()).thenReturn("ECU_B");
        UIContext uiContext = new UIContext() {
            @Override
            public LinkManager getLinkManager() {
                return linkManager;
            }
        };
        DevicePane[] pane = new DevicePane[1];
        SwingUtilities.invokeAndWait(() -> {
            pane[0] = createPane(uiContext, List.of(
                new PortResult("ECU_A", SerialPortType.Ecu),
                new PortResult("ECU_B", SerialPortType.EcuWithOpenblt)));
            pane[0].setDevMetadataProviderForTests((board, branch) -> {
                assertEquals("uaefi_pro", board);
                assertEquals(com.rusefi.core.io.BundleInfo.DEVELOPMENT, branch);
                return LocalDate.now(ZoneOffset.UTC).atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli();
            });
            pane[0].render(SessionState.CONNECTED, null);
        });

        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        while (System.nanoTime() < deadline) {
            final boolean[] ready = new boolean[1];
            SwingUtilities.invokeAndWait(() -> ready[0] = UiUtils.getAllComponents((JComponent) pane[0].getContent()).stream()
                .anyMatch(c -> c instanceof JTextArea && "Update available".equals(((JTextArea) c).getText())));
            if (ready[0]) {
                break;
            }
            Thread.sleep(10);
        }

                for (int width : new int[]{2000, 960, 480}) {
            SwingUtilities.invokeAndWait(() -> {
                JComponent content = (JComponent) pane[0].getContent();
                content.setSize(width, 800);
                layoutRecursively(content);
                List<Component> all = UiUtils.getAllComponents(content);
                JTextArea branch = (JTextArea) all.stream().filter(c -> c instanceof JTextArea
                    && "Dev".equals(((JTextArea) c).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                assertEquals("Dev", branch.getText());
                JTextArea status = (JTextArea) all.stream().filter(c -> c instanceof JTextArea
                    && "Update available".equals(((JTextArea) c).getText())
                    && SwingUtilities.getAncestorOfClass(FirmwareStatusSummaryPanel.class, c) != null)
                    .findFirst().orElseThrow(AssertionError::new);
                assertTrue(status.isVisible(), "availability is shown in the prominent status row");
                assertFalse(branch.getText().contains("signature"));
                assertFalse(branch.getText().contains("SHA"));
                if (width == 480) {
                    try {
                        java.awt.geom.Rectangle2D end = branch.modelToView2D(branch.getDocument().getLength());
                        assertTrue(end.getMaxY() <= branch.getHeight(),
                            "long branch status wraps within the narrow card instead of clipping: text="
                                + branch.getBounds() + " end=" + end);
                    } catch (javax.swing.text.BadLocationException e) {
                        throw new AssertionError(e);
                    }
                }
                JSplitButton update = (JSplitButton) all.stream().filter(c -> c instanceof JSplitButton)
                    .findFirst().orElseThrow(AssertionError::new);
                assertEquals("Update firmware", ((JButton) update.getComponent(0)).getText());
                assertTrue(((JButton) update.getComponent(0)).isEnabled());
            });
        }
    }

    @Test
    void startupFirmwareCardUsesTheSameCenteredProminentLayout() throws Throwable {
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        protocol.signature = "rusEFI master.2026.11.11.uaefi_pro.123456";
        when(linkManager.getBinaryProtocol()).thenReturn(protocol);
        when(linkManager.getLastTriedPort()).thenReturn("ECU_B");
        PortResult connected = new PortResult("ECU_B", SerialPortType.EcuWithOpenblt);
        FakePortScanner scanner = new FakePortScanner();
        AvailableHardware hardware = new AvailableHardware(List.of(connected), false, false, false);
        scanner.fireHardwareChange(hardware);
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        StatusPanelWithProgressBar statusPanel = new StatusPanelWithProgressBar();
        JPanel[] panel = new JPanel[1];
        FirmwareStatusSummaryPanel[] summary = new FirmwareStatusSummaryPanel[1];
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<Optional<PortResult>> selected = new AtomicReference<>(Optional.of(connected));
            StartupUpdateActions actions = new StartupUpdateActions(connectivity, statusPanel,
                new SingleAsyncJobExecutor(statusPanel), selected,
                new SoftwareUpdateCoordinator(new CompletableFuture<>()),
                ignored -> { }, () -> { }, () -> null, () -> { });
            // Exercise the visible manual-update control regardless of this machine's persisted auto-update preference.
            actions.getUpdateSoftwareButton().setVisible(true);
            actions.getUpdateSoftwareButton().setEnabled(true);
            actions.setDevMetadataProviderForTests((board, branch) ->
                LocalDate.now(ZoneOffset.UTC).atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli());
            JComboBox<PortResult> ports = new JComboBox<>();
            ports.addItem(connected);
            ports.setSelectedItem(connected);
            ProgramSelector selector = new ProgramSelector(connectivity, ports, ignored -> { }, () -> { });
            selector.setLinkManager(linkManager);
            actions.configureFirmwareSelector(selector);
            actions.setSplashLinkManager(linkManager);
            actions.onHardwareUpdated();
            selector.apply(hardware);
            panel[0] = new JPanel(new BorderLayout());
            panel[0].add(FirmwareUpdateCard.inResponsiveViewport(
                StartupFrame.createFirmwareUpdateCard(actions, selector, statusPanel)), BorderLayout.CENTER);
            summary[0] = (FirmwareStatusSummaryPanel) actions.getFirmwareSummary();
        });

        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        boolean[] ready = new boolean[1];
        while (System.nanoTime() < deadline) {
            SwingUtilities.invokeAndWait(() -> {
                List<Component> components = UiUtils.getAllComponents(summary[0]);
                ready[0] = components.stream().anyMatch(c -> c instanceof JTextArea
                    && "Update available".equals(((JTextArea) c).getText()));
            });
            if (ready[0]) {
                break;
            }
            Thread.sleep(10);
        }
        SwingUtilities.invokeAndWait(() -> {
            List<Component> components = UiUtils.getAllComponents(summary[0]);
            assertTrue(ready[0], "Startup fixture reaches release-status ready state: textAreas="
                + components.stream().filter(c -> c instanceof JTextArea)
                    .map(c -> ((JTextArea) c).getText()).collect(java.util.stream.Collectors.toList())
                + ", buttons=" + components.stream().filter(c -> c instanceof JButton)
                    .map(c -> ((JButton) c).getText() + "=" + c.isVisible() + "/" + c.isEnabled())
                    .collect(java.util.stream.Collectors.toList()));
            JButton switchBranch = (JButton) components.stream().filter(c -> c instanceof JButton
                && "Switch firmware branch".equals(((JButton) c).getText()))
                .findFirst().orElseThrow(AssertionError::new);
            assertFalse(switchBranch.isVisible(), "master firmware is not an eligible branch-switch source");
        });
        SwingUtilities.invokeAndWait(() -> assertStartupNarrowHighFontLayout(panel[0], summary[0]));
        for (int width : new int[]{2000, 960, 480}) {
            SwingUtilities.invokeAndWait(() -> {
                panel[0].setSize(width, 900);
                layoutRecursively(panel[0]);
                FirmwareUpdateCard card = (FirmwareUpdateCard) UiUtils.getAllComponents(panel[0]).stream()
                    .filter(c -> c instanceof FirmwareUpdateCard).findFirst().orElseThrow(AssertionError::new);
                JScrollPane scroll = (JScrollPane) UiUtils.getAllComponents(panel[0]).stream()
                    .filter(c -> c instanceof JScrollPane).findFirst().orElseThrow(AssertionError::new);
                assertTrue(Math.abs(2 * card.getX() + card.getWidth() - scroll.getViewport().getWidth()) <= 2,
                    "startup firmware card is centered at width " + width);
                double ratio = (double) card.getWidth() / scroll.getViewport().getWidth();
                if (scroll.getViewport().getWidth() >= 1400) {
                    assertTrue(Math.abs(ratio - 0.5) < 0.01, "large viewport card ratio at " + width + " is " + ratio);
                } else if (scroll.getViewport().getWidth() >= 900) {
                    assertTrue(Math.abs(ratio - 0.75) < 0.01, "medium viewport card ratio at " + width + " is " + ratio);
                } else {
                    assertTrue(ratio >= 0.90 && ratio < 1.0, "small viewport card ratio at " + width + " is " + ratio);
                }
                JSplitButton update = (JSplitButton) UiUtils.getAllComponents(panel[0]).stream()
                    .filter(c -> c instanceof JSplitButton).findFirst().orElseThrow(AssertionError::new);
                assertSame(card, SwingUtilities.getAncestorOfClass(FirmwareUpdateCard.class, update));
                JButton primary = (JButton) update.getComponent(0);
                assertTrue(update.getHeight() >= primary.getFontMetrics(primary.getFont()).getHeight()
                    + primary.getInsets().top + primary.getInsets().bottom);
                Rectangle actionBounds = SwingUtilities.convertRectangle(update.getParent(), update.getBounds(),
                    summary[0]);
                assertTrue(update.getWidth() < summary[0].getWidth() - 2,
                    "Startup primary action uses its natural label width");
                assertEquals("Update firmware", ((JButton) update.getComponent(0)).getText());
                assertTrue(((JButton) update.getComponent(0)).isEnabled());
                List<Component> summaryComponents = UiUtils.getAllComponents(summary[0]);
                List<String> identityLabels = List.of("ECU", "Branch", "Installed firmware", "Latest firmware");
                List<JLabel> labels = identityLabels.stream().map(text -> (JLabel) summaryComponents.stream()
                    .filter(c -> c instanceof JLabel && text.equals(((JLabel) c).getText()))
                    .findFirst().orElseThrow(AssertionError::new)).collect(java.util.stream.Collectors.toList());
                assertTrue(labels.stream().allMatch(label -> label.getParent() == labels.get(0).getParent()),
                    "All four identity rows use one shared grid");
                JPanel identityGrid = (JPanel) labels.get(0).getParent();
                List<Component> dateValues = identityGrid.getComponents().length == 8
                    ? java.util.Arrays.stream(identityGrid.getComponents())
                        .filter(c -> c instanceof JLabel && !identityLabels.contains(((JLabel) c).getText()))
                        .sorted(java.util.Comparator.comparingInt(Component::getY))
                        .collect(java.util.stream.Collectors.toList())
                    : Collections.emptyList();
                assertEquals(2, dateValues.size(), "Installed and latest values remain separate grid cells");
                List<Component> values = List.of(
                    summaryComponents.stream().filter(c -> c instanceof JTextArea
                        && ((JTextArea) c).getText().startsWith("uaefi_pro")).findFirst().orElseThrow(AssertionError::new),
                    summaryComponents.stream().filter(c -> c instanceof JTextArea
                        && "Dev".equals(((JTextArea) c).getText())).findFirst().orElseThrow(AssertionError::new),
                    dateValues.get(0), dateValues.get(1));
                int labelsX = SwingUtilities.convertPoint(identityGrid, labels.get(0).getX(), 0, summary[0]).x;
                int valuesX = SwingUtilities.convertPoint(identityGrid, values.get(0).getX(), 0, summary[0]).x;
                boolean stackedRows = values.get(0).getX() == labels.get(0).getX();
                if (!stackedRows) {
                    assertTrue(labels.stream().allMatch(label -> label.getX() == labels.get(0).getX()),
                        "Inline identity labels share one grid column at width " + width + ": "
                            + labels.stream().map(label -> label.getText() + "=" + label.getBounds())
                            .collect(java.util.stream.Collectors.joining(", ")));
                    assertTrue(values.stream().allMatch(value -> value.getX() == values.get(0).getX()),
                        "Inline identity values share one grid column");
                    assertTrue(values.get(2).getX() - labels.get(2).getX() - labels.get(2).getWidth()
                        >= com.formdev.flatlaf.util.UIScale.scale(12),
                        "Identity label/value columns keep a 12px gap");
                } else {
                    assertTrue(labels.stream().allMatch(label -> label.getX() == labels.get(0).getX())
                        && values.stream().allMatch(value -> value.getX() == values.get(0).getX()),
                        "All identity rows stack consistently");
                }
                int expectedGap = 4;
                for (int i = 0; i < labels.size() - 1; i++) {
                    int rowBottom = Math.max(labels.get(i).getY() + labels.get(i).getHeight(),
                        values.get(i).getY() + values.get(i).getHeight());
                    int nextRowTop = Math.min(labels.get(i + 1).getY(), values.get(i + 1).getY());
                    assertEquals(expectedGap, nextRowTop - rowBottom,
                        "Identity rows have equal compact spacing");
                }
                JTextArea status = (JTextArea) java.util.Arrays.stream(summary[0].getComponents())
                    .filter(component -> component instanceof JTextArea).findFirst()
                    .orElseThrow(AssertionError::new);
                JCheckBox keepTune = (JCheckBox) summaryComponents.stream().filter(c -> c instanceof JCheckBox
                    && "Keep my current tune".equals(((JCheckBox) c).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                assertEquals(labelsX, SwingUtilities.convertPoint(identityGrid, 0, 0, summary[0]).x,
                    "Identity grid shares the summary's left alignment");
                assertTrue(values.stream().allMatch(value -> value.getX() == values.get(0).getX()),
                    "All identity values share one grid column");
                assertEquals(labelsX, SwingUtilities.convertPoint(status, 0, 0, summary[0]).x,
                    "Startup status shares the summary's left alignment");
                assertEquals(labelsX, SwingUtilities.convertPoint(keepTune, 0, 0, summary[0]).x,
                    "Startup tune option shares the summary's left alignment");
                JButton switchBranch = (JButton) UiUtils.getAllComponents(summary[0]).stream()
                    .filter(c -> c instanceof JButton && "Switch firmware branch".equals(((JButton) c).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                assertEquals("Switch firmware branch", switchBranch.getText());
                assertSame(update.getParent(), switchBranch.getParent(),
                    "Branch switch shares the primary action row");
                if (switchBranch.isVisible()) {
                    Rectangle branchBounds = SwingUtilities.convertRectangle(switchBranch.getParent(),
                        switchBranch.getBounds(), summary[0]);
                    assertTrue(branchBounds.y >= 0 && branchBounds.y + branchBounds.height <= summary[0].getHeight(),
                        "Branch action is reachable inside the summary");
                    Rectangle groupedActions = actionBounds.union(branchBounds);
                    assertTrue(Math.abs(groupedActions.x + groupedActions.width / 2 - summary[0].getWidth() / 2) <= 2,
                        "Primary and branch actions are centered as a group");
                    if (width == 480 && update.getY() != switchBranch.getY()) {
                        assertFalse(update.getBounds().intersects(switchBranch.getBounds()),
                            "Wrapped branch action does not overlap the primary action");
                        assertEquals(update.getParent().getWidth() / 2,
                            update.getX() + update.getWidth() / 2, 2);
                        assertEquals(update.getParent().getWidth() / 2,
                            switchBranch.getX() + switchBranch.getWidth() / 2, 2);
                    }
                } else {
                    Rectangle centeredPrimary = SwingUtilities.convertRectangle(update.getParent(),
                        update.getBounds(), summary[0]);
                    assertEquals(summary[0].getWidth() / 2, centeredPrimary.x + centeredPrimary.width / 2,
                        2, "Primary action is centered alone when branch switching is ineligible");
                }
                assertTrue(summaryComponents.stream().filter(c -> c instanceof JTextArea
                    && "Update available".equals(((JTextArea) c).getText())).anyMatch(Component::isVisible));
                for (Component component : UiUtils.getAllComponents(panel[0])) {
                    if (component instanceof JCheckBox || component instanceof JProgressBar || component instanceof JTextArea
                        || component instanceof JButton && DevicePane.COPY_LOG.equals(((JButton) component).getText())) {
                        assertSame(card, SwingUtilities.getAncestorOfClass(FirmwareUpdateCard.class, component),
                            component.getClass().getSimpleName() + " belongs to Startup's card");
                    }
                }
            });
        }

    }

    private static void assertStartupNarrowHighFontLayout(JPanel panel, FirmwareStatusSummaryPanel summary) {
        Map<JComponent, Font> originalFonts = new IdentityHashMap<>();
        for (Component component : UiUtils.getAllComponents(panel)) {
            if (component instanceof JComponent && component.getFont() != null) {
                JComponent swingComponent = (JComponent) component;
                originalFonts.put(swingComponent, swingComponent.getFont());
                swingComponent.setFont(swingComponent.getFont().deriveFont(
                    swingComponent.getFont().getSize2D() * 2));
            }
        }
        panel.setSize(480, 1200);
        layoutRecursively(panel);
        layoutRecursively(panel);
        layoutRecursively(panel);
        FirmwareUpdateCard card = (FirmwareUpdateCard) UiUtils.getAllComponents(panel).stream()
            .filter(c -> c instanceof FirmwareUpdateCard).findFirst().orElseThrow(AssertionError::new);
        JTextArea backupNote = (JTextArea) UiUtils.getAllComponents(summary).stream()
            .filter(c -> c instanceof JTextArea
                && ((JTextArea) c).getText().startsWith("A tune backup is attempted first"))
            .findFirst().orElseThrow(AssertionError::new);
        JSplitButton update = (JSplitButton) UiUtils.getAllComponents(panel).stream()
            .filter(c -> c instanceof JSplitButton).findFirst().orElseThrow(AssertionError::new);
        JButton switchBranch = (JButton) UiUtils.getAllComponents(summary).stream()
            .filter(c -> c instanceof JButton && "Switch firmware branch".equals(((JButton) c).getText()))
            .findFirst().orElseThrow(AssertionError::new);
        Rectangle updateBounds = SwingUtilities.convertRectangle(update.getParent(), update.getBounds(), card);
        assertTrue(update.isVisible() && ((JButton) update.getComponent(0)).isVisible(),
            "Startup primary update action is visible at 480px/2×");
        assertTrue(updateBounds.y >= 0 && updateBounds.y + updateBounds.height <= card.getHeight(),
            "Startup primary action fits inside the card: action=" + updateBounds + ", card=" + card.getBounds());
        if (switchBranch.isVisible()) {
            assertSame(update.getParent(), switchBranch.getParent(),
                "The branch action remains grouped with the primary action at 480px/2×");
            Rectangle branchBounds = switchBranch.getBounds();
            Rectangle primaryBounds = update.getBounds();
            assertFalse(primaryBounds.intersects(branchBounds),
                "The stacked primary and branch actions do not overlap at 480px/2×");
            assertEquals(update.getParent().getWidth() / 2, primaryBounds.x + primaryBounds.width / 2,
                2, "The primary action stays centered when the group wraps");
            assertEquals(update.getParent().getWidth() / 2, branchBounds.x + branchBounds.width / 2,
                2, "The branch action stays centered when the group wraps");
        }
        try {
            assertTrue(backupNote.modelToView2D(backupNote.getDocument().getLength()).getMaxY()
                <= backupNote.getHeight(), "Startup backup note wraps without clipping at 480px/2×");
        } catch (javax.swing.text.BadLocationException e) {
            throw new AssertionError(e);
        }
        Rectangle noteBounds = SwingUtilities.convertRectangle(backupNote.getParent(), backupNote.getBounds(), card);
        assertTrue(noteBounds.y >= 0 && noteBounds.y + noteBounds.height <= card.getHeight(),
            "Startup backup note fits inside the card: note=" + noteBounds + ", card=" + card.getBounds());

        List<Component> all = UiUtils.getAllComponents(panel);
        JButton updateSoftware = (JButton) all.stream().filter(c -> c instanceof JButton
            && "Update application".equals(((JButton) c).getText()))
            .findFirst().orElseThrow(AssertionError::new);
        JLabel firmwareOptions = (JLabel) all.stream().filter(c -> c instanceof JLabel
            && "Firmware options:".equals(((JLabel) c).getText()))
            .findFirst().orElseThrow(AssertionError::new);
        List<Component> secondaryControls = new ArrayList<>();
        secondaryControls.add(updateSoftware);
        if (firmwareOptions.isVisible()) {
            secondaryControls.add(firmwareOptions);
        }
        for (Component control : secondaryControls) {
            Rectangle bounds = SwingUtilities.convertRectangle(control.getParent(), control.getBounds(), card);
            assertTrue(control.isVisible() && bounds.y >= 0 && bounds.y + bounds.height <= card.getHeight(),
                control.getClass().getSimpleName() + " remains reachable inside Startup card: "
                    + (control instanceof JButton ? "text=" + ((JButton) control).getText()
                        + ", name=" + control.getName() + ", " : "")
                    + bounds + ", card=" + card.getBounds());
            for (Component current = control; current != card; current = current.getParent()) {
                Container parent = current.getParent();
                assertNotNull(parent, "Startup control remains inside the card hierarchy");
                Rectangle childBounds = current.getBounds();
                assertTrue(childBounds.x >= 0 && childBounds.y >= 0
                    && childBounds.x + childBounds.width <= parent.getWidth()
                    && childBounds.y + childBounds.height <= parent.getHeight(),
                    current.getClass().getSimpleName() + " is not clipped by its ancestor "
                        + parent.getClass().getSimpleName() + " while checking " + control + ": child=" + childBounds
                        + ", parent=" + parent.getBounds());
            }
        }
        assertTrue(card.getHeight() >= card.getPreferredSize().height,
            "Startup card height includes all wrapped controls: actual=" + card.getHeight()
                + ", preferred=" + card.getPreferredSize().height);

        originalFonts.forEach(JComponent::setFont);
        panel.setSize(2000, 900);
        layoutRecursively(panel);
    }

    @Test
    void devicePaneRefreshesDevIdentityWhenSignatureArrivesOnTheSameLivePort() throws Throwable {
        PortResult connected = new PortResult("ECU_DEV", SerialPortType.EcuWithOpenblt);
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        when(linkManager.getBinaryProtocol()).thenReturn(protocol);
        when(linkManager.getLastTriedPort()).thenReturn(connected.port);
        UIContext context = new UIContext() {
            @Override
            public LinkManager getLinkManager() { return linkManager; }
        };
        FakePortScanner scanner = new FakePortScanner();
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        AvailableHardware hardware = new AvailableHardware(List.of(connected), false, false, false);
        scanner.fireHardwareChange(hardware);
        DevicePane[] pane = new DevicePane[1];
        FirmwareStatusSummaryPanel[] summary = new FirmwareStatusSummaryPanel[1];

        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
            DeviceSessionManager sessions = new DeviceSessionManager(connectivity, connected, () -> { }, () -> {}) {
                @Override
                public SessionState getState() { return SessionState.CONNECTED; }
            };
            pane[0] = new DevicePane(context, connectivity, sessions, new JTabbedPane(),
                new SingleAsyncJobExecutor(status), status, ignored -> { }, () -> { });
            pane[0].setDevMetadataProviderForTests((board, branch) -> LocalDate.now(ZoneOffset.UTC)
                .atStartOfDay(ZoneOffset.UTC).toInstant().toEpochMilli());
            pane[0].render(SessionState.CONNECTED, hardware);

            // Model a same-port connection becoming identifiable after the first render.
            protocol.signature = "rusEFI master." + LocalDate.now(ZoneOffset.UTC).toString().replace('-', '.')
                + ".uaefi_pro.build";
            pane[0].render(SessionState.CONNECTED, hardware);
            summary[0] = (FirmwareStatusSummaryPanel) UiUtils.getAllComponents((JComponent) pane[0].getContent())
                .stream().filter(component -> component instanceof FirmwareStatusSummaryPanel)
                .findFirst().orElseThrow(AssertionError::new);
        });

        String expected = "No newer build date found.";
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        String actual = "";
        while (System.nanoTime() < deadline) {
            AtomicReference<String> status = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> status.set(UiUtils.getAllComponents(summary[0]).stream()
                .filter(component -> component instanceof JTextArea
                    && !((JTextArea) component).getText().equals("Dev"))
                .map(component -> ((JTextArea) component).getText()).findFirst().orElse("")));
            actual = status.get();
            if (expected.equals(actual)) {
                break;
            }
            Thread.sleep(10);
        }
        assertEquals(expected, actual);

        SwingUtilities.invokeAndWait(() -> {
            pane[0].setDevMetadataProviderForTests((board, branch) -> LocalDate.now(ZoneOffset.UTC)
                .plusDays(1).atStartOfDay(ZoneOffset.UTC).toInstant().toEpochMilli());
            protocol.signature = "rusEFI master." + LocalDate.now(ZoneOffset.UTC).minusDays(2)
                .toString().replace('-', '.') + ".uaefi_pro.build";
            pane[0].render(SessionState.CONNECTED, hardware);
        });
        long unknownDeadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        actual = "";
        while (System.nanoTime() < unknownDeadline) {
            AtomicReference<String> status = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> status.set(UiUtils.getAllComponents(summary[0]).stream()
                .filter(component -> component instanceof JTextArea
                    && !((JTextArea) component).getText().equals("Dev"))
                .map(component -> ((JTextArea) component).getText()).findFirst().orElse("")));
            actual = status.get();
            if (actual.equals("Couldn't compare firmware versions.")) {
                break;
            }
            Thread.sleep(10);
        }
        assertEquals("Couldn't compare firmware versions.", actual);
        SwingUtilities.invokeAndWait(() -> {
            List<Component> components = UiUtils.getAllComponents(summary[0]);
            JTextArea branch = (JTextArea) components.stream().filter(component -> component instanceof JTextArea
                && "Dev".equals(((JTextArea) component).getText()))
                .findFirst().orElseThrow(AssertionError::new);
            assertEquals("Dev", branch.getText());
            JTextArea status = (JTextArea) components.stream().filter(component -> component instanceof JTextArea
                && "Couldn't compare firmware versions.".equals(((JTextArea) component).getText()))
                .findFirst().orElseThrow(AssertionError::new);
            assertTrue(status.isVisible());
            assertFalse(status.getText().contains("Update available"));
        });
    }

    @Test
    void devicePaneReleaseSnapshotSurvivesSplashFrameContentSwap() throws Throwable {
        PortResult connected = new PortResult("ECU_SWAP", SerialPortType.EcuWithOpenblt);
        AvailableHardware hardware = new AvailableHardware(List.of(connected), false, false, false);
        FakePortScanner scanner = new FakePortScanner();
        scanner.fireHardwareChange(hardware);
        ConnectivityContext connectivity = new ConnectivityContext(scanner);
        LinkManager linkManager = mock(LinkManager.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        String today = LocalDate.now(ZoneOffset.UTC).toString();
        protocol.signature = "rusEFI master." + today.replace('-', '.') + ".uaefi_pro.build";
        when(linkManager.getBinaryProtocol()).thenReturn(protocol);
        when(linkManager.getLastTriedPort()).thenReturn(connected.port);
        UIContext context = new UIContext() {
            @Override
            public LinkManager getLinkManager() { return linkManager; }
        };
        DevicePane[] pane = new DevicePane[1];
        FirmwareStatusSummaryPanel[] summary = new FirmwareStatusSummaryPanel[1];
        JPanel[] frameContentRef = new JPanel[1];

        SwingUtilities.invokeAndWait(() -> {
            UnsupportedEcuCardHost host = new UnsupportedEcuCardHost(connectivity, linkManager);
            host.setNormalContent(new JPanel());
            JPanel frameContent = new JPanel(new BorderLayout());
            frameContentRef[0] = frameContent;
            frameContent.add(host.getContent(), BorderLayout.CENTER);
            // Match the reused, already-visible splash frame without constructing a native JFrame.
            frameContent.addNotify();

            StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
            DeviceSessionManager sessions = new DeviceSessionManager(connectivity, connected, () -> { }, () -> {}) {
                @Override
                public SessionState getState() { return SessionState.CONNECTED; }
            };
            pane[0] = new DevicePane(context, connectivity, sessions, new JTabbedPane(),
                new SingleAsyncJobExecutor(status), status, ignored -> { }, () -> { });
            pane[0].setDevMetadataProviderForTests((board, branch) -> LocalDate.now(ZoneOffset.UTC)
                .atStartOfDay(ZoneOffset.UTC).toInstant().toEpochMilli());
            JPanel consoleContent = new JPanel(new BorderLayout());
            consoleContent.add(pane[0].getContent(), BorderLayout.CENTER);
            host.setNormalContent(consoleContent);

            // FrameHelper.swapContent removes the visible splash host, then adds it back with console content.
            frameContent.removeAll();
            frameContent.add(host.getContent(), BorderLayout.CENTER);
            summary[0] = (FirmwareStatusSummaryPanel) UiUtils.getAllComponents((JComponent) pane[0].getContent())
                .stream().filter(component -> component instanceof FirmwareStatusSummaryPanel)
                .findFirst().orElseThrow(AssertionError::new);
        });

        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        String actual = "";
        while (System.nanoTime() < deadline) {
            AtomicReference<String> status = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> status.set(UiUtils.getAllComponents(summary[0]).stream()
                .filter(component -> component instanceof JTextArea
                    && !((JTextArea) component).getText().equals("Dev"))
                .map(component -> ((JTextArea) component).getText()).findFirst().orElse("")));
            actual = status.get();
            if ("No newer build date found.".equals(actual)) {
                break;
            }
            Thread.sleep(10);
        }
        assertEquals("No newer build date found.", actual);
        SwingUtilities.invokeAndWait(() -> frameContentRef[0].removeNotify());
    }

    @Test
    void toolbarHoldsTheUpdateControlsAndOptionsAboveTheActivityLog() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            DevicePane pane = createPane();
            List<Component> all = UiUtils.getAllComponents((JComponent) pane.getContent());

            JSplitButton update = (JSplitButton) all.stream().filter(c -> c instanceof JSplitButton)
                .findFirst().orElseThrow(AssertionError::new);
            JButton primary = (JButton) update.getComponent(0);
            assertTrue(update.getPreferredSize().height >= primary.getFontMetrics(primary.getFont()).getHeight()
                + primary.getInsets().top + primary.getInsets().bottom,
                "primary action retains natural text height at the active font");
            assertTrue(SwingUtilities.getAncestorOfClass(FirmwareStatusSummaryPanel.class, update) != null,
                "the primary action belongs to the live ECU identity/status block");

            for (String option : new String[]{
                "Update this console software automatically when a newer version is available",
                "A tune backup is attempted before updating, but can fail. If it succeeds, this option controls restoring it afterwards. Check settings before using the ECU."
            }) {
                JCheckBox box = (JCheckBox) all.stream()
                    .filter(c -> c instanceof JCheckBox && option.equals(((JCheckBox) c).getToolTipText()))
                    .findFirst().orElseThrow(() -> new AssertionError(option));
                assertNotNull(box.getToolTipText(), option + " explains itself");
            }

            assertTrue(all.stream().anyMatch(c -> c instanceof JLabel && DevicePane.ACTIVITY.equals(((JLabel) c).getText())));
            assertTrue(all.stream().anyMatch(c -> c instanceof JLabel && ((JLabel) c).getText().equals("ECU firmware")));
            assertTrue(all.stream().anyMatch(c -> c instanceof JTextArea
                && ((JTextArea) c).getText().equals("Not connected")));
            assertTrue(all.stream().anyMatch(c -> c instanceof JTextArea && "—".equals(((JTextArea) c).getText())));
            assertTrue(all.stream().anyMatch(c -> c instanceof JTextArea && ((JTextArea) c).getText().startsWith("Disconnected")));
            JLabel help = (JLabel) all.stream().filter(c -> c instanceof JLabel
                && ((JLabel) c).getText().contains("How it works")).findFirst()
                .orElseThrow(() -> new AssertionError(all.stream().filter(c -> c instanceof JLabel)
                    .map(c -> ((JLabel) c).getText()).collect(java.util.stream.Collectors.joining(" | "))));
            assertNotNull(SwingUtilities.getAncestorOfClass(FirmwareUpdateCard.class, help));
            assertTrue(help.getPreferredSize().width > 0, "help link has a visible size");
            JButton copy = (JButton) all.stream()
                .filter(c -> c instanceof JButton && DevicePane.COPY_LOG.equals(((JButton) c).getText()))
                .findFirst().orElseThrow(AssertionError::new);
            assertTrue(copy.getParent() instanceof JoinedButtons);
        });
    }

    @Test
    void startupAndDeviceOptionsAndSharedHintFitNarrowHighFontLayouts() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            JComponent device = (JComponent) createPane().getContent();
            List<Component> deviceComponents = UiUtils.getAllComponents(device);
            deviceComponents.stream().filter(c -> c instanceof JTextArea
                && !((JTextArea) c).getText().equals("—")).map(c -> (JTextArea) c).findFirst()
                .orElseThrow(AssertionError::new)
                .setText("Couldn't check for updates. Reconnect the ECU and try again.");
            for (Component component : deviceComponents) {
                if (component instanceof JComponent) {
                    ((JComponent) component).setFont(component.getFont().deriveFont(20f));
                }
            }
            device.setSize(480, 1400);
            layoutRecursively(device);
            layoutRecursively(device);
            JCheckBox[] options = deviceComponents.stream().filter(c -> c instanceof JCheckBox)
                .map(c -> (JCheckBox) c).toArray(JCheckBox[]::new);
            assertEquals(2, options.length, "the tune option and console software option each appear once");
            for (JCheckBox option : options) {
                assertTrue(option.getHeight() >= option.getPreferredSize().height,
                    "wrapped Device checkbox is not clipped: " + option.getToolTipText());
                if (option.getToolTipText().startsWith("Update this console")) {
                    assertEquals("Auto-update console software", visibleCheckboxText(option),
                        "software option keeps its full label when the available width requires wrapping");
                }
                assertTrue(option.getX() >= 0 && option.getY() >= 0
                    && option.getX() + option.getWidth() <= option.getParent().getWidth()
                    && option.getY() + option.getHeight() <= option.getParent().getHeight(),
                    "Device option bounds fit: " + option.getToolTipText());
            }
            assertTrue(java.util.Arrays.stream(options).anyMatch(c -> "Auto-update console software".equals(visibleCheckboxText(c))));
            assertTrue(deviceComponents.stream().anyMatch(c -> c instanceof JCheckBox
                && "Keep my current tune".equals(visibleCheckboxText((JCheckBox) c))));
            JLabel help = (JLabel) deviceComponents.stream().filter(c -> c instanceof JLabel
                && ((JLabel) c).getText().contains("How it works")).findFirst().orElseThrow(AssertionError::new);
            assertTrue(help.getWidth() > 0, "help link remains visible at narrow width");
            assertTrue(help.getX() >= 0 && help.getX() + help.getWidth() <= help.getParent().getWidth(),
                "separate help row stays inside the card");
            JTextArea hint = (JTextArea) deviceComponents.stream()
                .filter(c -> c instanceof JTextArea && ((JTextArea) c).getText().contains("Reconnect the ECU"))
                .findFirst().orElseThrow(AssertionError::new);
            assertTrue(hint.getWidth() <= 480);
            assertTrue(hint.getX() >= 0 && hint.getX() + hint.getWidth() <= hint.getParent().getWidth(),
                "shared hint stays inside the summary panel");
            try {
                assertTrue(hint.modelToView2D(hint.getDocument().getLength()).getMaxY() <= hint.getHeight(),
                    "release status wraps instead of clipping");
            } catch (javax.swing.text.BadLocationException e) {
                throw new AssertionError(e);
            }

            JLabel startupLabel = new JLabel("ECU firmware options:");
            JButton switchButton = new JButton("Switch firmware branch");
            JButton rollbackButton = new JButton("Roll back firmware");
            JCheckBox migrate = StartupUpdateActions.createMigrateSettingsCheckBox();
            JPanel startupOptions = StartupFrame.stackControls(startupLabel, switchButton, rollbackButton, migrate);
            for (Component component : UiUtils.getAllComponents(startupOptions)) {
                if (component instanceof JComponent) {
                    ((JComponent) component).setFont(component.getFont().deriveFont(20f));
                }
            }
            startupOptions.setSize(480, 700);
            layoutRecursively(startupOptions);
            assertChildrenFit(startupOptions);
            assertTrue(migrate.getHeight() >= migrate.getPreferredSize().height,
                "startup tune control fits at high font size");
            assertEquals("Keep my current tune", visibleCheckboxText(migrate));
            assertFalse(migrate.getText().contains("<html>"), "wide tune checkbox is not forced into HTML lines");
        });
    }

    private static String visibleCheckboxText(JCheckBox checkBox) {
        return checkBox.getText().replace("<html>", "").replace("</html>", "")
            .replace("<br>", " ").replace("<br/>", " ");
    }

    private static void assertChildrenFit(Container parent) {
        for (Component child : parent.getComponents()) {
            assertTrue(child.getX() >= 0 && child.getY() >= 0
                && child.getX() + child.getWidth() <= parent.getWidth()
                && child.getY() + child.getHeight() <= parent.getHeight(),
                child.getClass().getSimpleName() + " bounds " + child.getBounds() + " fit in "
                    + parent.getClass().getSimpleName() + " " + parent.getBounds());
            if (child instanceof Container) {
                assertChildrenFit((Container) child);
            }
        }
    }

    @Test
    void firmwareSummaryAlignmentCharacterization() throws Throwable {
        UIContext uiContext = new UIContext();
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        protocol.signature = "rusEFI older-stable.2026.11.13.uaefi_pro.123456";
        uiContext.getLinkManager().setBinaryProtocolForTests(protocol);
        DevicePane[] pane = new DevicePane[1];
        SwingUtilities.invokeAndWait(() -> {
            pane[0] = createPane(uiContext);
            pane[0].render(SessionState.CONNECTED, null);
            JComponent content = (JComponent) pane[0].getContent();
            for (Component component : UiUtils.getAllComponents(content)) {
                if (component instanceof JComponent && component.getFont() != null) {
                    JComponent swingComponent = (JComponent) component;
                    swingComponent.setFont(swingComponent.getFont().deriveFont(
                        swingComponent.getFont().getSize2D() * 2));
                }
            }
            content.setSize(480, 1400);
            layoutRecursively(content);
            layoutRecursively(content);

            FirmwareStatusSummaryPanel summary = (FirmwareStatusSummaryPanel) UiUtils.getAllComponents(content).stream()
                .filter(component -> component instanceof FirmwareStatusSummaryPanel)
                .findFirst().orElseThrow(AssertionError::new);
            JTextArea status = (JTextArea) java.util.Arrays.stream(summary.getComponents())
                .filter(component -> component instanceof JTextArea)
                .findFirst().orElseThrow(AssertionError::new);
            JTextArea ecu = (JTextArea) UiUtils.getAllComponents(summary).stream()
                .filter(component -> component instanceof JTextArea
                    && ((JTextArea) component).getText().startsWith("uaefi_pro"))
                .findFirst().orElseThrow(AssertionError::new);
            JLabel installed = (JLabel) UiUtils.getAllComponents(summary).stream()
                .filter(component -> component instanceof JLabel
                    && ((JLabel) component).getText().equals("Installed firmware"))
                .findFirst().orElseThrow(AssertionError::new);
            JCheckBox keepTune = (JCheckBox) UiUtils.getAllComponents(summary).stream()
                .filter(component -> component instanceof JCheckBox
                    && ((JCheckBox) component).getText().equals("Keep my current tune"))
                .findFirst().orElseThrow(AssertionError::new);
            JPanel dateRows = (JPanel) installed.getParent().getParent();

            int datesX = SwingUtilities.convertPoint(dateRows, 0, 0, summary).x;
            int statusX = SwingUtilities.convertPoint(status, 0, 0, summary).x;
            int ecuX = SwingUtilities.convertPoint(ecu.getParent(), 0, 0, summary).x;
            int tuneX = SwingUtilities.convertPoint(keepTune, 0, 0, summary).x;
            assertEquals(datesX, statusX, "status starts at the same left edge as firmware dates");
            assertEquals(datesX, ecuX, "ECU row starts at the same left edge as firmware dates");
            assertEquals(datesX, tuneX, "tune option starts at the same left edge as firmware dates");
            assertTrue(keepTune.getWidth() >= keepTune.getPreferredSize().width,
                "full tune option text fits at 2× font size");
            assertTrue(ecu.getWidth() >= ecu.getFontMetrics(ecu.getFont()).stringWidth(ecu.getText()),
                "full ECU identity fits the value column at 2× font size; value="
                    + ecu.getBounds() + ", textWidth="
                    + ecu.getFontMetrics(ecu.getFont()).stringWidth(ecu.getText()));
        });
    }

    /** The primary action stays with the summary inside the centered responsive firmware card. */
    @Test
    void updateButtonIsGroupedWithTheSummaryInsteadOfFloatingInTheToolbar() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            JComponent content = (JComponent) createPane().getContent();
            List<Component> all = UiUtils.getAllComponents(content);
            JSplitButton update = (JSplitButton) all.stream().filter(c -> c instanceof JSplitButton)
                .findFirst().orElseThrow(AssertionError::new);
            FirmwareStatusSummaryPanel summary = (FirmwareStatusSummaryPanel)
                SwingUtilities.getAncestorOfClass(FirmwareStatusSummaryPanel.class, update);
            assertNotNull(summary);
            FirmwareUpdateCard card = (FirmwareUpdateCard) SwingUtilities.getAncestorOfClass(FirmwareUpdateCard.class, update);
            assertNotNull(card);
            JScrollPane scroll = (JScrollPane) all.stream().filter(c -> c instanceof JScrollPane)
                .findFirst().orElseThrow(AssertionError::new);
            for (int width : new int[]{1200, 480}) {
                content.setSize(width, 800);
                layoutRecursively(content);
                assertTrue(Math.abs(2 * card.getX() + card.getWidth() - scroll.getViewport().getWidth()) <= 2,
                    "outer card is horizontally centered at width " + width);
                assertTrue(update.getPreferredSize().height >=
                    ((JButton) update.getComponent(0)).getFontMetrics(
                        ((JButton) update.getComponent(0)).getFont()).getHeight()
                        + ((JButton) update.getComponent(0)).getInsets().top
                        + ((JButton) update.getComponent(0)).getInsets().bottom,
                    "primary action fits the active font at width " + width);
                JTextArea ecu = (JTextArea) UiUtils.getAllComponents(summary).stream()
                    .filter(c -> c instanceof JTextArea && ((JTextArea) c).getText().equals("Not connected"))
                    .findFirst().orElseThrow(AssertionError::new);
                JTextArea branch = (JTextArea) UiUtils.getAllComponents(summary).stream()
                    .filter(c -> c instanceof JTextArea && "—".equals(((JTextArea) c).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                int ecuY = SwingUtilities.convertPoint(ecu, 0, 0, summary).y;
                int updateY = SwingUtilities.convertPoint(update, 0, 0, summary).y;
                assertTrue(ecuY < updateY, "identity leads the update action");
                assertTrue(branch.getY() > ecu.getY(), "branch has its own identity row");
            }
            assertEquals(new java.awt.Color(0x16A34A), getUploadColour());
        });
    }

    @Test
    void responsiveDeviceCardContainsAllFirmwareControlsAndProgress() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            JComponent content = (JComponent) createPane().getContent();
            for (int width : new int[]{1600, 2000, 480}) {
                content.setSize(width, 900);
                layoutRecursively(content);
                List<Component> all = UiUtils.getAllComponents(content);
                List<FirmwareUpdateCard> cards = all.stream().filter(c -> c instanceof FirmwareUpdateCard)
                    .map(c -> (FirmwareUpdateCard) c).collect(java.util.stream.Collectors.toList());
                assertEquals(1, cards.size(), "one outer firmware card");
                FirmwareUpdateCard card = cards.get(0);
                JScrollPane scroll = (JScrollPane) all.stream().filter(c -> c instanceof JScrollPane)
                    .findFirst().orElseThrow(AssertionError::new);
                int viewportWidth = scroll.getViewport().getWidth();
                double cardRatio = (double) card.getWidth() / viewportWidth;
                if (viewportWidth >= 1400) {
                    assertTrue(Math.abs(card.getWidth() - viewportWidth / 2) <= 2,
                        "large viewport uses half width: " + card.getWidth() + "/" + viewportWidth);
                } else if (viewportWidth >= 900) {
                    assertTrue(Math.abs(card.getWidth() - viewportWidth * 0.75) <= 2,
                        "medium viewport uses three quarters width: " + card.getWidth() + "/" + viewportWidth);
                } else {
                    assertTrue(cardRatio >= 0.90 && cardRatio < 1.0,
                        "small viewport uses full available width with an outer gutter: " + cardRatio);
                }
                for (Component component : all) {
                    if (component instanceof JSplitButton || component instanceof JCheckBox
                        || component instanceof JProgressBar || component instanceof JTextArea
                        || component instanceof JButton && DevicePane.COPY_LOG.equals(((JButton) component).getText())) {
                        assertSame(card, SwingUtilities.getAncestorOfClass(FirmwareUpdateCard.class, component),
                            component.getClass().getSimpleName() + " belongs to the single card");
                    }
                }
            }

            JLabel activity = (JLabel) UiUtils.getAllComponents(content).stream().filter(c -> c instanceof JLabel
                && DevicePane.ACTIVITY.equals(((JLabel) c).getText())).findFirst().orElseThrow(AssertionError::new);
            assertFalse(activity.getParent().getParent().isVisible(),
                "activity stays hidden until an operation starts");
        });
    }

    @Test
    void liveFirmwareSummaryAndPrimaryLabelFollowConnectedBranchAndDisconnect() throws Throwable {
        UIContext uiContext = new UIContext();
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        protocol.signature = "rusEFI master.2026.11.13.test_board.123456";
        uiContext.getLinkManager().setBinaryProtocolForTests(protocol);
        DevicePane[] pane = new DevicePane[1];
        SwingUtilities.invokeAndWait(() -> {
            pane[0] = createPane(uiContext);
            pane[0].render(SessionState.CONNECTED, null);
        });

        assertSummaryAndAction("test_board", "Dev", "Update ECU · Dev", pane[0]);

        protocol.signature = "rusEFI older-stable.2026.11.13.test_board.abcdef";
        SwingUtilities.invokeAndWait(() -> pane[0].render(SessionState.CONNECTED, null));
        assertSummaryAndAction("test_board", "older-stable", "Update ECU · older-stable", pane[0]);

        protocol.signature = null;
        SwingUtilities.invokeAndWait(() -> pane[0].render(SessionState.DISCONNECTED, null));
        assertSummaryAndAction("Not connected", "—", "Update ECU firmware", pane[0]);
    }

    @Test
    void offlineOpenBltSummaryShowsBootloaderIdentityAndManualAction() throws Throwable {
        PortResult bootloader = new PortResult("COM8", SerialPortType.OpenBlt, null,
            new OpenbltInfo(true, "rusefi.uaefi_pro", "1.16.0", List.of()));
        AvailableHardware hardware = new AvailableHardware(List.of(bootloader), false, false, false);
        DevicePane[] pane = new DevicePane[1];
        boolean previousMigrationSetting = com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded;
        try {
            SwingUtilities.invokeAndWait(() -> {
                pane[0] = createPane(new UIContext(), List.of(bootloader));
                pane[0].render(SessionState.DEVICE_IN_BLT, hardware);
            });

            SwingUtilities.invokeAndWait(() -> {
                List<Component> all = UiUtils.getAllComponents((JComponent) pane[0].getContent());
                assertTrue(all.stream().anyMatch(component -> component instanceof JTextArea
                    && "OpenBLT bootloader v1.16.0 — uaefi_pro (COM8)"
                        .equals(((JTextArea) component).getText())));
                assertTrue(all.stream().filter(component -> component instanceof JTextArea)
                    .map(component -> ((JTextArea) component).getText())
                    .anyMatch(text -> text.contains("Manual OpenBLT Update is available")));
                assertTrue(all.stream().filter(component -> component instanceof JTextArea)
                    .map(component -> ((JTextArea) component).getText())
                    .anyMatch(text -> text.contains("Manual updates may restore a tune backed up earlier in this session")));
                assertEquals(2, all.stream().filter(component -> component instanceof JLabel
                    && "Not available in bootloader mode".equals(((JLabel) component).getText())).count());
                JCheckBox keepTune = (JCheckBox) all.stream().filter(component -> component instanceof JCheckBox
                    && "Keep my current tune".equals(((JCheckBox) component).getText()))
                    .findFirst().orElseThrow(AssertionError::new);
                assertTrue(keepTune.isSelected());
                assertFalse(keepTune.isEnabled());
                assertTrue(keepTune.getToolTipText().contains("independently of this option"));
                JSplitButton update = (JSplitButton) all.stream().filter(component -> component instanceof JSplitButton)
                    .findFirst().orElseThrow(AssertionError::new);
                assertEquals("Manual OpenBLT Update", ((JButton) update.getComponent(0)).getText());
                assertTrue(((JButton) update.getComponent(0)).isEnabled());
            });
        } finally {
            SwingUtilities.invokeAndWait(() ->
                com.rusefi.ui.basic.MigrateSettingsCheckboxState.isMigrationNeeded = previousMigrationSetting);
        }
    }

    private static void assertSummaryAndAction(String summary, String branch, String action, DevicePane pane) throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            List<Component> all = UiUtils.getAllComponents((JComponent) pane.getContent());
            assertTrue(all.stream().anyMatch(c -> c instanceof JTextArea
                && summary.equals(((JTextArea) c).getText())), summary);
            assertTrue(all.stream().anyMatch(c -> c instanceof JTextArea
                && branch.equals(((JTextArea) c).getText())), branch);
            JSplitButton update = (JSplitButton) all.stream().filter(c -> c instanceof JSplitButton)
                .findFirst().orElseThrow(AssertionError::new);
            assertEquals(action, ((JButton) update.getComponent(0)).getText());
        });
    }

    @Test
    void idleActivitySectionIsHiddenUntilAnOperationStarts() throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            JComponent content = (JComponent) createPane().getContent();
            List<Component> all = UiUtils.getAllComponents(content);
            JLabel activity = (JLabel) all.stream().filter(c -> c instanceof JLabel
                && DevicePane.ACTIVITY.equals(((JLabel) c).getText())).findFirst().orElseThrow(AssertionError::new);
            assertFalse(activity.getParent().getParent().isVisible(),
                "activity heading and copy control stay hidden while idle");
            JProgressBar progress = (JProgressBar) all.stream().filter(c -> c instanceof JProgressBar)
                .findFirst().orElseThrow(AssertionError::new);
            assertFalse(progress.isShowing(), "empty Ready/0% progress is not shown before work starts");
        });
    }

    private static Component statusLayoutCenter(JComponent content) {
        List<Component> all = UiUtils.getAllComponents(content);
        JProgressBar progress = (JProgressBar) all.stream().filter(c -> c instanceof JProgressBar)
            .findFirst().orElseThrow(AssertionError::new);
        Container progressRow = progress.getParent();
        Container statusContent = progressRow.getParent();
        return ((java.awt.BorderLayout) statusContent.getLayout()).getLayoutComponent(java.awt.BorderLayout.CENTER);
    }

    private static void layoutRecursively(Container container) {
        container.invalidate();
        if (container.getLayout() instanceof java.awt.LayoutManager2) {
            ((java.awt.LayoutManager2) container.getLayout()).invalidateLayout(container);
        }
        container.doLayout();
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                layoutRecursively((Container) component);
            }
        }
    }

    private static void invalidateLayoutRecursively(Container container) {
        if (container.getLayout() instanceof java.awt.LayoutManager2) {
            ((java.awt.LayoutManager2) container.getLayout()).invalidateLayout(container);
        }
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                invalidateLayoutRecursively((Container) component);
            }
        }
    }

    /** The update icon keeps the earlier upload arrow's green. */
    private static java.awt.Color getUploadColour() {
        java.awt.image.BufferedImage image = new java.awt.image.BufferedImage(16, 16, java.awt.image.BufferedImage.TYPE_INT_ARGB);
        java.awt.Graphics2D g = image.createGraphics();
        com.rusefi.ui.widgets.CommandIcon.UPLOAD.paintIcon(null, g, 0, 0);
        g.dispose();
        return new java.awt.Color(image.getRGB(8, 10), true);
    }
}
