package com.rusefi.ui.wizard;

import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import java.util.function.Function;
import java.util.function.Predicate;

import static org.junit.jupiter.api.Assertions.*;

public class WizardStepDescriptorTest {

    @Test
    public void flaggedDescriptorReportsItself() {
        WizardStepDescriptor d = new WizardStepDescriptor(
            "wizardFoo", true, WizardStepDescriptor.ALWAYS, null, null);
        assertTrue(d.isFlagged());
        assertEquals("wizardFoo", d.flagName);
        assertTrue(d.autoLaunch);
    }

    @Test
    public void standaloneDescriptorIsNotFlagged() {
        Function<UIContext, WizardStep> factory = ctx -> null;
        WizardStepDescriptor d = new WizardStepDescriptor(
            null, false, WizardStepDescriptor.ALWAYS, c -> true, factory);
        assertFalse(d.isFlagged());
        assertNull(d.flagName);
        assertSame(factory, d.factory);
    }

    @Test
    public void nullApplicableDefaultsToAlways() {
        WizardStepDescriptor d = new WizardStepDescriptor(
            "wizardFoo", true, null, null, null);
        assertNotNull(d.applicable);
        // ALWAYS must accept any input including null
        assertTrue(d.applicable.test(null));
    }

    @Test
    public void alwaysPredicateAcceptsAnything() {
        Predicate<UIContext> p = WizardStepDescriptor.ALWAYS;
        assertTrue(p.test(null));
        assertTrue(p.test(new UIContext()));
    }
}
