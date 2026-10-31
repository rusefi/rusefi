package com.rusefi.maintenance;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleUtil;
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
        final Optional<String> rejection;
        try {
            Optional<FirmwareFlashPolicy> policy = FirmwareFlashPolicy.load();
            if (!policy.isPresent()) {
                return true;
            }
            BundleInfo destination = resolveDestination(firmwareSrecFile, BundleUtil.readBundleFullNameNotNull());
            rejection = Objects.requireNonNull(policy.get().getRejectionReason(currentSignature, destination));
        } catch (RuntimeException e) {
            callbacks.firmwareUpdateBlocked("Unable to verify firmware flashing eligibility: " + e.getMessage());
            return false;
        }
        if (rejection.isPresent()) {
            callbacks.firmwareUpdateBlocked(rejection.get());
            return false;
        }
        return true;
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
