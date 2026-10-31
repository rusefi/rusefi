package com.rusefi.maintenance;

import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class ManualDfuRecoveryTest {
    @Test
    void cancellingPickerDoesNotPrepareOrFlash() throws IOException {
        ManualDfuRecovery.FirmwarePreparation prepare = mock(ManualDfuRecovery.FirmwarePreparation.class);
        ManualDfuRecovery.FlashAction flash = mock(ManualDfuRecovery.FlashAction.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        ManualDfuRecovery.run(ConnectedEcuTarget.forManualRecovery("proteus_f7"), callbacks,
            suggested -> {
                assertEquals("proteus_f7", suggested);
                return null;
            }, prepare, flash);
        verifyNoInteractions(prepare, flash);
        verify(callbacks, never()).done();
        verify(callbacks, never()).error();
    }

    @Test
    void changingBoardPreparesAndFlashesOnlyTheExplicitChoice() {
        ConnectedEcuTarget previous = ConnectedEcuTarget.forManualRecovery("proteus_f7");
        List<String> events = new ArrayList<>();
        ManualDfuRecovery.run(previous, UpdateOperationCallbacks.DUMMY,
            suggested -> {
                events.add("pick:" + suggested);
                return "proteus_h7";
            }, selected -> {
                events.add("prepare:" + selected);
                return "/selected-download/rusefi.bin";
            }, (file, target) -> {
                events.add("flash:" + target.effectiveTarget());
                assertEquals("/selected-download/rusefi.bin", file);
                assertFalse(target.isLiveTargetKnown());
            });
        assertEquals(Arrays.asList("pick:proteus_f7", "prepare:proteus_h7", "flash:proteus_h7"), events);
        assertEquals("proteus_f7", previous.effectiveTarget());
    }

    @Test
    void unknownBoardStillRequiresExplicitSelection() {
        ConnectedEcuTarget previous = mock(ConnectedEcuTarget.class);
        when(previous.effectiveTarget()).thenReturn("universal");
        ManualDfuRecovery.run(previous, UpdateOperationCallbacks.DUMMY,
            suggested -> {
                assertEquals("universal", suggested);
                return "uaefi";
            }, selected -> {
                assertEquals("uaefi", selected);
                return "/selected/rusefi.bin";
            }, (file, target) -> assertEquals("uaefi", target.effectiveTarget()));
        verify(previous, never()).set(anyString());
    }

    @Test
    void unavailableFirmwareReportsFailureWithoutFlashing() {
        ManualDfuRecovery.FlashAction flash = mock(ManualDfuRecovery.FlashAction.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        ManualDfuRecovery.run(ConnectedEcuTarget.forManualRecovery("uaefi"), callbacks,
            suggested -> suggested, selected -> {
                throw new IOException("Selected firmware requires OpenBLT");
            }, flash);
        verifyNoInteractions(flash);
        verify(callbacks).error();
        verify(callbacks).logLine(contains("requires OpenBLT"));
    }

    @Test
    void failedCatalogStopsBeforeDownloadOrFlash() throws IOException {
        ManualDfuRecovery.FirmwarePreparation prepare = mock(ManualDfuRecovery.FirmwarePreparation.class);
        ManualDfuRecovery.FlashAction flash = mock(ManualDfuRecovery.FlashAction.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        ManualDfuRecovery.run(ConnectedEcuTarget.forManualRecovery("uaefi"), callbacks,
            suggested -> { throw new IOException("Board list unavailable"); }, prepare, flash);
        verifyNoInteractions(prepare, flash);
        verify(callbacks).error();
    }

    @Test
    void catalogUsesConcreteSortedTargetIdsAndRejectsInvalidData() throws IOException {
        assertEquals(Arrays.asList("super-uaefi", "uaefi", "uaefi_pro", "hellen121nissan", "hellen154hyundai",
            "alphax-2chan", "proteus_f7"), DfuBoardPicker.compatibleTargets(
            "proteus_f7, hellen154hyundai, uaefi_pro, universal, alphax-2chan, uaefi, "
                + "hellen121nissan, super-uaefi, uaefi"));
        assertThrows(IOException.class, () -> DfuBoardPicker.compatibleTargets(null));
        assertThrows(IOException.class, () -> DfuBoardPicker.compatibleTargets("*"));
        assertThrows(IOException.class, () -> DfuBoardPicker.compatibleTargets("../bad"));
    }
}
