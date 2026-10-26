package com.rusefi.ui.console;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.io.ConnectionStatusValue;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.Icon;
import javax.swing.JTextArea;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.StyleConstants;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class MainFrameUpdateCheckTest {
    @TempDir
    Path tempDir;

    @Test
    public void installedImageWithDifferentFilenameDate() throws IOException {
        // #10406 characterization: one image can have a UTC filename date and a London signature date.
        // Comparing only the filename date incorrectly offers this installed image again.
        RusEfiSignature installed = new RusEfiSignature(
            "master", "2026", "07", "02", "uaefi_pro", "4226383888", false);
        Path file = writeFirmware("rusefi_development_2026-07-01_uaefi_pro_4226383888_local_update.srec",
            "rusEFI master.2026.07.02.uaefi_pro.4226383888");

        assertEquals(MainFrame.FirmwareUpdateCheckResult.UP_TO_DATE,
            MainFrame.firmwareUpdateCheckResultFromFile(installed, file.toFile()));
    }

    private Path writeFirmware(String filename, String signature) throws IOException {
        byte[] data = (signature + "\0").getBytes(StandardCharsets.US_ASCII);
        int count = 4 + data.length + 1;
        int checksum = count + 8; // S3 address 0x08000000.
        StringBuilder record = new StringBuilder(String.format("S3%02X08000000", count));
        for (byte value : data) {
            checksum += value & 0xFF;
            record.append(String.format("%02X", value & 0xFF));
        }
        record.append(String.format("%02X%n", ~checksum & 0xFF));
        Path file = tempDir.resolve(filename);
        Files.write(file, record.toString().getBytes(StandardCharsets.US_ASCII));
        return file;
    }

    // ECU signature: rusEFI development.2026.05.09.uaefi_pro.4226383888
    private static final RusEfiSignature ECU_SIG = new RusEfiSignature(
        "development", "2026", "05", "09", "uaefi_pro", "4226383888", false
    );
    private static final String ECU_SIGNATURE = "rusEFI development.2026.05.09.uaefi_pro.4226383888";

    @Test
    public void nullEcuSigIsUnableToCheck() throws IOException {
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK,
            MainFrame.firmwareUpdateCheckResultFromFile(null,
                writeFirmware("firmware.srec", ECU_SIGNATURE).toFile()));
    }

    @Test
    public void nullSrecFileIsUnableToCheck() {
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, null));
    }

    @Test
    public void missingSrecIsUnableToCheckEvenWhenFilenameMatches() {
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, tempDir.resolve(
                "rusefi_development_2026-05-09_uaefi_pro_4226383888_local_update.srec").toFile()));
    }

    @Test
    public void malformedSrecIsUnableToCheck() throws IOException {
        Path file = tempDir.resolve("firmware.srec");
        Files.write(file, "garbage".getBytes(StandardCharsets.US_ASCII));
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, file.toFile()));
    }

    @Test
    public void matchingEmbeddedSignatureIsUpToDateRegardlessOfFilename() throws IOException {
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UP_TO_DATE,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG,
                writeFirmware("firmware.srec", ECU_SIGNATURE).toFile()));
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UP_TO_DATE,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, writeFirmware(
                "rusefi_development_2027-05-09_uaefi_pro_9999999999_local_update.srec",
                ECU_SIGNATURE).toFile()));
    }

    @Test
    public void absentEmbeddedSignatureDoesNotFallBackToFilename() throws IOException {
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, writeFirmware(
                "rusefi_development_2027-05-09_uaefi_pro_9999999999_local_update.srec",
                "no readable firmware signature").toFile()));
    }

    @Test
    public void firmwareCheckRequiresFullyConnectedState() {
        assertFalse(MainFrame.isFirmwareUpdateConnectionReady(ConnectionStatusValue.NOT_CONNECTED));
        assertFalse(MainFrame.isFirmwareUpdateConnectionReady(ConnectionStatusValue.LOADING));
        assertTrue(MainFrame.isFirmwareUpdateConnectionReady(ConnectionStatusValue.CONNECTED));
    }

    @Test
    public void installerAnnouncementRequiresExtractedPublicWindowsBundleAndCompatibleLiveTarget() {
        String compatibility = "uaefi, proteus_f7";
        String installerUrl = "https://example.test/universal-updater.exe";

        assertTrue(MainFrame.shouldAnnounceInstaller(
            true, true, true, true, true, false, "PROTEUS_F7", compatibility, installerUrl));
        assertFalse(MainFrame.shouldAnnounceInstaller(
            false, true, true, true, true, false, "proteus_f7", compatibility, installerUrl));
        assertFalse(MainFrame.shouldAnnounceInstaller(
            true, false, true, true, true, false, "proteus_f7", compatibility, installerUrl));
        assertFalse(MainFrame.shouldAnnounceInstaller(
            true, true, false, true, true, false, "proteus_f7", compatibility, installerUrl));
        assertFalse(MainFrame.shouldAnnounceInstaller(
            true, true, true, false, true, false, "proteus_f7", compatibility, installerUrl));
        assertFalse(MainFrame.shouldAnnounceInstaller(
            true, true, true, true, false, false, "proteus_f7", compatibility, installerUrl));
        assertFalse(MainFrame.shouldAnnounceInstaller(
            true, true, true, true, true, true, "proteus_f7", compatibility, installerUrl));
        assertFalse(MainFrame.shouldAnnounceInstaller(
            true, true, true, true, true, false, "hellen121nissan", compatibility, installerUrl));
        assertFalse(MainFrame.shouldAnnounceInstaller(
            true, true, true, true, true, false, "proteus_f7", compatibility, ""));
    }

    @Test
    public void differentEmbeddedDateSameConfigurationHashReturnsAvailable() throws IOException {
        for (String date : new String[]{"2026.05.10", "2026.06.09", "2027.05.09", "2026.05.08"}) {
            Path file = writeFirmware(
                "rusefi_development_2026-05-09_uaefi_pro_4226383888_local_update.srec",
                "rusEFI development." + date + ".uaefi_pro.4226383888");
            assertEquals(MainFrame.FirmwareUpdateCheckResult.AVAILABLE,
                MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, file.toFile()),
                "Different embedded dates must not match just because the configuration is unchanged: " + date);
        }
    }

    @Test
    public void differentEmbeddedHashOnSameDateReturnsAvailable() throws IOException {
        assertEquals(MainFrame.FirmwareUpdateCheckResult.AVAILABLE,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, writeFirmware("firmware.srec",
                "rusEFI development.2026.05.09.uaefi_pro.1111111111").toFile()));
    }

    @Test
    public void branchAndTargetDifferencesAreNotReportedAsMatching() throws IOException {
        for (String signature : new String[]{
            "rusEFI master.2026.05.09.uaefi_pro.4226383888",
            "rusEFI development.2026.05.09.proteus_f7.4226383888"}) {
            assertEquals(MainFrame.FirmwareUpdateCheckResult.AVAILABLE,
                MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG,
                    writeFirmware("firmware.srec", signature).toFile()), signature);
        }
    }

    @Test
    public void legacyFilenameStillUsesEmbeddedSignature() throws IOException {
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UP_TO_DATE,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, writeFirmware(
                "rusefi_development_2026-05-10_8849742d4267db6407b1400ae917a1ed39795d32_update.srec",
                ECU_SIGNATURE).toFile()));
    }

    @Test
    public void afterUpdatingEcuTheSameImageIsNoLongerOffered() throws IOException {
        String nextSignature = "rusEFI development.2026.05.10.uaefi_pro.4226383888";
        Path file = writeFirmware(
            "rusefi_development_2026-05-09_uaefi_pro_4226383888_local_update.srec", nextSignature);
        assertEquals(MainFrame.FirmwareUpdateCheckResult.AVAILABLE,
            MainFrame.firmwareUpdateCheckResultFromFile(ECU_SIG, file.toFile()));
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UP_TO_DATE,
            MainFrame.firmwareUpdateCheckResultFromFile(SignatureHelper.parse(nextSignature), file.toFile()));
    }

    @Test
    public void menuIconsLoadAtExpectedSize() {
        String[] icons = {"folder-open", "floppy", "logout", "refresh", "controller", "player-play", "player-stop"};
        for (String name : icons) {
            Icon icon = MainFrame.loadMenuIcon(name);
            assertNotNull(icon, name + " should load");
            assertEquals(18, icon.getIconWidth(), name + " icon width");
            assertEquals(18, icon.getIconHeight(), name + " icon height");
        }
    }

    @Test
    public void firmwareCheckMessageUsesAvailableWidth() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            MainFrame.FrameOverlay overlay = new MainFrame.FrameOverlay("Checking ECU firmware...", Color.DARK_GRAY,
                new MainFrame.OverlayAction("Update ECU Firmware", 'U', () -> {}),
                new MainFrame.OverlayAction("Close", 'C', () -> {}));
            overlay.setActionVisible(0, false);
            overlay.setMessage("Unable to check ECU firmware", Color.RED);
            Container content = (Container) overlay.getComponent(0);
            JTextArea message = (JTextArea) content.getComponent(0);
            for (int width : new int[]{800, 1280, 1916}) {
                overlay.setSize(width, 600);
                layoutRecursively(overlay);
                assertTrue(message.getWidth() >= width - 100, "Message should use the available window width");
                assertTrue(message.getWidth() >= message.getFontMetrics(message.getFont()).stringWidth(message.getText()),
                    "The firmware check error should fit on one line");
                assertTrue(message.getHeight() >= message.getFontMetrics(message.getFont()).getHeight());
                assertTrue(content.getY() >= 0);
                assertTrue(content.getY() + content.getHeight() <= overlay.getHeight(), "Actions should remain on screen");
            }
        });
    }

    @Test
    public void firmwareCheckMessageIsCenteredWithoutRestylingDialog() throws Exception {
        // Screenshot characterization: the firmware-check text is left aligned despite centered actions.
        SwingUtilities.invokeAndWait(() -> {
            MainFrame.FrameOverlay overlay = new MainFrame.FrameOverlay("Checking ECU firmware...", Color.DARK_GRAY, true,
                new MainFrame.OverlayAction("Update ECU Firmware", 'U', () -> {}),
                new MainFrame.OverlayAction("Close", 'C', () -> {}));
            overlay.setActionVisible(0, false);
            Container content = (Container) overlay.getComponent(0);
            JTextPane message = (JTextPane) content.getComponent(0);
            assertTrue(overlay.isOpaque(), "Keep the existing dialog background");
            assertEquals(32f, message.getFont().getSize2D(), "Keep the existing message font size");
            assertEquals(new JTextArea().getFont().deriveFont(java.awt.Font.BOLD, 32f), message.getFont());
            assertEquals(new JTextArea().getMargin(), message.getMargin());
            assertEquals(Color.DARK_GRAY, message.getForeground());
            for (String text : new String[]{"Checking ECU firmware...", "ECU firmware update available",
                "ECU already matches the local firmware image", "Unable to check ECU firmware"}) {
                overlay.setMessage(text, Color.DARK_GRAY);
                assertEquals(StyleConstants.ALIGN_CENTER, StyleConstants.getAlignment(
                    message.getStyledDocument().getParagraphElement(0).getAttributes()));
                assertEquals(text, overlay.getMessageForUnitTest());
            }
        });
    }

    private static void layoutRecursively(Container container) {
        container.doLayout();
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                layoutRecursively((Container) component);
            }
        }
    }

    @Test
    public void overlayShowsExplicitResultsAndActions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean updateClicked = new AtomicBoolean();
            AtomicBoolean closeClicked = new AtomicBoolean();
            MainFrame.FrameOverlay overlay = new MainFrame.FrameOverlay("Checking ECU firmware...", Color.DARK_GRAY,
                new MainFrame.OverlayAction("Update ECU Firmware", 'U', () -> updateClicked.set(true)),
                new MainFrame.OverlayAction("Close", 'C', () -> closeClicked.set(true)));
            overlay.setActionVisible(0, false);

            assertEquals("Checking ECU firmware...", overlay.getMessageForUnitTest());
            assertFalse(overlay.isActionVisibleForUnitTest(0));
            assertTrue(overlay.isActionVisibleForUnitTest(1));

            overlay.setMessage("ECU firmware update available", Color.GREEN);
            overlay.setActionVisible(0, true);
            assertEquals("ECU firmware update available", overlay.getMessageForUnitTest());
            assertTrue(overlay.isActionVisibleForUnitTest(0));
            assertTrue(overlay.isActionVisibleForUnitTest(1));
            overlay.actionForUnitTest(0);
            assertTrue(updateClicked.get());

            overlay.setMessage("ECU already matches the local firmware image", Color.GREEN);
            overlay.setActionVisible(0, false);
            assertEquals("ECU already matches the local firmware image", overlay.getMessageForUnitTest());
            assertFalse(overlay.isActionVisibleForUnitTest(0));
            assertTrue(overlay.isActionVisibleForUnitTest(1));

            overlay.setMessage("Unable to check ECU firmware", Color.RED);
            assertEquals("Unable to check ECU firmware", overlay.getMessageForUnitTest());
            assertFalse(overlay.isActionVisibleForUnitTest(0));
            assertTrue(overlay.isActionVisibleForUnitTest(1));
            overlay.actionForUnitTest(1);
            assertTrue(closeClicked.get());
        });
    }

    @Test
    public void connectionFailureOverlayShowsDownloadOnlyWhenAvailable() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean downloadClicked = new AtomicBoolean();
            AtomicBoolean closeClicked = new AtomicBoolean();
            MainFrame.FrameOverlay overlay = new MainFrame.FrameOverlay("Connection failed", Color.DARK_GRAY,
                new MainFrame.OverlayAction("Open Download Page", 'O', () -> downloadClicked.set(true)),
                new MainFrame.OverlayAction("Close", 'C', () -> closeClicked.set(true)));

            assertEquals("Connection failed", overlay.getMessageForUnitTest());
            assertTrue(overlay.isActionVisibleForUnitTest(0));
            overlay.actionForUnitTest(0);
            assertTrue(downloadClicked.get());
            overlay.actionForUnitTest(1);
            assertTrue(closeClicked.get());

            overlay.setActionVisible(0, false);
            assertFalse(overlay.isActionVisibleForUnitTest(0));
        });
    }

    @Test
    public void unsavedTuneOverlayOffersSaveDiscardAndCancel() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean saveClicked = new AtomicBoolean();
            AtomicBoolean discardClicked = new AtomicBoolean();
            AtomicBoolean cancelClicked = new AtomicBoolean();
            MainFrame.FrameOverlay overlay = new MainFrame.FrameOverlay("The tune has unsaved changes.", Color.DARK_GRAY,
                new MainFrame.OverlayAction("Save and Exit", 'S', () -> saveClicked.set(true)),
                new MainFrame.OverlayAction("Exit Without Saving", 'E', () -> discardClicked.set(true)),
                new MainFrame.OverlayAction("Cancel", 'C', () -> cancelClicked.set(true)));

            assertEquals("The tune has unsaved changes.", overlay.getMessageForUnitTest());
            overlay.actionForUnitTest(0);
            overlay.actionForUnitTest(1);
            overlay.actionForUnitTest(2);
            assertTrue(saveClicked.get());
            assertTrue(discardClicked.get());
            assertTrue(cancelClicked.get());
        });
    }
}
