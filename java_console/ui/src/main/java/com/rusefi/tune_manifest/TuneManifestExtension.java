package com.rusefi.tune_manifest;

import com.rusefi.AvailableHardware;
import org.json.simple.JSONObject;

import java.util.Iterator;
import java.util.Optional;
import java.util.ServiceConfigurationError;
import java.util.ServiceLoader;

/** Optional custom-board tune manifest parsing and selection. */
public interface TuneManifestExtension {
    default TuneModel parse(JSONObject source, TuneModel standardModel) {
        return standardModel;
    }

    default boolean includeTune(TuneModel tune, AvailableHardware hardware) {
        return true;
    }

    default String getSelectionWarning(AvailableHardware hardware) {
        return null;
    }

    static Optional<TuneManifestExtension> load() {
        ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
        if (classLoader == null) {
            classLoader = TuneManifestExtension.class.getClassLoader();
        }

        try {
            Iterator<TuneManifestExtension> providers = ServiceLoader.load(TuneManifestExtension.class, classLoader).iterator();
            if (!providers.hasNext()) {
                return Optional.empty();
            }

            TuneManifestExtension extension = providers.next();
            if (providers.hasNext()) {
                throw new IllegalStateException("Multiple TuneManifestExtension providers found");
            }
            return Optional.of(extension);
        } catch (ServiceConfigurationError e) {
            throw new IllegalStateException("Failed to load TuneManifestExtension", e);
        }
    }
}
