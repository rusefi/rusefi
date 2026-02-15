package com.rusefi.util;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.OrdinalOutOfRangeException;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.tune.xml.Msq;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import jakarta.xml.bind.JAXBException;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class TuneBackupUtil {
    private static final Logging log = getLogging(TuneBackupUtil.class);

    public static void saveConfigurationImageToFiles(
        final ConfigurationImageWithMeta imageWithMeta,
        final IniFileModel ini,
        @Nullable final String binaryFileName,
        @Nullable final String xmlFileName
    ) throws JAXBException, IOException {
        if (binaryFileName != null) {
            ConfigurationImageFile.saveToFile(imageWithMeta, binaryFileName);
        }
        if (xmlFileName != null) {
            saveXmlFile(imageWithMeta, ini, xmlFileName);
        }
    }

    private static void saveXmlFile(
        final ConfigurationImageWithMeta imageWithMeta,
        final IniFileModel ini,
        @NotNull final String xmlFileName
    ) throws JAXBException, IOException {
        ConfigurationImage image = imageWithMeta.getConfigurationImage();
        if (image == null) {
            log.warn("No image for saveConfigurationImageToFiles");
            return;
        }
        try {
            final Msq tune = MsqFactory.valueOf(image, ini);
            tune.writeXmlFile(xmlFileName);
        } catch (final OrdinalOutOfRangeException e) {
            log.warn("OrdinalOutOfRangeException exception on saving .msq file:" + e, e);
            throw e;
        }
    }
}

