package com.opensr5.io;

import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageWithMeta;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.util.Enumeration;
import java.util.Optional;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

/**
 * Utility class to read/write {@link ConfigurationImage} into a file
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/20/2015.
 */
public class ConfigurationImageFile {
    private static final Log log = LogFactory.getLog(ConfigurationImageFile.class);

    private static final String IMAGE_ZIP_ENTRY_NAME = "Image.bin";

    private ConfigurationImageFile() {
    }

    public static ConfigurationImageWithMeta readFromFile(final String fileName) throws IOException {
        try (final ZipFile zipFile = new ZipFile(fileName)) {
            Optional<ConfigurationImageMeta> meta = Optional.empty();
            Enumeration<? extends ZipEntry> zipEntries = zipFile.entries();
            while (zipEntries.hasMoreElements()) {
                final ZipEntry zipEntry = zipEntries.nextElement();
                final String zipEntryName = zipEntry.getName();
                if (ConfigurationImageMeta.getZipEntryName(ConfigurationImageMetaVersion0_0.class).equals(zipEntryName)) {
                    if (!meta.isPresent()) {
                        final ConfigurationImageMetaVersion0_0 metaVersion0_0 = ConfigurationImageMetaYamlUtil.loadMeta(
                            ConfigurationImageMetaVersion0_0.class,
                            zipFile.getInputStream(zipEntry)
                        );
                        meta = Optional.of(metaVersion0_0);
                    }
                } else if (IMAGE_ZIP_ENTRY_NAME.equals(zipEntryName)) {
                    // just skip
                } else {
                    log.warn(String.format("Unexpected entry: `%s` in file `%s`", zipEntryName, fileName));
                }
            }
            if (meta.isPresent()) {
                final ConfigurationImageMeta foundMeta = meta.get();
                final ZipEntry calibrationsZipEntry = zipFile.getEntry(IMAGE_ZIP_ENTRY_NAME);
                try (final InputStream calibrationsInputStream = zipFile.getInputStream(calibrationsZipEntry)) {
                    final byte[] content = readFromStream(foundMeta.getImageSize(), calibrationsInputStream);
                    if (content != null) {
                        return new ConfigurationImageWithMeta(foundMeta, content);
                    } else {
                        log.warn(String.format("Loading content from file `%s` failed", fileName));
                    }
                }
            } else {
                log.warn(String.format("Meta is not found in file `%s`", fileName));
            }
            return null;
        }
    }

    private static byte @Nullable [] readFromStream(final int size, final InputStream fis) throws IOException {
        final byte[] image = new byte[size];
        final int result = fis.read(image);
        if (result == image.length) {
            log.warn(String.format("%d bytes are read instead of expected %d", result, size));
            return image;
        } else {
            return null;
        }
    }

    private static byte[] getFileContent(ConfigurationImage configurationImage) {
        try {
            try (ByteArrayOutputStream baos = new ByteArrayOutputStream()) {
                baos.write(configurationImage.getContent());
                return baos.toByteArray();
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public static void saveToFile(
        final ConfigurationImageWithMeta configurationImage,
        final String fileName
    ) throws IOException {
        System.out.printf("Saving " + configurationImage.getSize() + " bytes of configuration into " + fileName);
        final File outputFile = new File(fileName);
        final byte[] calibrationsFileContent = getFileContent(configurationImage);
        final int calibrationsFileSize = calibrationsFileContent.length;
        try (
            final FileOutputStream fos = new FileOutputStream(outputFile);
            final ZipOutputStream zos = new ZipOutputStream(fos)
        ) {
            final ZipEntry calibrationsZipEntry = new ZipEntry(IMAGE_ZIP_ENTRY_NAME);
            zos.putNextEntry(calibrationsZipEntry);
            calibrationsZipEntry.setSize(calibrationsFileSize);
            zos.write(calibrationsFileContent);
            zos.closeEntry();

            final ConfigurationImageMeta meta = configurationImage.getMeta();
            final ZipEntry metaZipEntry = new ZipEntry(meta.getZipEntryName());
            zos.putNextEntry(metaZipEntry);
            ConfigurationImageMetaYamlUtil.dump(meta, zos);
            zos.closeEntry();
        }
    }
}
