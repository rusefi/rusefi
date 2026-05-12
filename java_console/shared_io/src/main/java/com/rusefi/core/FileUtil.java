package com.rusefi.core;

import com.devexperts.logging.Logging;

import java.io.*;
import java.nio.channels.FileChannel;
import java.util.function.Predicate;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Minor mess: we also have FileUtils in io
 */
public class FileUtil {
    private static final Logging log = getLogging(FileUtil.class);
    public static final String RUSEFI_SETTINGS_FOLDER = System.getProperty("user.home") + File.separator + ".rusEFI" + File.separator;

    public static void unzip(
        final String zipFileName,
        final File destDir,
        final Predicate<ZipEntry> filter
    ) throws IOException {
        byte[] buffer = new byte[1024];
        ZipInputStream zis = new ZipInputStream(new FileInputStream(zipFileName));
        ZipEntry zipEntry = zis.getNextEntry();
        while (zipEntry != null) {
            /**
             * drama: java API does not have access to file permission meta info
             * apache commons compress does
             * https://stackoverflow.com/questions/1050560/maintain-file-permissions-when-extracting-from-a-zip-file-using-jdk-5-api
             */
            if (filter.test(zipEntry)) {
                File newFile = newFile(destDir, zipEntry);
                if (zipEntry.isDirectory()) {
                    if (!newFile.isDirectory()) {
                        log.info("we already have a file with name matching directory name: " + newFile);
                        newFile.delete();
                    }
                    log.info("mkdirs " + newFile);
                    newFile.mkdirs();
                } else {
                    unzipFile(buffer, zis, newFile);
                }
            }
            zipEntry = zis.getNextEntry();
        }
        zis.closeEntry();
        zis.close();
        log.info("Unzip " + zipFileName + " to " + destDir + " worked!");
    }

    private static void unzipFile(byte[] buffer, ZipInputStream zis, File newFile) throws IOException {
        log.info("Unzipping " + newFile);
        FileOutputStream fos = new FileOutputStream(newFile);
        int len;
        while ((len = zis.read(buffer)) > 0) {
            fos.write(buffer, 0, len);
        }
        fos.close();
    }

    private static File newFile(File destinationDir, ZipEntry zipEntry) throws IOException {
        File destFile = new File(destinationDir, zipEntry.getName());

        String destDirPath = destinationDir.getCanonicalPath();
        String destFilePath = destFile.getCanonicalPath();

        if (!destFilePath.startsWith(destDirPath + File.separator)) {
            throw new IOException("Entry is outside of the target dir: " + zipEntry.getName());
        }

        return destFile;
    }

    public static void close(Closeable closeable) {
        if (closeable != null) {
            try {
                closeable.close();
            } catch (IOException ignored) {
                // ignored
            }
        }
    }

    public static void copyFile(String sourceFile, String destinationFile) throws IOException {
        FileChannel sourceFileChannel = new FileInputStream(sourceFile).getChannel();
        FileChannel dest = new FileOutputStream(destinationFile).getChannel();
        dest.transferFrom(sourceFileChannel, 0, sourceFileChannel.size());
    }
}
