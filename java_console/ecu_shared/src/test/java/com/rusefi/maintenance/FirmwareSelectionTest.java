package com.rusefi.maintenance;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoInteractions;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

class FirmwareSelectionTest {
    private static final String SOURCE = "rusEFI development.2026.09.09.test_board.hash";
    private static final BundleInfo DESTINATION = new BundleInfo("development", null, "test_board");

    @TempDir
    Path temp;

    @Test
    void selectionRequiresTheArtifactForItsTransport() throws Exception {
        Path srec = write("firmware.srec", "srec bytes");
        Path bin = write("firmware.bin", "bin bytes");
        FirmwareSelection both = new FirmwareSelection(SOURCE, DESTINATION,
            srec.toString(), bin.toString(), false);
        FirmwareSelection srecOnly = new FirmwareSelection(SOURCE, DESTINATION,
            srec.toString(), null, false);
        FirmwareSelection binOnly = new FirmwareSelection(SOURCE, DESTINATION,
            null, bin.toString(), false);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);

        assertTrue(both.revalidateForSrec(SOURCE, callbacks));
        assertTrue(both.revalidateForBin(SOURCE, callbacks));
        assertTrue(srecOnly.revalidateForSrec(SOURCE, callbacks));
        assertFalse(srecOnly.revalidateForBin(SOURCE, callbacks));
        assertTrue(binOnly.revalidateForBin(SOURCE, callbacks));
        assertFalse(binOnly.revalidateForSrec(SOURCE, callbacks));
    }

    @Test
    void limaDevToDevSelectionRequiresConfirmationForBothTransports() throws Exception {
        String limaDev = "rusEFI lts-26lima-dev.2026.09.09.test_board.hash";
        Path srec = write("firmware.srec", "srec bytes");
        Path bin = write("firmware.bin", "bin bytes");
        FirmwareSelection confirmed = new FirmwareSelection(limaDev, DESTINATION,
            srec.toString(), bin.toString(), true);
        FirmwareSelection unconfirmed = new FirmwareSelection(limaDev, DESTINATION,
            srec.toString(), bin.toString(), false);

        assertTrue(confirmed.revalidateForSrec(limaDev, mock(UpdateOperationCallbacks.class)));
        assertTrue(confirmed.revalidateForBin(limaDev, mock(UpdateOperationCallbacks.class)));
        assertFalse(unconfirmed.revalidateForSrec(limaDev, mock(UpdateOperationCallbacks.class)));
        assertFalse(unconfirmed.revalidateForBin(limaDev, mock(UpdateOperationCallbacks.class)));
    }

    @Test
    void replacedOrDeletedSelectedBytesInvalidateSelection() throws Exception {
        Path bin = write("firmware.bin", "original bytes");
        FirmwareSelection replaced = new FirmwareSelection(SOURCE, DESTINATION, null, bin.toString(), false);
        Files.write(bin, "replacement bytes".getBytes(StandardCharsets.UTF_8));
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        assertFalse(replaced.revalidateForBin(SOURCE, callbacks));
        verify(callbacks).firmwareUpdateBlocked("The firmware file has changed or cannot be read. Please download it again.");

        Path deletedBin = write("deleted.bin", "original bytes");
        FirmwareSelection deleted = new FirmwareSelection(SOURCE, DESTINATION, null, deletedBin.toString(), false);
        Files.delete(deletedBin);
        UpdateOperationCallbacks deletionCallbacks = mock(UpdateOperationCallbacks.class);
        assertFalse(deleted.revalidateForBin(SOURCE, deletionCallbacks));
        verify(deletionCallbacks).firmwareUpdateBlocked("The firmware file has changed or cannot be read. Please download it again.");
    }

    @Test
    void sourceSignatureMustStillMatchTheSelection() throws Exception {
        Path srec = write("firmware.srec", "srec bytes");
        FirmwareSelection selection = new FirmwareSelection(SOURCE, DESTINATION, srec.toString(), null, false);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        assertFalse(selection.revalidateForSrec(
            "rusEFI lts-26lima.2026.09.09.test_board.hash", callbacks));
        verify(callbacks).firmwareUpdateBlocked("The connected ECU or its firmware has changed. Please restart the update.");
    }

    @Test
    void explicitSrecCannotOverrideDifferentSelectedPath() throws Exception {
        Path selected = write("selected.srec", "selected bytes");
        Path conflicting = write("other-branch.srec", "other branch bytes");
        FirmwareSelection selection = new FirmwareSelection(SOURCE, DESTINATION,
            selected.toString(), null, false);

        assertTrue(selection.matchesSrecPath(null));
        assertTrue(selection.matchesSrecPath(selected.toString()));
        assertFalse(selection.matchesSrecPath(conflicting.toString()));
    }

    @Test
    void explicitBinCannotOverrideDifferentSelectedPath() throws Exception {
        Path selected = write("selected.bin", "selected bytes");
        Path conflicting = write("other-branch.bin", "other branch bytes");
        FirmwareSelection selection = new FirmwareSelection(SOURCE, DESTINATION,
            null, selected.toString(), false);

        assertTrue(selection.matchesBinPath(null));
        assertTrue(selection.matchesBinPath(selected.toString()));
        assertFalse(selection.matchesBinPath(conflicting.toString()));
    }

    @Test
    void unknownOrMismatchedBoardDestinationIsRejected() throws Exception {
        Path srec = write("firmware.srec", "srec bytes");
        FirmwareSelection wrongBoard = new FirmwareSelection(SOURCE,
            new BundleInfo("development", null, "other_board"), srec.toString(), null, false);
        assertFalse(wrongBoard.revalidateForSrec(SOURCE, mock(UpdateOperationCallbacks.class)));
    }

    @Test
    void automaticDfuRejectsSrecOnlySelectionBeforeCalibrationOrHandoff() throws Exception {
        Path srec = write("firmware.srec", "srec bytes");
        FirmwareSelection srecOnly = new FirmwareSelection(SOURCE, DESTINATION, srec.toString(), null, false);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = SOURCE;
        LinkManager lm = mock(LinkManager.class);
        ConnectivityContext connectivity = mock(ConnectivityContext.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);

        assertFalse(DfuFlasher.doAutoDfu(null, new PortResult("TEST_PORT", SerialPortType.Ecu),
            bp, lm, callbacks, connectivity, null, CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION,
            srecOnly));
        verifyNoInteractions(lm, connectivity);
    }

    @Test
    void automaticOpenBltRejectsBinOnlySelectionBeforeCalibrationOrHandoff() throws Exception {
        Path bin = write("firmware.bin", "bin bytes");
        FirmwareSelection binOnly = new FirmwareSelection(SOURCE, DESTINATION, null, bin.toString(), false);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = SOURCE;
        LinkManager lm = mock(LinkManager.class);
        ConnectivityContext connectivity = mock(ConnectivityContext.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);

        assertFalse(FirmwareOperations.flashOpenbltSerialAutomatic(null,
            new PortResult("TEST_PORT", SerialPortType.EcuWithOpenblt), bp, lm, callbacks, connectivity,
            null, CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION, binOnly));
        verifyNoInteractions(lm, connectivity);
    }

    @Test
    void directAutomaticDfuRequiresASelectionEvenForAnEligibleFilename() {
        for (String filename : new String[]{
            "rusefi_development_2026-09-09_test_board_1234_hash_update.srec",
            "rusefi_development_2026-09-09_test_board_1234_hash.bin"}) {
            BinaryProtocol bp = mock(BinaryProtocol.class);
            bp.signature = SOURCE;
            LinkManager lm = mock(LinkManager.class);
            ConnectivityContext connectivity = mock(ConnectivityContext.class);
            UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);

            assertFalse(DfuFlasher.doAutoDfu(null, new PortResult("TEST_PORT", SerialPortType.Ecu),
                bp, lm, callbacks, connectivity, filename));
            verify(callbacks).firmwareUpdateBlocked("Couldn't verify which ECU board and branch this firmware is for. Please restart the update.");
            verifyNoInteractions(lm, connectivity);
        }
    }

    @Test
    void directAutomaticApisRejectConflictingPathsBeforeCalibrationOrHandoff() throws Exception {
        Path srec = write("selected.srec", "selected SREC bytes");
        Path bin = write("selected.bin", "selected BIN bytes");
        FirmwareSelection selection = new FirmwareSelection(SOURCE, DESTINATION,
            srec.toString(), bin.toString(), false);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = SOURCE;
        ConnectivityContext connectivity = mock(ConnectivityContext.class);
        LinkManager can = mock(LinkManager.class);
        when(can.getLastTriedPort()).thenReturn(LinkManager.PCAN);
        UpdateOperationCallbacks canCallbacks = mock(UpdateOperationCallbacks.class);

        assertFalse(FirmwareOperations.flashOpenbltCanAutomatic(null,
            new PortResult(LinkManager.PCAN, SerialPortType.EcuWithOpenblt), bp, can, canCallbacks,
            connectivity, temp.resolve("other-branch.srec").toString(),
            CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION, selection));
        verify(canCallbacks).firmwareUpdateBlocked("The firmware file does not match the selected update. Please restart the update.");
        verify(can).getLastTriedPort();
        verifyNoMoreInteractions(can);

        LinkManager serial = mock(LinkManager.class);
        UpdateOperationCallbacks dfuCallbacks = mock(UpdateOperationCallbacks.class);
        assertFalse(DfuFlasher.doAutoDfu(null, new PortResult("TEST_PORT", SerialPortType.Ecu),
            bp, serial, dfuCallbacks, connectivity, temp.resolve("other-branch.bin").toString(),
            CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION, selection));
        verify(dfuCallbacks).firmwareUpdateBlocked("The firmware file does not match the selected update. Please restart the update.");
        verifyNoInteractions(serial, connectivity);
    }

    private Path write(String name, String contents) throws IOException {
        return Files.write(temp.resolve(name), contents.getBytes(StandardCharsets.UTF_8));
    }
}
