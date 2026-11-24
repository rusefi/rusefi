package com.rusefi.ui.basic;

import com.rusefi.core.io.BundleInfo;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.io.LinkManager;
import com.rusefi.ConnectivityContext;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.autoupdate.FirmwareRollbackResolver;
import com.rusefi.autoupdate.Autoupdate.DevFirmwareMetadataFailure;
import org.junit.jupiter.api.Test;

import java.util.Optional;
import java.util.Properties;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;
import java.time.LocalDate;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;
import javax.swing.SwingUtilities;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JProgressBar;
import javax.swing.JLabel;
import javax.swing.JPanel;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.mockito.Mockito.*;

class FirmwareRollbackControllerTest {
    @Test
    void acceptedLtsUpdateRevealsDownloadProgressBeforeDownloaderReportsProgress() throws Exception {
        String prior = System.getProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "true");
        CountDownLatch catalogReady = new CountDownLatch(1);
        CountDownLatch downloaderStarted = new CountDownLatch(1);
        CountDownLatch releaseDownloader = new CountDownLatch(1);
        CountDownLatch downloadFinished = new CountDownLatch(1);
        CountDownLatch downloadSettled = new CountDownLatch(1);
        AtomicReference<FirmwareRollbackController> controllerRef = new AtomicReference<>();
        AtomicReference<StatusPanelWithProgressBar> callbacksRef = new AtomicReference<>();
        String oldSha = "1111111111111111111111111111111111111111";
        String latestSha = "2222222222222222222222222222222222222222";

        try {
            PortResult port = spy(new PortResult("TEST_PORT", SerialPortType.Ecu));
            doReturn(Optional.of(oldSha)).when(port).getFirmwareHash();
            LinkManager lm = mock(LinkManager.class);
            BinaryProtocol bp = mock(BinaryProtocol.class);
            bp.signature = "rusEFI lts-26lima.2026.11.10.test_board.live";
            when(lm.getBinaryProtocol()).thenReturn(bp);
            when(lm.getLastTriedPort()).thenReturn(port.port);

            SwingUtilities.invokeAndWait(() -> {
                StatusPanelWithProgressBar callbacks = spy(new StatusPanelWithProgressBar());
                callbacksRef.set(callbacks);
                FirmwareRollbackController controller = new FirmwareRollbackController(
                    mock(ConnectivityContext.class), callbacks,
                    new SingleAsyncJobExecutor(callbacks), () -> Optional.of(port), () -> true,
                    () -> {
                        FirmwareRollbackController current = controllerRef.get();
                        if (current != null && !current.isBusy()) {
                            catalogReady.countDown();
                            if (downloaderStarted.getCount() == 0) {
                                downloadSettled.countDown();
                            }
                        }
                    }, ignored -> {}, () -> {});
                controllerRef.set(controller);
                controller.setLinkManager(lm);
                controller.setCatalogLoaderForTests((board, branch) ->
                    catalog(board, branch, oldSha, "2026-11-10 10:00", latestSha, "2026-11-13 10:00"));
                controller.setFirmwareDownloaderForTests((build, progress, logger) -> {
                    downloaderStarted.countDown();
                    try {
                        if (!releaseDownloader.await(5, TimeUnit.SECONDS)) {
                            throw new IllegalStateException("test downloader release timed out");
                        }
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        throw new IllegalStateException(e);
                    } finally {
                        downloadFinished.countDown();
                    }
                    return Optional.empty();
                });
                controller.refresh(port);
            });
            assertTrue(catalogReady.await(5, TimeUnit.SECONDS), "catalog-backed update candidates become ready");

            SwingUtilities.invokeAndWait(() -> assertTrue(
                controllerRef.get().startLatestUpdate(new JPanel(), port),
                "the accepted latest-update request enters the shared firmware job path"));
            assertTrue(downloaderStarted.await(5, TimeUnit.SECONDS), "fake downloader blocks before any progress");
            SwingUtilities.invokeAndWait(() -> {
                assertTrue(callbacksRef.get().isActivityVisible(),
                    "accepted catalog-backed work reveals Activity before the downloader can report progress");
                verify(callbacksRef.get())
                    .updateFirmwarePhase(UpdateOperationCallbacks.FirmwareUpdateStage.DOWNLOAD);
                verify(callbacksRef.get()).updateProgress(0);
            });
        } finally {
            releaseDownloader.countDown();
            if (downloaderStarted.getCount() == 0) {
                assertTrue(downloadFinished.await(5, TimeUnit.SECONDS));
                assertTrue(downloadSettled.await(5, TimeUnit.SECONDS));
            }
            restoreRollbackEnabledProperty(prior);
        }
    }

    @Test
    void firmwareSummaryPreservesTheOwningTuneCheckboxSelection() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean selected : new boolean[]{false, true}) {
                ProgramSelector selector = mock(ProgramSelector.class);
                when(selector.getControl()).thenReturn(new JPanel());
                ConnectivityContext context = new ConnectivityContext(new com.rusefi.FakePortScanner());
                FirmwareRollbackController controller = new FirmwareRollbackController(
                    context, UpdateOperationCallbacks.DUMMY,
                    new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), Optional::empty,
                    () -> true, () -> {}, ignored -> {}, () -> {});
                JCheckBox keepTune = new JCheckBox("Keep my current tune");
                keepTune.setSelected(selected);

                new FirmwareStatusSummaryPanel(controller, selector, new JButton(), keepTune, context,
                    () -> null, () -> false);

                assertEquals(selected, keepTune.isSelected());
            }
        });
    }

    @Test
    void summaryRefreshDoesNotReenableTunePreservationDuringFlash() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ProgramSelector selector = mock(ProgramSelector.class);
            when(selector.getControl()).thenReturn(new JPanel());
            ConnectivityContext context = new ConnectivityContext(new com.rusefi.FakePortScanner());
            FirmwareRollbackController controller = new FirmwareRollbackController(
                context, UpdateOperationCallbacks.DUMMY,
                new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), Optional::empty,
                () -> true, () -> {}, ignored -> {}, () -> {});
            JCheckBox keepTune = new JCheckBox("Keep my current tune");
            keepTune.setSelected(true);
            AtomicBoolean flashing = new AtomicBoolean(true);

            FirmwareStatusSummaryPanel summary = new FirmwareStatusSummaryPanel(
                controller, selector, new JButton(), keepTune, context, () -> null, flashing::get);

            assertTrue(keepTune.isSelected());
            assertFalse(keepTune.isEnabled());
            summary.refresh();
            assertFalse(keepTune.isEnabled(), "periodic summary refresh must preserve the active-flash lock");

            flashing.set(false);
            summary.refresh();
            assertTrue(keepTune.isEnabled(), "the tune option returns after the busy state clears");
        });
    }

    @Test
    void reconnectingTheSameLimaEcuRediscoversItsHistory() throws Exception {
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "true");
        try {
            PortResult port = new PortResult("TEST_PORT", SerialPortType.Ecu);
            LinkManager lm = mock(LinkManager.class);
            BinaryProtocol bp = mock(BinaryProtocol.class);
            bp.signature = "rusEFI lts-26lima.2026.11.11.test_board.123";
            when(lm.getBinaryProtocol()).thenReturn(bp);
            when(lm.getLastTriedPort()).thenReturn(port.port);
            AtomicReference<FirmwareRollbackController> controller = new AtomicReference<>();
            Semaphore completed = new Semaphore(0);
            AtomicInteger discoveries = new AtomicInteger();
            SwingUtilities.invokeAndWait(() -> {
                FirmwareRollbackController value = new FirmwareRollbackController(
                    mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
                    new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY),
                    () -> Optional.of(port), () -> true,
                    () -> {
                        if (controller.get() != null && !controller.get().isBusy()) {
                            completed.release();
                        }
                    }, ignored -> {}, () -> {});
                controller.set(value);
                value.setCatalogLoaderForTests((board, branch) -> {
                    discoveries.incrementAndGet();
                    return Collections.emptyList();
                });
                value.setLinkManager(lm);
                value.refresh(port);
            });
            assertTrue(completed.tryAcquire(5, TimeUnit.SECONDS));
            assertEquals(1, discoveries.get());
            SwingUtilities.invokeAndWait(() -> {
                controller.get().setLinkManager(null);
                controller.get().refresh(port);
                controller.get().setLinkManager(lm);
                controller.get().refresh(port);
            });
            assertTrue(completed.tryAcquire(5, TimeUnit.SECONDS));
            assertEquals(2, discoveries.get());
        } finally {
            System.clearProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        }
    }

    @Test
    void historyUsesLiveEcuBranchAndDoesNotGuessWhenDisconnected() {
        BundleInfo lima = FirmwareRollbackController.connectedFirmware(
            "rusEFI lts-26lima.2026.11.11.test_board.123");
        assertEquals("lts-26lima", lima.getBranchName());
        assertEquals("test_board", lima.getTarget());
        assertTrue(FirmwareRollbackController.isRollbackButtonVisible(true, lima));
        assertFalse(FirmwareRollbackController.isRollbackButtonVisible(true,
            FirmwareRollbackController.connectedFirmware("rusEFI master.2026.11.11.test_board.456")));
        assertNull(FirmwareRollbackController.connectedFirmware(null));
        assertNull(FirmwareRollbackController.connectedFirmware("invalid"));
    }

    @Test
    void serialFirmwareUpdateRejectsSocketCanEcu() {
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult(LinkManager.SOCKET_CAN, SerialPortType.Ecu)));
        assertTrue(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult("COM7", SerialPortType.Ecu)));
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult("COM7", SerialPortType.OpenBlt)));
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult(LinkManager.PCAN, SerialPortType.Ecu)));
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult(LinkManager.PCAN, SerialPortType.EcuWithOpenblt)));
        assertFalse(FirmwareRollbackController.isSerialFirmwareUpdateSupported(
            new PortResult("SLCAN:COM7", SerialPortType.Ecu)));
    }

    @Test
    void latestUpdateCannotReplaceConfirmedBWithStaleProviderAOrAnotherConnection() {
        PortResult a = new PortResult("ECU_A", SerialPortType.Ecu);
        PortResult b = new PortResult("ECU_B", SerialPortType.Ecu);
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b, a, true, b.port),
            "startup's independent provider must not override the confirmed selection");
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b, b, true, a.port));
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b,
            new PortResult(b.port, SerialPortType.EcuWithOpenblt), true, b.port));
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b, null, true, b.port));
        assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(b, b, false, b.port));
        assertTrue(FirmwareRollbackController.matchesExpectedUpdateTarget(b,
            new PortResult(b.port, b.type), true, b.port));
        for (String transport : new String[]{LinkManager.PCAN, LinkManager.SOCKET_CAN, "SLCAN:COM7"}) {
            PortResult can = new PortResult(transport, SerialPortType.Ecu);
            assertFalse(FirmwareRollbackController.matchesExpectedUpdateTarget(can, can, true, can.port));
        }
    }

    @Test
    void enablesHistoryOnlyForLtsBundles() {
        BundleInfo lts = new BundleInfo("lts-26-test", null, "uaefi");
        BundleInfo development = new BundleInfo("development", null, "uaefi");
        BundleInfo release = new BundleInfo("release", null, "uaefi");
        assertTrue(FirmwareRollbackController.isLtsBundle(lts));
        assertFalse(FirmwareRollbackController.isLtsBundle(development));
        assertFalse(FirmwareRollbackController.isLtsBundle(release));
        assertTrue(FirmwareRollbackController.isRollbackButtonVisible(true, lts));
        assertFalse(FirmwareRollbackController.isRollbackButtonVisible(false, lts));
        assertFalse(FirmwareRollbackController.isRollbackButtonVisible(true, development));
    }

    @Test
    void rollbackFeatureIsDefaultOffAndBundleConfigurable() {
        Properties properties = new Properties();
        assertFalse(FirmwareRollbackController.isFirmwareRollbackEnabled(properties));

        properties.setProperty("firmware_rollback_enabled", "true");
        assertTrue(FirmwareRollbackController.isFirmwareRollbackEnabled(properties));
    }

    @Test
    void rollbackFeatureAllowsLocalJvmOverride() {
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "true");
        try {
            assertTrue(FirmwareRollbackController.isFirmwareRollbackEnabled(new Properties()));
        } finally {
            System.clearProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        }
    }

    @Test
    void successfulSessionFlashOverridesStaleScannedHash() {
        assertEquals("new-sha", FirmwareRollbackController.selectCurrentFirmwareSha(
            " new-sha ", Optional.of("old-sha"), "persisted-sha"));
        assertEquals("ecu-sha", FirmwareRollbackController.selectCurrentFirmwareSha(
            null, Optional.of(" ecu-sha "), "persisted-sha"));
        assertEquals("persisted-sha", FirmwareRollbackController.selectCurrentFirmwareSha(
            null, Optional.empty(), "persisted-sha"));
    }

    @Test
    void releaseStatusNeverInfersOrderingFromAnUnmatchedHash() {
        assertEquals("Couldn't check for updates. Try again later.",
            FirmwareRollbackController.releaseStatus("lts-26lima", false, false, false, false, false, false));
        assertEquals("Checking for updates…",
            FirmwareRollbackController.releaseStatus("lts-26lima", true, true, false, false, false, false));
        assertEquals("Firmware update information is unavailable.",
            FirmwareRollbackController.releaseStatus("lts-26lima", true, false, false, false, false, false));
        assertEquals("Couldn't check for updates. Try again later.",
            FirmwareRollbackController.releaseStatus("lts-26lima", true, false, true, false, false, false),
            "a hash not found for this board and branch is not evidence of an update");
        assertEquals("This ECU is up to date.",
            FirmwareRollbackController.releaseStatus("lts-26lima", true, false, true, true, true, false));
        assertEquals("Update available",
            FirmwareRollbackController.releaseStatus("lts-26lima", true, false, true, true, false, true));
        assertEquals("Couldn't compare firmware versions.",
            FirmwareRollbackController.releaseStatus("lts-26lima", true, false, true, true, false, false),
            "different SHAs without strictly newer catalog ordering do not prove a newer release");
        assertTrue(FirmwareRollbackController.sameReleaseIdentity(
            "test_board", "lts-26lima", "abc", "TEST_BOARD", "LTS-26LIMA", "ABC"));
        assertFalse(FirmwareRollbackController.sameReleaseIdentity(
            "test_board", "lts-26lima", "abc", "other_board", "lts-26lima", "def"));
        assertFalse(FirmwareRollbackController.sameReleaseIdentity(
            "test_board", "lts-26lima", "abc", "test_board", "lts-26kansas", "def"));
    }

    @Test
    void missingEcuHashCanBeCharacterizedByExactCatalogSignature() throws Exception {
        PortResult port = spy(new PortResult("TEST_PORT", SerialPortType.Ecu));
        doReturn(Optional.empty()).when(port).getFirmwareHash();
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        String oldSignature = "rusEFI lts-26lima.2026.10.16.uaefi_pro.1167482973";
        bp.signature = oldSignature;
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.getLastTriedPort()).thenReturn(port.port);
        CountDownLatch discovered = new CountDownLatch(1);
        AtomicReference<FirmwareRollbackController> controllerRef = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            FirmwareRollbackController controller = new FirmwareRollbackController(
                mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
                new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(port),
                () -> true, () -> {
                    FirmwareRollbackController current = controllerRef.get();
                    if (current != null && !current.isBusy()) {
                        discovered.countDown();
                    }
                }, ignored -> {}, () -> {});
            controllerRef.set(controller);
            controller.enableReleaseStatusDiscovery();
            controller.setLinkManager(lm);
            controller.setCatalogLoaderForTests((board, branch) -> FirmwareRollbackResolver.parseIndex(
                "<a href=\"1111111111111111111111111111111111111111/\" data-firmware-signature=\"" +
                    oldSignature + "\">old</a> 2026-10-16 10:00 -\n" +
                    "<a href=\"2222222222222222222222222222222222222222/\" data-firmware-signature=\"" +
                    "rusEFI lts-26lima.2026.11.13.uaefi_pro.2720682551" +
                    "\">new</a> 2026-11-13 10:00 -",
                board, branch, "https://example.test/build_server/" + board + "/" + branch + "/"));
            controller.refresh(port);
        });
        assertTrue(discovered.await(5, TimeUnit.SECONDS));
        assertEquals("Update available",
            controllerRef.get().getReleaseStatus(),
            "the ECU reports no hash but its exact full signature is listed");
    }

    @Test
    void signatureFallbackRejectsMissingDuplicateAndAmbiguousCatalogIdentity() throws Exception {
        String signature = "rusEFI lts-26lima.2026.10.16.uaefi_pro.1167482973";
        String older = "<a href=\"1111111111111111111111111111111111111111/\">old</a> 2026-10-16 10:00 -\n";
        String latest = "<a href=\"2222222222222222222222222222222222222222/\">new</a> 2026-11-13 10:00 -";
        assertEquals("Couldn't check for updates. Try again later.",
            signatureStatus(signature, "uaefi_pro", "lts-26lima", older + latest));
        String latestSignature = "rusEFI lts-26lima.2026.11.13.uaefi_pro.2720682551";
        assertEquals("This ECU is up to date.", signatureStatus(latestSignature,
            "uaefi_pro", "lts-26lima",
            "<a href=\"2222222222222222222222222222222222222222/\" data-firmware-signature=\"" +
                latestSignature + "\">latest</a> 2026-11-13 10:00 -"));
        assertEquals("Couldn't check for updates. Try again later.",
            signatureStatus(signature, "uaefi_pro", "lts-26lima", older +
                "<a href=\"3333333333333333333333333333333333333333/\" data-firmware-signature=\"" +
                signature + "\">old</a> 2026-10-17 10:00 -\n" +
                "<a href=\"4444444444444444444444444444444444444444/\" data-firmware-signature=\"" +
                signature + "\">duplicate</a> 2026-10-18 10:00 -\n" + latest));
        assertEquals("Couldn't compare firmware versions.", signatureStatus(signature, "uaefi_pro",
            "lts-26lima", "<a href=\"1111111111111111111111111111111111111111/\" data-firmware-signature=\"" +
                signature + "\">old</a> 2026-10-16 10:00 -\n" +
                "<a href=\"2222222222222222222222222222222222222222/\">new</a> 2026-11-13 10:00 -\n" +
                "<a href=\"3333333333333333333333333333333333333333/\">tie</a> 2026-11-13 10:00 -"));
        String signedOld = "<a href=\"1111111111111111111111111111111111111111/\" data-firmware-signature=\"" +
            signature + "\">old</a> 2026-10-16 10:00 -\n";
        String signedLatest = "<a href=\"2222222222222222222222222222222222222222/\" data-firmware-signature=\"" +
            "rusEFI lts-26lima.2026.11.13.uaefi_pro.2720682551" +
            "\">new</a> 2026-11-13 10:00 -";
        assertEquals("Couldn't check for updates. Try again later.",
            signatureStatus(signature, "other_board", "lts-26lima", signedOld + signedLatest));
        assertEquals("Couldn't check for updates. Try again later.",
            signatureStatus(signature, "uaefi_pro", "lts-26kansas", signedOld + signedLatest));
        assertEquals("Couldn't check for updates. Try again later.",
            signatureStatus(signature, "uaefi_pro", "lts-26lima", signedOld + signedLatest,
                Optional.of("5555555555555555555555555555555555555555")),
            "a present conflicting ECU SHA must not fall back to its matching signature");
    }

    @Test
    void ltsLatestDateRequiresUniqueCurrentBoardBranchSignatureButNotInstalledHash() throws Exception {
        LocalDate today = LocalDate.now(ZoneOffset.UTC);
        String installed = "rusEFI lts-26lima." + today.minusDays(2).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.installed";
        String validLatest = "rusEFI lts-26lima." + today.minusDays(1).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.latest";
        String oldEntry = signedIndexEntry("1111111111111111111111111111111111111111", installed, today.minusDays(2));
        String latestEntry = signedIndexEntry("2222222222222222222222222222222222222222",
            validLatest, today.minusDays(1));

        FirmwareRollbackController.ReleaseDisplay valid = ltsReleaseDisplay(installed, "uaefi_pro", "lts-26lima",
            oldEntry + latestEntry, Optional.empty());
        assertEquals(today.minusDays(1), valid.latestDate,
            "a valid latest build date remains available without a matching installed SHA");

        String wrongBoard = "rusEFI lts-26lima." + today.minusDays(1).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".other_board.latest";
        assertNull(ltsReleaseDisplay(installed, "uaefi_pro", "lts-26lima", oldEntry
            + signedIndexEntry("2222222222222222222222222222222222222222", wrongBoard, today.minusDays(1)),
            Optional.empty()).latestDate);

        String wrongBranch = "rusEFI lts-26kansas."
            + today.minusDays(1).format(DateTimeFormatter.ofPattern("yyyy.M.d")) + ".uaefi_pro.latest";
        assertNull(ltsReleaseDisplay(installed, "uaefi_pro", "lts-26lima", oldEntry
            + signedIndexEntry("2222222222222222222222222222222222222222", wrongBranch, today.minusDays(1)),
            Optional.empty()).latestDate);

        assertNull(ltsReleaseDisplay(installed, "uaefi_pro", "lts-26lima",
            oldEntry + latestEntry + signedIndexEntry("3333333333333333333333333333333333333333",
                validLatest, today.minusDays(1)), Optional.empty()).latestDate,
            "a tie in latest catalog ordering is ambiguous");
        assertNull(ltsReleaseDisplay(installed, "uaefi_pro", "lts-26lima", oldEntry
            + "<a href=\"2222222222222222222222222222222222222222/\">unsigned</a> "
            + today.minusDays(1).format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) + " 10:00 -",
            Optional.empty()).latestDate, "missing signature metadata is not a build date");
    }

    private static String signedIndexEntry(String sha, String signature, LocalDate indexDate) {
        return "<a href=\"" + sha + "/\" data-firmware-signature=\"" + signature
            + "\">build</a> " + indexDate.format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) + " 10:00 -\n";
    }

    private static FirmwareRollbackController.ReleaseDisplay ltsReleaseDisplay(
        String installedSignature, String board, String branch, String html, Optional<String> installedHash
    ) throws Exception {
        PortResult port = spy(new PortResult("TEST_PORT", SerialPortType.Ecu));
        doReturn(installedHash).when(port).getFirmwareHash();
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = installedSignature;
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.getLastTriedPort()).thenReturn(port.port);
        CountDownLatch discovered = new CountDownLatch(1);
        AtomicReference<FirmwareRollbackController> controllerRef = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            FirmwareRollbackController controller = new FirmwareRollbackController(
                mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
                new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(port),
                () -> true, () -> {
                    FirmwareRollbackController current = controllerRef.get();
                    if (current != null && !current.isBusy()) {
                        discovered.countDown();
                    }
                }, ignored -> {}, () -> {});
            controllerRef.set(controller);
            controller.enableReleaseStatusDiscovery();
            controller.setLinkManager(lm);
            controller.setCatalogLoaderForTests((catalogBoard, catalogBranch) ->
                FirmwareRollbackResolver.parseIndex(html, catalogBoard, catalogBranch,
                    "https://example.test/build_server/" + catalogBoard + "/" + catalogBranch + "/"));
            controller.refresh(port);
        });
        assertTrue(discovered.await(5, TimeUnit.SECONDS));
        return controllerRef.get().getReleaseDisplay();
    }

    private static String signatureStatus(String signature, String catalogBoard, String catalogBranch,
                                          String html) throws Exception {
        return signatureStatus(signature, catalogBoard, catalogBranch, html, Optional.empty());
    }

    private static String signatureStatus(String signature, String catalogBoard, String catalogBranch,
                                          String html, Optional<String> ecuHash) throws Exception {
        PortResult port = spy(new PortResult("TEST_PORT", SerialPortType.Ecu));
        doReturn(ecuHash).when(port).getFirmwareHash();
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = signature;
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.getLastTriedPort()).thenReturn(port.port);
        CountDownLatch discovered = new CountDownLatch(1);
        AtomicReference<FirmwareRollbackController> controllerRef = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            FirmwareRollbackController controller = new FirmwareRollbackController(
                mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
                new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(port),
                () -> true, () -> {
                    FirmwareRollbackController current = controllerRef.get();
                    if (current != null && !current.isBusy()) {
                        discovered.countDown();
                    }
                }, ignored -> {}, () -> {});
            controllerRef.set(controller);
            controller.enableReleaseStatusDiscovery();
            controller.setLinkManager(lm);
            controller.setCatalogLoaderForTests((board, branch) -> FirmwareRollbackResolver.parseIndex(
                html, catalogBoard, catalogBranch,
                "https://example.test/build_server/" + catalogBoard + "/" + catalogBranch + "/"));
            controller.refresh(port);
        });
        assertTrue(discovered.await(5, TimeUnit.SECONDS));
        return controllerRef.get().getReleaseStatus();
    }

    @Test
    void developmentReleaseStatusUsesBoardDailyMetadataAndKeepsSameDayNeutral() throws Exception {
        PortResult port = new PortResult("ECU_A", SerialPortType.Ecu);
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        LocalDate today = LocalDate.now(ZoneOffset.UTC);
        bp.signature = "rusEFI master." + today.minusDays(2).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.live";
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.getLastTriedPort()).thenReturn(port.port);
        AtomicReference<FirmwareRollbackController> controllerRef = new AtomicReference<>();
        CountDownLatch firstComplete = new CountDownLatch(1);
        CountDownLatch secondComplete = new CountDownLatch(1);
        CountDownLatch thirdComplete = new CountDownLatch(1);
        CountDownLatch fourthComplete = new CountDownLatch(1);
        CountDownLatch fifthComplete = new CountDownLatch(1);
        CountDownLatch sixthComplete = new CountDownLatch(1);
        AtomicInteger lookups = new AtomicInteger();
        FirmwareRollbackController controller = new FirmwareRollbackController(
            mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
            new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(port),
            () -> true, () -> {}, ignored -> {}, () -> {});
        controllerRef.set(controller);
        controller.enableReleaseStatusDiscovery();
        controller.setLinkManager(lm);
        controller.setDevMetadataLoaderForTests((board, branch) -> {
            assertEquals("uaefi_pro", board);
            assertEquals(BundleInfo.DEVELOPMENT, branch);
            int lookup = lookups.incrementAndGet();
            if (lookup == 6) {
                return 0;
            }
            if (lookup == 5) {
                return today.plusDays(1).atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli();
            }
            return today.atTime(12, 0).toInstant(ZoneOffset.UTC).toEpochMilli();
        });
        controller.addStateChangedListener(() -> {
            if (!controllerRef.get().getReleaseStatus().startsWith("Checking")) {
                if (lookups.get() == 1) {
                    firstComplete.countDown();
                } else if (lookups.get() == 2) {
                    secondComplete.countDown();
                } else if (lookups.get() == 3) {
                    thirdComplete.countDown();
                } else if (lookups.get() == 4) {
                    fourthComplete.countDown();
                } else if (lookups.get() == 5) {
                    fifthComplete.countDown();
                } else if (lookups.get() == 6) {
                    sixthComplete.countDown();
                }
            }
        });
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(firstComplete.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals("Update available", controller.getReleaseStatus());
        assertEquals(today, controller.getReleaseDisplay().latestDate,
            "valid Dev metadata displays its date even when the installed hash is unknown");

        bp.signature = "rusEFI development." + today.format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.live";
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(secondComplete.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals("No newer build date found.", controller.getReleaseStatus());
        assertEquals(2, lookups.get(), "master/development aliases use the same board-specific metadata route");

        bp.signature = "rusEFI master." + today.plusDays(1).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.live";
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(thirdComplete.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals("Couldn't compare firmware versions.", controller.getReleaseStatus());

        bp.signature = "rusEFI master.2026.13.1.uaefi_pro.live";
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(fourthComplete.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals("Couldn't compare firmware versions.", controller.getReleaseStatus());

        bp.signature = "rusEFI master." + today.minusDays(1).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.live";
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(fifthComplete.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals("Couldn't compare firmware versions.", controller.getReleaseStatus(),
            "a future published bundle date is not an update claim");

        bp.signature = "rusEFI master." + today.minusDays(2).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.live";
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(sixthComplete.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals("Couldn't compare versions because the latest firmware has no publication date.",
            controller.getReleaseStatus(),
            "a missing Last-Modified value is not availability evidence");

        bp.signature = "rusEFI release.2026.11.12.uaefi_pro.live";
        assertEquals("Couldn't check for updates. Try again later.", controller.getReleaseStatus());
    }

    @Test
    void developmentMetadataFailuresExplainWhatCouldNotBeCheckedWithoutClaimingFirmwareStatus() {
        assertEquals("Couldn't check for updates. No firmware download was found for this ECU.",
            FirmwareRollbackController.developmentMetadataFailureStatus(
                DevFirmwareMetadataFailure.DOWNLOAD_NOT_FOUND));
        assertEquals("Couldn't compare versions because the latest firmware has no publication date.",
            FirmwareRollbackController.developmentMetadataFailureStatus(
                DevFirmwareMetadataFailure.NO_PUBLICATION_DATE));
        assertEquals("Couldn't check for updates. Try again later.",
            FirmwareRollbackController.developmentMetadataFailureStatus(
                DevFirmwareMetadataFailure.INVALID_SERVER_RESPONSE));
        assertEquals("Couldn't reach the update server. Check your network and try again.",
            FirmwareRollbackController.developmentMetadataFailureStatus(
                DevFirmwareMetadataFailure.CONNECTION_FAILED));
    }

    @Test
    void devMetadataFailureAndStaleBoardResponseStayUnknownOrBoundToCurrentEcu() throws Exception {
        PortResult port = new PortResult("ECU_A", SerialPortType.Ecu);
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        LocalDate today = LocalDate.now(ZoneOffset.UTC);
        bp.signature = "rusEFI master." + today.minusDays(2).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".board_a.live";
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.getLastTriedPort()).thenReturn(port.port);
        FirmwareRollbackController controller = new FirmwareRollbackController(
            mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
            new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(port),
            () -> true, () -> {}, ignored -> {}, () -> {});
        controller.enableReleaseStatusDiscovery();
        controller.setLinkManager(lm);
        CountDownLatch firstStarted = new CountDownLatch(1);
        CountDownLatch releaseFirst = new CountDownLatch(1);
        CountDownLatch boardBCompleted = new CountDownLatch(1);
        CountDownLatch staleResponseApplied = new CountDownLatch(1);
        AtomicInteger calls = new AtomicInteger();
        controller.addStateChangedListener(() -> {
            if (releaseFirst.getCount() == 0 && calls.get() >= 2
                && !controller.getReleaseStatus().startsWith("Checking")) {
                staleResponseApplied.countDown();
            }
        });
        controller.setDevMetadataLoaderForTests((board, branch) -> {
            if (calls.incrementAndGet() == 1) {
                firstStarted.countDown();
                try {
                    if (!releaseFirst.await(5, TimeUnit.SECONDS)) {
                        throw new java.io.IOException("test metadata release timed out");
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    throw new java.io.IOException(e);
                }
                return today.minusDays(1).atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli();
            }
            assertEquals("board_b", board);
            boardBCompleted.countDown();
            return today.atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli();
        });
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(firstStarted.await(5, TimeUnit.SECONDS));

        bp.signature = "rusEFI development." + today.minusDays(2).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".board_b.live";
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(boardBCompleted.await(5, TimeUnit.SECONDS));
        awaitStatusSettled(controller);
        assertEquals("Update available", controller.getReleaseStatus());
        assertEquals(today, controller.getReleaseDisplay().latestDate);

        releaseFirst.countDown();
        assertTrue(staleResponseApplied.await(5, TimeUnit.SECONDS));
        assertEquals("Update available", controller.getReleaseStatus(),
            "late metadata for board_a must not overwrite board_b status");

        bp.signature = "rusEFI master." + today.minusDays(3).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".board_a.live";
        assertNull(controller.getReleaseDisplay().latestDate,
            "a stale snapshot for board_b cannot supply the latest date after identity switches to board_a");

        bp.signature = "rusEFI master." + today.minusDays(2).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".missing_board.live";
        controller.setDevMetadataLoaderForTests((board, branch) -> {
            throw new java.io.IOException("no board-specific asset");
        });
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        awaitStatusSettled(controller);
        assertEquals("Couldn't reach the update server. Check your network and try again.",
            controller.getReleaseStatus());
    }

    @Test
    void devDateStatusRequiresSelectedPortToMatchTheLiveEcu() throws Exception {
        PortResult live = new PortResult("ECU_A", SerialPortType.Ecu);
        AtomicReference<PortResult> selected = new AtomicReference<>(new PortResult("ECU_B", SerialPortType.Ecu));
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        LocalDate today = LocalDate.now(ZoneOffset.UTC);
        bp.signature = "rusEFI master." + today.minusDays(1).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.live";
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.getLastTriedPort()).thenReturn(live.port);
        FirmwareRollbackController controller = new FirmwareRollbackController(
            mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
            new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(selected.get()),
            () -> true, () -> {}, ignored -> {}, () -> {});
        controller.enableReleaseStatusDiscovery();
        controller.setLinkManager(lm);
        AtomicInteger lookups = new AtomicInteger();
        CountDownLatch completed = new CountDownLatch(1);
        controller.setDevMetadataLoaderForTests((board, branch) -> {
            lookups.incrementAndGet();
            completed.countDown();
            return today.atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli();
        });

        SwingUtilities.invokeAndWait(() -> controller.refresh(selected.get()));
        assertEquals("Couldn't check for updates. Reconnect the ECU and try again.", controller.getReleaseStatus());
        assertEquals(0, lookups.get(), "do not query or attribute metadata while another ECU is selected");

        selected.set(live);
        SwingUtilities.invokeAndWait(() -> controller.refresh(live));
        assertTrue(completed.await(5, TimeUnit.SECONDS));
        awaitStatusSettled(controller);
        assertEquals("Update available", controller.getReleaseStatus());
    }

    @Test
    void devMetadataReadsSelectionOnlyOnEdtDeduplicatesAndExpiresAfterFiveMinutes() throws Exception {
        PortResult port = new PortResult("ECU_A", SerialPortType.Ecu);
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        LocalDate today = LocalDate.now(ZoneOffset.UTC);
        bp.signature = "rusEFI master." + today.minusDays(1).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.live";
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.getLastTriedPort()).thenReturn(port.port);
        AtomicBoolean providerCalledOffEdt = new AtomicBoolean();
        AtomicInteger calls = new AtomicInteger();
        CountDownLatch firstStarted = new CountDownLatch(1);
        CountDownLatch releaseFirst = new CountDownLatch(1);
        CountDownLatch secondFinished = new CountDownLatch(1);
        CountDownLatch thirdFinished = new CountDownLatch(1);
        AtomicLong clock = new AtomicLong(1_000);
        ManualDevRefreshScheduler scheduler = new ManualDevRefreshScheduler();
        FirmwareRollbackController controller = new FirmwareRollbackController(
            mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
            new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> {
                if (!SwingUtilities.isEventDispatchThread()) {
                    providerCalledOffEdt.set(true);
                }
                return Optional.of(port);
            }, () -> true, () -> {}, ignored -> {}, () -> {});
        controller.enableReleaseStatusDiscovery();
        controller.setLinkManager(lm);
        controller.setDevClockForTests(clock::get);
        controller.setDevRefreshSchedulerForTests(scheduler);
        controller.setDevMetadataLoaderForTests((board, branch) -> {
            int call = calls.incrementAndGet();
            if (call == 1) {
                firstStarted.countDown();
                try {
                    if (!releaseFirst.await(5, TimeUnit.SECONDS)) {
                        throw new java.io.IOException("test metadata release timed out");
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    throw new java.io.IOException(e);
                }
            } else {
                if (call == 2) {
                    secondFinished.countDown();
                } else {
                    thirdFinished.countDown();
                }
            }
            return today.atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli();
        });

        SwingUtilities.invokeAndWait(() -> controller.setDevStatusViewActive(true));
        assertTrue(firstStarted.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertEquals(1, calls.get(), "the active request is shared across repeated scanner refreshes");
        releaseFirst.countDown();
        awaitStatusSettled(controller);
        assertEquals(DEV_METADATA_TTL_MILLIS, scheduler.pendingDelayMillis());
        clock.addAndGet(DEV_METADATA_TTL_MILLIS - 1);
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertEquals(1, calls.get(), "fresh metadata remains cached until TTL expiry");
        assertEquals(1, scheduler.pendingDelayMillis(), "cached refresh schedules only the remaining TTL");
        clock.incrementAndGet();
        SwingUtilities.invokeAndWait(scheduler::firePending);
        assertTrue(secondFinished.await(5, TimeUnit.SECONDS));
        awaitStatusSettled(controller);
        assertEquals(2, calls.get(), "timer expiry starts a new request for the unchanged selection and signature");
        assertEquals("Update available", controller.getReleaseStatus());

        SwingUtilities.invokeAndWait(() -> controller.refresh(null));
        assertFalse(scheduler.hasPendingTask(), "disconnect cancels the next scheduled metadata check");
        SwingUtilities.invokeAndWait(() -> controller.refresh(port));
        assertTrue(thirdFinished.await(5, TimeUnit.SECONDS));
        awaitStatusSettled(controller);
        assertTrue(scheduler.hasPendingTask());
        SwingUtilities.invokeAndWait(controller::dispose);
        assertFalse(scheduler.hasPendingTask(), "disposing the summary/controller cancels the pending check");
        assertEquals(3, calls.get(), "a cancelled scheduled callback cannot launch another network request");
        assertFalse(providerCalledOffEdt.get(), "Swing-backed port selection is read only on the EDT");
    }

    private static final long DEV_METADATA_TTL_MILLIS = 5 * 60 * 1000;

    private static final class ManualDevRefreshScheduler implements FirmwareRollbackController.DevRefreshScheduler {
        private Scheduled scheduled;

        @Override
        public FirmwareRollbackController.DevRefreshTask schedule(int delayMillis, Runnable task) {
            Scheduled next = new Scheduled(delayMillis, task);
            scheduled = next;
            return () -> {
                next.cancelled = true;
                if (scheduled == next) {
                    scheduled = null;
                }
            };
        }

        private int pendingDelayMillis() {
            if (scheduled == null) {
                throw new AssertionError("No scheduled Dev metadata refresh");
            }
            return scheduled.delayMillis;
        }

        private boolean hasPendingTask() {
            return scheduled != null && !scheduled.cancelled;
        }

        private void firePending() {
            Scheduled next = scheduled;
            if (next == null) {
                throw new AssertionError("No scheduled Dev metadata refresh");
            }
            scheduled = null;
            if (!next.cancelled) {
                next.task.run();
            }
        }

        private static final class Scheduled {
            private final int delayMillis;
            private final Runnable task;
            private boolean cancelled;

            private Scheduled(int delayMillis, Runnable task) {
                this.delayMillis = delayMillis;
                this.task = task;
            }
        }
    }

    @Test
    void lateDevReplyCannotOverrideStatusAfterSelectionWasRepopulated() throws Exception {
        PortResult original = new PortResult("ECU_A", SerialPortType.Ecu);
        AtomicReference<PortResult> selected = new AtomicReference<>(original);
        LinkManager lm = mock(LinkManager.class);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        LocalDate today = LocalDate.now(ZoneOffset.UTC);
        bp.signature = "rusEFI master." + today.minusDays(1).format(DateTimeFormatter.ofPattern("yyyy.M.d"))
            + ".uaefi_pro.live";
        when(lm.getBinaryProtocol()).thenReturn(bp);
        when(lm.getLastTriedPort()).thenReturn(original.port);
        FirmwareRollbackController controller = new FirmwareRollbackController(
            mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
            new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(selected.get()),
            () -> true, () -> {}, ignored -> {}, () -> {});
        controller.enableReleaseStatusDiscovery();
        controller.setLinkManager(lm);
        CountDownLatch firstStarted = new CountDownLatch(1);
        CountDownLatch releaseFirst = new CountDownLatch(1);
        CountDownLatch secondFinished = new CountDownLatch(1);
        CountDownLatch staleFinished = new CountDownLatch(1);
        AtomicInteger calls = new AtomicInteger();
        controller.setDevMetadataLoaderForTests((board, branch) -> {
            if (calls.incrementAndGet() == 1) {
                firstStarted.countDown();
                try {
                    if (!releaseFirst.await(5, TimeUnit.SECONDS)) {
                        throw new java.io.IOException("test metadata release timed out");
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    throw new java.io.IOException(e);
                }
                return today.atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli();
            }
            secondFinished.countDown();
            return today.minusDays(2).atStartOfDay().toInstant(ZoneOffset.UTC).toEpochMilli();
        });
        controller.addStateChangedListener(() -> {
            if (releaseFirst.getCount() == 0 && calls.get() == 2
                && !controller.getReleaseStatus().startsWith("Checking")) {
                staleFinished.countDown();
            }
        });

        SwingUtilities.invokeAndWait(() -> controller.refresh(original));
        assertTrue(firstStarted.await(5, TimeUnit.SECONDS));
        PortResult other = new PortResult("ECU_B", SerialPortType.Ecu);
        selected.set(other);
        SwingUtilities.invokeAndWait(() -> controller.refresh(other));
        assertEquals("Couldn't check for updates. Reconnect the ECU and try again.", controller.getReleaseStatus());
        selected.set(new PortResult(original.port, original.type));
        SwingUtilities.invokeAndWait(() -> controller.refresh(selected.get()));
        assertTrue(secondFinished.await(5, TimeUnit.SECONDS));
        awaitStatusSettled(controller);
        assertEquals("No newer update found.", controller.getReleaseStatus());

        releaseFirst.countDown();
        assertTrue(staleFinished.await(5, TimeUnit.SECONDS));
        assertEquals("No newer update found.", controller.getReleaseStatus(),
            "late response from before port selection repopulation must not replace the accepted result");
    }

    private static void awaitStatusSettled(FirmwareRollbackController controller) throws Exception {
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        while (controller.getReleaseStatus().startsWith("Checking") && System.nanoTime() < deadline) {
            Thread.yield();
        }
        SwingUtilities.invokeAndWait(() -> {});
        assertFalse(controller.getReleaseStatus().startsWith("Checking"), "metadata lookup should complete");
    }

    @Test
    void releaseStatusUsesReadOnlyCatalogWhenRollbackIsDisabledAndRejectsStaleBranchResults() throws Exception {
        String prior = System.getProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "false");
        final String oldSha = "1111111111111111111111111111111111111111";
        final String latestSha = "2222222222222222222222222222222222222222";
        final String kansasSha = "3333333333333333333333333333333333333333";
        final String tieSha = "4444444444444444444444444444444444444444";
        try {
            PortResult port = spy(new PortResult("TEST_PORT", SerialPortType.Ecu));
            AtomicReference<String> ecuSha = new AtomicReference<>(oldSha);
            doAnswer(ignored -> Optional.of(ecuSha.get())).when(port).getFirmwareHash();
            LinkManager lm = mock(LinkManager.class);
            BinaryProtocol bp = mock(BinaryProtocol.class);
            bp.signature = "rusEFI lts-26lima.2026.11.11.test_board.123";
            when(lm.getBinaryProtocol()).thenReturn(bp);
            when(lm.getLastTriedPort()).thenReturn(port.port);

            AtomicReference<FirmwareRollbackController> controllerRef = new AtomicReference<>();
            AtomicReference<CountDownLatch> idle = new AtomicReference<>(new CountDownLatch(1));
            AtomicReference<CountDownLatch> firstCatalogStarted = new AtomicReference<>(new CountDownLatch(1));
            AtomicReference<CountDownLatch> releaseFirstCatalog = new AtomicReference<>(new CountDownLatch(1));
            AtomicReference<String> mode = new AtomicReference<>("late-lima");
            SwingUtilities.invokeAndWait(() -> {
                FirmwareRollbackController controller = new FirmwareRollbackController(
                    mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
                    new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY),
                    () -> Optional.of(port), () -> true,
                    () -> {
                        FirmwareRollbackController current = controllerRef.get();
                        if (current != null && !current.isBusy()) {
                            idle.get().countDown();
                        }
                    }, ignored -> {}, () -> {});
                controllerRef.set(controller);
                controller.enableReleaseStatusDiscovery();
                controller.setLinkManager(lm);
                controller.setCatalogLoaderForTests((board, branch) -> {
                    if ("late-lima".equals(mode.get()) && "lts-26lima".equals(branch)) {
                        firstCatalogStarted.get().countDown();
                        try {
                            if (!releaseFirstCatalog.get().await(5, TimeUnit.SECONDS)) {
                                throw new java.io.IOException("test catalog release timed out");
                            }
                        } catch (InterruptedException e) {
                            Thread.currentThread().interrupt();
                            throw new java.io.IOException(e);
                        }
                        return catalog(board, branch, oldSha, "2026-11-10 10:00",
                            latestSha, "2026-11-11 10:00");
                    }
                    if ("failure".equals(mode.get())) {
                        throw new java.io.IOException("offline");
                    }
                    if ("wrong-board".equals(mode.get())) {
                        return catalog("other_board", branch, oldSha, "2026-11-10 10:00");
                    }
                    if ("wrong-branch".equals(mode.get())) {
                        return catalog(board, "lts-26kansas", oldSha, "2026-11-10 10:00");
                    }
                    if ("tie".equals(mode.get())) {
                        return catalog(board, branch, oldSha, "2026-11-11 10:00",
                            tieSha, "2026-11-11 10:00");
                    }
                    if ("late-lima".equals(mode.get()) && "lts-26kansas".equals(branch)) {
                        return catalog(board, branch, kansasSha, "2026-11-12 10:00");
                    }
                    return catalog(board, branch, oldSha, "2026-11-10 10:00",
                        latestSha, "2026-11-11 10:00");
                });
                controller.refresh(port);
            });
            assertFalse(FirmwareRollbackController.isFirmwareRollbackEnabled(new Properties()));
            assertTrue(firstCatalogStarted.get().await(5, TimeUnit.SECONDS));
            assertEquals("Checking for updates…", controllerRef.get().getReleaseStatus());
            assertFalse(controllerRef.get().isPrimaryUpdateBlockedByCatalogDiscovery(),
                "read-only checks must not block normal updates when rollback interception is disabled");
            releaseFirstCatalog.get().countDown();
            assertTrue(idle.get().await(5, TimeUnit.SECONDS));
            assertEquals("Update available",
                controllerRef.get().getReleaseStatus());
            assertFalse(controllerRef.get().getRollbackButton().isVisible(),
                "read-only release checks must not enable rollback when its feature flag is off");

            ecuSha.set(latestSha);
            assertEquals("This ECU is up to date.", controllerRef.get().getReleaseStatus());
            ecuSha.set("5555555555555555555555555555555555555555");
            assertEquals("Couldn't check for updates. Try again later.",
                controllerRef.get().getReleaseStatus());

            ecuSha.set(oldSha);
            resetAndDiscover(controllerRef.get(), port, idle, mode, "wrong-board");
            assertEquals("Couldn't check for updates. Try again later.",
                controllerRef.get().getReleaseStatus());
            resetAndDiscover(controllerRef.get(), port, idle, mode, "wrong-branch");
            assertEquals("Couldn't check for updates. Try again later.",
                controllerRef.get().getReleaseStatus());
            resetAndDiscover(controllerRef.get(), port, idle, mode, "failure");
            assertEquals("Firmware update information is unavailable.", controllerRef.get().getReleaseStatus());
            resetAndDiscover(controllerRef.get(), port, idle, mode, "tie");
            ecuSha.set(oldSha);
            assertEquals("Couldn't compare firmware versions.", controllerRef.get().getReleaseStatus());

            mode.set("late-lima");
            firstCatalogStarted.set(new CountDownLatch(1));
            releaseFirstCatalog.set(new CountDownLatch(1));
            idle.set(new CountDownLatch(1));
            SwingUtilities.invokeAndWait(() -> {
                controllerRef.get().reset();
                bp.signature = "rusEFI lts-26lima.2026.11.11.test_board.123";
                ecuSha.set(oldSha);
                controllerRef.get().refresh(port);
            });
            assertTrue(firstCatalogStarted.get().await(5, TimeUnit.SECONDS));
            idle.set(new CountDownLatch(1));
            SwingUtilities.invokeAndWait(() -> {
                bp.signature = "rusEFI lts-26kansas.2026.11.12.test_board.456";
                ecuSha.set(kansasSha);
                controllerRef.get().refresh(port);
            });
            assertTrue(idle.get().await(5, TimeUnit.SECONDS));
            assertEquals("This ECU is up to date.", controllerRef.get().getReleaseStatus());
            idle.set(new CountDownLatch(1));
            releaseFirstCatalog.get().countDown();
            assertTrue(idle.get().await(5, TimeUnit.SECONDS));
            assertEquals("This ECU is up to date.", controllerRef.get().getReleaseStatus(),
                "a late response for the previous board/branch must not replace current ECU status");
        } finally {
            restoreRollbackEnabledProperty(prior);
        }
    }

    @Test
    void releaseStatusDoesNotUseSelectedEcuHashForDifferentLiveEcu() throws Exception {
        String prior = System.getProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "false");
        try {
            String liveSha = "1111111111111111111111111111111111111111";
            String selectedSha = "2222222222222222222222222222222222222222";
            PortResult livePort = spy(new PortResult("ECU_A", SerialPortType.Ecu));
            PortResult selectedPort = spy(new PortResult("ECU_B", SerialPortType.Ecu));
            doReturn(Optional.of(liveSha)).when(livePort).getFirmwareHash();
            doReturn(Optional.of(selectedSha)).when(selectedPort).getFirmwareHash();
            AtomicReference<PortResult> selected = new AtomicReference<>(selectedPort);
            LinkManager lm = mock(LinkManager.class);
            BinaryProtocol bp = mock(BinaryProtocol.class);
            bp.signature = "rusEFI lts-26lima.2026.11.11.test_board.live";
            when(lm.getBinaryProtocol()).thenReturn(bp);
            when(lm.getLastTriedPort()).thenReturn(livePort.port);

            AtomicReference<FirmwareRollbackController> controllerRef = new AtomicReference<>();
            CountDownLatch discovered = new CountDownLatch(1);
            SwingUtilities.invokeAndWait(() -> {
                FirmwareRollbackController controller = new FirmwareRollbackController(
                    mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
                    new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(selected.get()),
                    () -> true, () -> {
                        FirmwareRollbackController current = controllerRef.get();
                        if (current != null && !current.isBusy()) {
                            discovered.countDown();
                        }
                    }, ignored -> {}, () -> {});
                controllerRef.set(controller);
                controller.enableReleaseStatusDiscovery();
                controller.setLinkManager(lm);
                controller.setCatalogLoaderForTests((board, branch) -> catalog(board, branch,
                    liveSha, "2026-11-10 10:00", selectedSha, "2026-11-11 10:00"));
                controller.refresh(selectedPort);
            });
            assertTrue(discovered.await(5, TimeUnit.SECONDS));

            assertEquals("Couldn't check for updates. Reconnect the ECU and try again.",
                controllerRef.get().getReleaseStatus(),
                "a selected ECU hash must not be attributed to the different ECU on the live link");

            SwingUtilities.invokeAndWait(() -> {
                selected.set(livePort);
                controllerRef.get().refresh(livePort);
            });
            assertEquals("Update available",
                controllerRef.get().getReleaseStatus(), "matching selected/live port should use ECU A's hash");
        } finally {
            restoreRollbackEnabledProperty(prior);
        }
    }

    @Test
    void primaryUpdateIsBlockedOnlyWhileEnabledRollbackCatalogIsDiscovering() throws Exception {
        String prior = System.getProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", "true");
        try {
            PortResult port = new PortResult("TEST_PORT", SerialPortType.Ecu);
            LinkManager lm = mock(LinkManager.class);
            BinaryProtocol bp = mock(BinaryProtocol.class);
            bp.signature = "rusEFI lts-26lima.2026.11.11.test_board.123";
            when(lm.getBinaryProtocol()).thenReturn(bp);
            when(lm.getLastTriedPort()).thenReturn(port.port);
            CountDownLatch started = new CountDownLatch(1);
            CountDownLatch release = new CountDownLatch(1);
            CountDownLatch idle = new CountDownLatch(1);
            AtomicReference<FirmwareRollbackController> controllerRef = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> {
                FirmwareRollbackController controller = new FirmwareRollbackController(
                    mock(ConnectivityContext.class), UpdateOperationCallbacks.DUMMY,
                    new SingleAsyncJobExecutor(UpdateOperationCallbacks.DUMMY), () -> Optional.of(port),
                    () -> true, () -> {
                        FirmwareRollbackController current = controllerRef.get();
                        if (current != null && !current.isBusy()) {
                            idle.countDown();
                        }
                    }, ignored -> {}, () -> {});
                controllerRef.set(controller);
                controller.setLinkManager(lm);
                controller.setCatalogLoaderForTests((board, branch) -> {
                    started.countDown();
                    try {
                        if (!release.await(5, TimeUnit.SECONDS)) {
                            throw new java.io.IOException("test catalog release timed out");
                        }
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        throw new java.io.IOException(e);
                    }
                    return Collections.emptyList();
                });
                controller.refresh(port);
            });
            assertTrue(started.await(5, TimeUnit.SECONDS));
            assertTrue(controllerRef.get().isPrimaryUpdateBlockedByCatalogDiscovery());
            release.countDown();
            assertTrue(idle.await(5, TimeUnit.SECONDS));
            assertFalse(controllerRef.get().isPrimaryUpdateBlockedByCatalogDiscovery());
        } finally {
            restoreRollbackEnabledProperty(prior);
        }
    }

    private static void restoreRollbackEnabledProperty(String value) {
        if (value == null) {
            System.clearProperty("RE_FIRMWARE_ROLLBACK_ENABLED");
        } else {
            System.setProperty("RE_FIRMWARE_ROLLBACK_ENABLED", value);
        }
    }

    private static List<FirmwareRollbackResolver.Build> catalog(String board, String branch, String... shaAndDates) {
        StringBuilder html = new StringBuilder();
        for (int i = 0; i < shaAndDates.length; i += 2) {
            html.append("<a href=\"").append(shaAndDates[i]).append("/\">build</a> ")
                .append(shaAndDates[i + 1]).append(" -\n");
        }
        return FirmwareRollbackResolver.parseIndex(html.toString(), board, branch,
            "https://example.test/build_server/" + board + "/" + branch + "/");
    }

    private static void resetAndDiscover(FirmwareRollbackController controller, PortResult port,
                                         AtomicReference<CountDownLatch> idle, AtomicReference<String> mode,
                                         String nextMode) throws Exception {
        mode.set(nextMode);
        idle.set(new CountDownLatch(1));
        SwingUtilities.invokeAndWait(() -> {
            controller.reset();
            controller.refresh(port);
        });
        assertTrue(idle.get().await(5, TimeUnit.SECONDS));
    }
}
