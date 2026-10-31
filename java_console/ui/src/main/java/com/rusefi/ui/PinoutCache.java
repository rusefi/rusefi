package com.rusefi.ui;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.AtomicMoveNotSupportedException;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.List;
import java.util.zip.ZipFile;

final class PinoutCache {
    @FunctionalInterface
    interface Download {
        void downloadTo(File destination) throws IOException;
    }

    private PinoutCache() {
    }

    static boolean isCurrent(File cachedZip, String expectedSha) {
        if (cachedZip == null || !cachedZip.isFile()) return false;
        if (expectedSha == null || expectedSha.trim().isEmpty()) return true;
        try {
            return expectedSha.equalsIgnoreCase(sha256(cachedZip));
        } catch (IOException e) {
            return false;
        }
    }

    static boolean isCurrent(File cachedZip, String expectedSha, List<String> expectedEntries) {
        return isCurrent(cachedZip, expectedSha) && containsEntries(cachedZip, expectedEntries);
    }

    static boolean containsEntries(File cachedZip, List<String> expectedEntries) {
        if (cachedZip == null || !cachedZip.isFile() || expectedEntries == null) return false;
        try (ZipFile zip = new ZipFile(cachedZip)) {
            for (String entry : expectedEntries) {
                if (zip.getEntry(entry) == null) return false;
            }
            return true;
        } catch (IOException e) {
            return false;
        }
    }

    static void downloadAndReplace(File target, String expectedSha, Download download) throws IOException {
        downloadAndReplace(target, expectedSha, null, download);
    }

    static void downloadAndReplace(File target, String expectedSha, List<String> expectedEntries, Download download) throws IOException {
        File parent = target.getParentFile();
        if (parent != null && !parent.isDirectory() && !parent.mkdirs() && !parent.isDirectory()) {
            throw new IOException("Could not create pinout cache directory " + parent);
        }

        File temporary = File.createTempFile("pinout-", ".download", parent);
        try {
            download.downloadTo(temporary);
            if (!isCurrent(temporary, expectedSha)) {
                throw new IOException("Downloaded pinout archive SHA-256 does not match metadata");
            }
            if (expectedEntries != null && !containsEntries(temporary, expectedEntries)) {
                throw new IOException("Downloaded pinout archive does not contain this board's connectors");
            }
            replace(temporary, target);
        } finally {
            Files.deleteIfExists(temporary.toPath());
        }
    }

    static void writeAndReplace(File target, String content) throws IOException {
        File parent = target.getParentFile();
        if (parent != null && !parent.isDirectory() && !parent.mkdirs() && !parent.isDirectory()) {
            throw new IOException("Could not create pinout cache directory " + parent);
        }

        File temporary = File.createTempFile("pinout-meta-", ".download", parent);
        try {
            Files.writeString(temporary.toPath(), content);
            replace(temporary, target);
        } finally {
            Files.deleteIfExists(temporary.toPath());
        }
    }

    static String sha256(File file) throws IOException {
        MessageDigest digest;
        try {
            digest = MessageDigest.getInstance("SHA-256");
        } catch (NoSuchAlgorithmException e) {
            throw new IllegalStateException("SHA-256 is not available", e);
        }

        try (InputStream input = Files.newInputStream(file.toPath())) {
            byte[] buffer = new byte[64 * 1024];
            int read;
            while ((read = input.read(buffer)) != -1) {
                if (read > 0) digest.update(buffer, 0, read);
            }
        }

        StringBuilder result = new StringBuilder(64);
        for (byte value : digest.digest()) result.append(String.format("%02x", value));
        return result.toString();
    }

    private static void replace(File source, File target) throws IOException {
        try {
            Files.move(source.toPath(), target.toPath(),
                StandardCopyOption.ATOMIC_MOVE, StandardCopyOption.REPLACE_EXISTING);
        } catch (AtomicMoveNotSupportedException e) {
            Files.move(source.toPath(), target.toPath(), StandardCopyOption.REPLACE_EXISTING);
        }
    }
}
