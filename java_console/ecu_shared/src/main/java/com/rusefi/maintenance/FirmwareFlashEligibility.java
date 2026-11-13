package com.rusefi.maintenance;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.io.FirmwareBranch;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.util.Objects;
import java.util.Optional;

/** Side-effect-free eligibility check, apart from reporting a rejection to the caller. */
public final class FirmwareFlashEligibility {
    private FirmwareFlashEligibility() {
    }

    public static boolean isAllowed(@Nullable String currentSignature, @Nullable String firmwareSrecFile,
                                    UpdateOperationCallbacks callbacks) {
        BundleInfo destination = resolveDestination(firmwareSrecFile, BundleUtil.readBundleFullNameNotNull());
        return isAllowed(currentSignature, destination, false, callbacks);
    }

    private static boolean isProviderAllowed(@Nullable String currentSignature, @Nullable BundleInfo destination,
                                             UpdateOperationCallbacks callbacks) {
        try {
            Optional<FirmwareFlashPolicy> policy = FirmwareFlashPolicy.load();
            if (policy.isPresent()) {
                Optional<String> rejection = Objects.requireNonNull(
                    policy.get().getRejectionReason(currentSignature, destination));
                if (rejection.isPresent()) {
                    callbacks.firmwareUpdateBlocked(rejection.get());
                    return false;
                }
            }
        } catch (RuntimeException e) {
            callbacks.firmwareUpdateBlocked("Couldn't check whether this firmware can be installed: " + e.getMessage());
            return false;
        }
        return true;
    }

    static boolean isAllowed(@Nullable String currentSignature, @Nullable BundleInfo destination,
                             boolean confirmedSwitch, UpdateOperationCallbacks callbacks) {
        if (!isProviderAllowed(currentSignature, destination, callbacks)) {
            return false;
        }
        RusEfiSignature source = currentSignature == null ? null : SignatureHelper.parse(currentSignature);
        if (source == null || !knownIdentity(source.getBundleTarget(), source.getBranch())
            || destination == null || BundleInfo.isUndefined(destination)
            || !knownIdentity(destination.getTarget(), destination.getBranchName())) {
            callbacks.firmwareUpdateBlocked("Couldn't identify the ECU or firmware file. The update was stopped.");
            return false;
        }
        if (!source.getBundleTarget().equals(destination.getTarget())) {
            callbacks.firmwareUpdateBlocked("This firmware is for a different ECU board. Please select firmware for your ECU.");
            return false;
        }
        String from = FirmwareBranch.normalize(source.getBranch());
        String to = FirmwareBranch.normalize(destination.getBranchName());
        if (FirmwareBranch.isSameBranch(from, to)) {
            return true;
        }
        if (confirmedSwitch && FirmwareBranch.canSwitch(from, to)) {
            return true;
        }
        callbacks.firmwareUpdateBlocked("This update would change firmware branches, which is not allowed here.");
        return false;
    }

    private static boolean knownIdentity(String board, String branch) {
        return board != null && !board.trim().isEmpty()
            && branch != null && FirmwareBranch.isSameBranch(branch, FirmwareBranch.normalize(branch));
    }

    /** An explicit artifact overrides the bundle, including when its identity is unknown. */
    @Nullable
    static BundleInfo resolveDestination(@Nullable String firmwareSrecFile, BundleInfo bundle) {
        if (firmwareSrecFile == null) {
            return BundleInfo.isUndefined(bundle) ? null : bundle;
        }
        RusEfiSignature signature = SignatureHelper.parseSrec(new File(firmwareSrecFile).getName());
        return signature == null ? null : signature.asBundleInfo();
    }
}
