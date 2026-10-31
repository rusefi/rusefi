package com.opensr5.io;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageWithMeta;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.util.Enumeration;
import java.util.Map;
import java.util.Optional;
import java.util.TreeMap;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Utility class to read/write {@link ConfigurationImage} into a file.
 * A configuration image file is a zip archive containing the following two files:
 * 1. <b>Image.bin</b> - a binary file with an image binary representation
 * 2. <b>${CONFIGURATION_IMAGE_META}.yaml</b> - a yaml file with an image binary metadata, where
 *    ${CONFIGURATION_IMAGE_META} is a name of the corresponding class, inherited from {@link ConfigurationImageMeta}
 *    class. Currently, we support only {@link ConfigurationImageMetaVersion0_0} value for ${CONFIGURATION_IMAGE_META},
 *    but in the future we could extend list of supported image binary metadata formats.
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/20/2015.
 */
public class ConfigurationImageFile {
    private static final Logging log = getLogging(ConfigurationImageFile.class);

    private static final String IMAGE_ZIP_ENTRY_NAME = "Image.bin";
    private static final String PAGE_ZIP_ENTRY_FORMAT = "Page-%04X.bin";

    private ConfigurationImageFile() {
    }

    @NotNull
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
                } else if (IMAGE_ZIP_ENTRY_NAME.equals(zipEntryName) || isPageEntry(zipEntryName)) {
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
            return ConfigurationImageWithMeta.VOID;
        }
    }

    private static byte @Nullable [] readFromStream(final int size, final InputStream fis) throws IOException {
        final byte[] image = new byte[size];
        final int result = fis.read(image);
        if (result == image.length) {
            return image;
        } else {
            log.warn(String.format("%d bytes are read instead of expected %d", result, size));
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
        final ConfigurationImageWithMeta configurationImageWithMeta,
        final String fileName
    ) throws IOException {
        saveToFile(java.util.Collections.singletonMap(0, configurationImageWithMeta), fileName);
    }

    public static void saveToFile(
        final Map<Integer, ConfigurationImageWithMeta> pages,
        final String fileName
    ) throws IOException {
        ConfigurationImageWithMeta mainImage = pages.get(0);
        if (mainImage == null) {
            throw new IllegalArgumentException("Main configuration page is missing");
        }
        ConfigurationImage configurationImage = mainImage.getConfigurationImage();
        if (configurationImage == null) {
            log.warn("No image to save");
            return;
        }
        log.info(String.format("Saving %d bytes of configuration into %s", configurationImage.getSize(), fileName));
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

            for (Map.Entry<Integer, ConfigurationImageWithMeta> page : pages.entrySet()) {
                if (page.getKey() == 0) {
                    continue;
                }
                final ZipEntry pageEntry = new ZipEntry(String.format(PAGE_ZIP_ENTRY_FORMAT, page.getKey()));
                zos.putNextEntry(pageEntry);
                zos.write(getFileContent(page.getValue().getConfigurationImage()));
                zos.closeEntry();
            }

            final ConfigurationImageMeta meta = mainImage.getMeta();
            final ZipEntry metaZipEntry = new ZipEntry(meta.getZipEntryName());
            zos.putNextEntry(metaZipEntry);
            ConfigurationImageMetaYamlUtil.dump(meta, zos);
            zos.closeEntry();
        }
    }

    public static Map<Integer, ConfigurationImageWithMeta> readPagesFromFile(final String fileName) throws IOException {
        final Map<Integer, ConfigurationImageWithMeta> pages = new TreeMap<>();
        final ConfigurationImageWithMeta mainImage = readFromFile(fileName);
        pages.put(0, mainImage);
        try (final ZipFile zipFile = new ZipFile(fileName)) {
            final Enumeration<? extends ZipEntry> entries = zipFile.entries();
            while (entries.hasMoreElements()) {
                final ZipEntry entry = entries.nextElement();
                if (!isPageEntry(entry.getName())) {
                    continue;
                }
                final int pageIdentifier = Integer.parseInt(entry.getName().substring(5, 9), 16);
                final byte[] content;
                try (InputStream input = zipFile.getInputStream(entry)) {
                    ByteArrayOutputStream output = new ByteArrayOutputStream();
                    byte[] buffer = new byte[4096];
                    int count;
                    while ((count = input.read(buffer)) != -1) {
                        output.write(buffer, 0, count);
                    }
                    content = output.toByteArray();
                }
                pages.put(
                    pageIdentifier,
                    new ConfigurationImageWithMeta(
                        new ConfigurationImageMetaVersion0_0(content.length, mainImage.getMeta().getEcuSignature()),
                        content
                    )
                );
            }
        }
        return pages;
    }

    private static boolean isPageEntry(String name) {
        return name.matches("Page-[0-9A-Fa-f]{4}\\.bin");
    }
}
