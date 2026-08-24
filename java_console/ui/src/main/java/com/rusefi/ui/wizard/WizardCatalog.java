package com.rusefi.ui.wizard;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.hd81.HdConstants;

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

    private static final Predicate<UIContext> NOT_HD81 = boardNameContains(HdConstants.HD_81).negate();

    public static final List<WizardStepDescriptor> ALL = Collections.unmodifiableList(Arrays.asList(
        // ----- Flagged full-wizard steps (order matters; drives step numbering) -----
        new WizardStepDescriptor(
            "wizardNumberOfCylinders",
            "Cylinders",
            true,
            NOT_HD81,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardFiringOrder",
            "Firing Order",
            true,
            NOT_HD81,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardDisplacement",
            "Base VE Table",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardMapSensorType",
            "MAP Sensor",
            true,
            NOT_HD81,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardTps",
            "TPS",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardCltSensor",
            "CLT Sensor",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardCrankTrigger",
            "Crank Trigger",
            true,
            NOT_HD81,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardCamTrigger",
            "Cam Trigger",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardIgnitionOutputs",
            "Ignition Outputs",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardInjectorOutputs",
            "Injector Outputs",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),
        new WizardStepDescriptor(
            "wizardInjectorFlow",
            "Injector Flow",
            true,
            WizardStepDescriptor.ALWAYS,
            null,
            null
        ),

        // ----- Standalone auto-launch steps -----
        new WizardStepDescriptor(
            null,
            null,
            true,
            boardNameContains(HdConstants.HD_81),
            ctx -> GenericFieldsPanel.anyFieldEmpty(ctx, VIN_FIELDS),
            ctx -> new GenericFieldsPanel(
                ctx,
                "Enter Vehicle Identification Number (VIN) to continue",
                null,
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
