package com.rusefi.ui.wizard;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.function.Predicate;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class WizardCatalogTest {

    @Test
    public void flaggedStepsAreAllFlagged() {
        List<WizardStepDescriptor> flagged = WizardCatalog.flaggedSteps();
        assertFalse(flagged.isEmpty());
        for (WizardStepDescriptor d : flagged) {
            assertTrue(d.isFlagged(), "flaggedSteps() returned non-flagged entry: " + d.flagName);
        }
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
        Predicate<UIContext> hd81 = WizardCatalog.boardNameContains("hd81");
        assertTrue(hd81.test(contextWithSignature("rusEFI master.2026.04.18.hd81-gdi.abc123")));
        assertTrue(hd81.test(contextWithSignature("rusEFI master.2026.04.18.HD81-gdi.abc123")));
    }

    @Test
    public void boardNameContainsRejectsNonMatchingBoard() {
        Predicate<UIContext> hd81 = WizardCatalog.boardNameContains("hd81");
        assertFalse(hd81.test(contextWithSignature("rusEFI master.2026.04.18.proteus_f7.abc123")));
    }

    @Test
    public void boardNameContainsReturnsFalseForNullBinaryProtocol() {
        UIContext ctx = mock(UIContext.class);
        when(ctx.getBinaryProtocol()).thenReturn(null);
        assertFalse(WizardCatalog.boardNameContains("hd81").test(ctx));
    }

    @Test
    public void boardNameContainsReturnsFalseForNullSignature() {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = null;
        UIContext ctx = mock(UIContext.class);
        when(ctx.getBinaryProtocol()).thenReturn(bp);
        assertFalse(WizardCatalog.boardNameContains("hd81").test(ctx));
    }

    @Test
    public void boardNameContainsReturnsFalseForUnparseableSignature() {
        // Missing "rusEFI " prefix → parse() returns null → predicate is false.
        assertFalse(WizardCatalog.boardNameContains("hd81").test(contextWithSignature("garbage")));
    }

    private UIContext contextWithSignature(String signature) {
        BinaryProtocol bp = mock(BinaryProtocol.class);
        bp.signature = signature;
        UIContext ctx = mock(UIContext.class);
        when(ctx.getBinaryProtocol()).thenReturn(bp);
        return ctx;
    }
}
