package com.rusefi.ui.wizard;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.hd81.HdConstants;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class WizardCatalogTest {

    @Test
    public void flaggedStepsHaveStableOrderAndTitles() {
        List<WizardStepDescriptor> flagged = WizardCatalog.flaggedSteps();
        assertEquals(Arrays.asList(
            "wizardNumberOfCylinders", "wizardFiringOrder", "wizardDisplacement", "wizardMapSensorType",
            "wizardTps", "wizardCltSensor", "wizardCrankTrigger", "wizardCamTrigger",
            "wizardIgnitionOutputs", "wizardInjectorOutputs", "wizardInjectorFlow"),
            flagged.stream().map(d -> d.flagName).collect(java.util.stream.Collectors.toList()));
        assertEquals(Arrays.asList(
            "Cylinders", "Firing Order", "Base VE Table", "MAP Sensor", "TPS", "CLT Sensor",
            "Crank Trigger", "Cam Trigger", "Ignition Outputs", "Injector Outputs", "Injector Flow"),
            flagged.stream().map(d -> d.displayTitle).collect(java.util.stream.Collectors.toList()));
    }

    @Test
    public void flaggedStepsAreAllFlagged() {
        List<WizardStepDescriptor> flagged = WizardCatalog.flaggedSteps();
        assertFalse(flagged.isEmpty());
        for (WizardStepDescriptor d : flagged) {
            assertTrue(d.isFlagged(), "flaggedSteps() returned non-flagged entry: " + d.flagName);
        }
    }

    @Test
    public void tpsAndCltFollowMapAndApplyToAllBoards() {
        List<WizardStepDescriptor> flagged = WizardCatalog.flaggedSteps();
        List<String> names = flagged.stream().map(d -> d.flagName).collect(Collectors.toList());
        int mapIndex = names.indexOf("wizardMapSensorType");

        assertTrue(mapIndex >= 0);
        assertEquals("wizardTps", names.get(mapIndex + 1));
        assertEquals("wizardCltSensor", names.get(mapIndex + 2));
        assertTrue(flagged.get(mapIndex + 1).applicable.test(mock(UIContext.class)));
        assertTrue(flagged.get(mapIndex + 2).applicable.test(mock(UIContext.class)));
    }

    @Test
    public void standaloneAutoLaunchIsFilteredCorrectly() {
        List<WizardStepDescriptor> standalone = WizardCatalog.standaloneAutoLaunch();
        for (WizardStepDescriptor d : standalone) {
            assertFalse(d.isFlagged(), "standaloneAutoLaunch() returned flagged entry");
            assertTrue(d.autoLaunch);
            assertNotNull(d.factory, "standalone entries must have a factory");
        }
    }

    @Test
    public void catalogExposesVinAutoLaunchEntry() {
        // Sanity: VIN is the canonical standalone descriptor; its needsAttention predicate exists.
        List<WizardStepDescriptor> standalone = WizardCatalog.standaloneAutoLaunch();
        assertFalse(standalone.isEmpty(), "expected at least one standalone auto-launch entry");
        WizardStepDescriptor vin = standalone.get(0);
        assertNotNull(vin.needsAttention);
    }

    // ---- boardNameContains ----

    @Test
    public void boardNameContainsMatchesSubstringCaseInsensitive() {
        Predicate<UIContext> hd81 = WizardCatalog.boardNameContains(HdConstants.HD_81);
        assertTrue(hd81.test(contextWithSignature("rusEFI master.2026.04.18.hd81-gdi.abc123")));
        assertTrue(hd81.test(contextWithSignature("rusEFI master.2026.04.18.HD81-gdi.abc123")));
    }

    @Test
    public void boardNameContainsRejectsNonMatchingBoard() {
        Predicate<UIContext> hd81 = WizardCatalog.boardNameContains(HdConstants.HD_81);
        assertFalse(hd81.test(contextWithSignature("rusEFI master.2026.04.18.proteus_f7.abc123")));
    }

    @Test
    public void boardNameContainsReturnsFalseForNullBinaryProtocol() {
        UIContext ctx = mock(UIContext.class);
        when(ctx.getBinaryProtocol()).thenReturn(null);
        assertFalse(WizardCatalog.boardNameContains(HdConstants.HD_81).test(ctx));
    }

    @Test
    public void boardNameContainsReturnsFalseForNullSignature() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = null;
        UIContext ctx = mock(UIContext.class);
        when(ctx.getBinaryProtocol()).thenReturn(bp);
        assertFalse(WizardCatalog.boardNameContains(HdConstants.HD_81).test(ctx));
    }

    @Test
    public void boardNameContainsReturnsFalseForUnparseableSignature() {
        // Missing "rusEFI " prefix → parse() returns null → predicate is false.
        assertFalse(WizardCatalog.boardNameContains(HdConstants.HD_81).test(contextWithSignature("garbage")));
    }

    private UIContext contextWithSignature(String signature) {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = signature;
        UIContext ctx = mock(UIContext.class);
        when(ctx.getBinaryProtocol()).thenReturn(bp);
        return ctx;
    }
}
