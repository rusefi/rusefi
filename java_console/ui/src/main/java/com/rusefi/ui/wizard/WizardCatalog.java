package com.rusefi.ui.wizard;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.ui.UIContext;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.function.Predicate;

/**
 * Single source of truth mapping wizard configurations to their metadata.
 * <p>
 * Flagged entries drive the numbered full wizard (by {@code flagName} order).
 * Standalone entries drive connect-time auto-launch checks (e.g. empty VIN).
 */
public class WizardCatalog {
    private static final List<String> VIN_FIELDS = Collections.singletonList("vinNumber");

    private static final Predicate<UIContext> NOT_HD81 = boardNameContains("hd81").negate();

    public static final List<WizardStepDescriptor> ALL = Collections.unmodifiableList(Arrays.asList(
        // ----- Flagged full-wizard steps (order matters; drives step numbering) -----
        new WizardStepDescriptor(
            "wizardNumberOfCylinders",
            true,
            NOT_HD81,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardFiringOrder",
            true,
            NOT_HD81,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardMapSensorType",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardCrankTrigger",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardCamTrigger",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardInjectorFlow",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),

        // ----- Standalone auto-launch steps -----
        new WizardStepDescriptor(
            null,
            true,
            boardNameContains("hd81"),
            ctx -> GenericFieldsPanel.anyFieldEmpty(ctx, VIN_FIELDS),
            ctx -> new GenericFieldsPanel(
                ctx,
                "Enter Vehicle Identification Number",
                "Your ECU's VIN is empty. Enter the VIN to continue.",
                VIN_FIELDS,
                null)
        )
    ));

    /** Predicate: true when the connected ECU's bundleTarget (board name) contains {@code needle} (case-insensitive). */
    public static Predicate<UIContext> boardNameContains(String needle) {
        String n = needle.toLowerCase();
        return ctx -> {
            BinaryProtocol bp = ctx.getBinaryProtocol();
            if (bp == null || bp.signature == null) return false;
            RusEfiSignature parsed = SignatureHelper.parse(bp.signature);
            if (parsed == null) return false;
            String target = parsed.getBundleTarget();
            return target != null && target.toLowerCase().contains(n);
        };
    }

    /** Flagged steps only, in declaration order. Drives full wizard step numbering. */
    public static List<WizardStepDescriptor> flaggedSteps() {
        return ALL.stream().filter(WizardStepDescriptor::isFlagged).collect(java.util.stream.Collectors.toList());
    }

    /** Standalone auto-launch candidates. */
    public static List<WizardStepDescriptor> standaloneAutoLaunch() {
        return ALL.stream()
            .filter(d -> !d.isFlagged() && d.autoLaunch)
            .collect(java.util.stream.Collectors.toList());
    }
}
