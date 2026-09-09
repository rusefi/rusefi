package com.rusefi.maintenance;

import com.rusefi.core.io.BundleInfo;
import org.jetbrains.annotations.Nullable;

import java.util.Iterator;
import java.util.Optional;
import java.util.ServiceConfigurationError;
import java.util.ServiceLoader;

/**
 * Optional board-specific eligibility policy for firmware updates, independent of
 * the flashing transport or bootloader. Update flows invoke this policy before
 * handing off the connection or modifying the ECU.
 */
public interface FirmwareFlashPolicy {
    /**
     * Empty permits the update under this policy; otherwise returns the reason to show to the user.
     * The current signature identifies the installed firmware, not the selected artifact;
     * null means the installed firmware's identity is unavailable.
     * A null destination means the selected artifact's identity is unavailable.
     * Implementations must not perform hardware operations.
     */
    Optional<String> getRejectionReason(@Nullable String currentSignature, @Nullable BundleInfo destination);

    static Optional<FirmwareFlashPolicy> load() {
        ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
        if (classLoader == null) {
            classLoader = FirmwareFlashPolicy.class.getClassLoader();
        }
        try {
            Iterator<FirmwareFlashPolicy> providers = ServiceLoader.load(FirmwareFlashPolicy.class, classLoader).iterator();
            if (!providers.hasNext()) {
                return Optional.empty();
            }
            FirmwareFlashPolicy policy = providers.next();
            if (providers.hasNext()) {
                throw new IllegalStateException("Multiple FirmwareFlashPolicy providers found");
            }
            return Optional.of(policy);
        } catch (ServiceConfigurationError e) {
            throw new IllegalStateException("Failed to load FirmwareFlashPolicy", e);
        }
    }
}
