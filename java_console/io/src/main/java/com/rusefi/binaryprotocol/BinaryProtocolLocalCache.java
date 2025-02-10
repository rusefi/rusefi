package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.io.ConfigurationImageFile;
import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class BinaryProtocolLocalCache {
    private static final Logging log = getLogging(BinaryProtocol.class);

    public static final String STATE_FOLDER = "state/";
    static final String CONFIGURATION_RUSEFI_XML = STATE_FOLDER + "current_configuration.msq";
    static final String CONFIGURATION_RUSEFI_BINARY = STATE_FOLDER + "current_configuration.zip";

    public static boolean DISABLE_LOCAL_CONFIGURATION_CACHE;

    static {
        new File(BinaryProtocolLocalCache.STATE_FOLDER).mkdirs();
    }

    @NotNull
    static ConfigurationImageWithMeta getAndValidateLocallyCached(BinaryProtocol binaryProtocol) {
        if (DISABLE_LOCAL_CONFIGURATION_CACHE)
            return ConfigurationImageWithMeta.VOID;
        ConfigurationImageWithMeta localCached;
        try {
            localCached = ConfigurationImageFile.readFromFile(CONFIGURATION_RUSEFI_BINARY);
        } catch (IOException e) {
            log.error("Error reading " + CONFIGURATION_RUSEFI_BINARY + ": no worries " + e);
            return ConfigurationImageWithMeta.VOID;
        }

        if (!localCached.isEmpty()) {
            int crcOfLocallyCachedConfiguration = IoHelper.getCrc32(localCached.getConfigurationImage().getContent());
            log.info(String.format(CONFIGURATION_RUSEFI_BINARY + " Local cache CRC %x\n", crcOfLocallyCachedConfiguration));

            // there is a local file! let's request CRC from controller so that we can compare it to local file (validate)
            int crcFromController = binaryProtocol.getCrcFromController(localCached.getConfigurationImage().getSize());

            if (crcOfLocallyCachedConfiguration == crcFromController) {
                return localCached;
            }

        }
        return ConfigurationImageWithMeta.VOID;
    }
}
