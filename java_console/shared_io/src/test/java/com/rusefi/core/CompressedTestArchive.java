package com.rusefi.core;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

/** Creates local fixtures with installed Linux 7z or an explicitly configured Windows 7za.exe. */
final class CompressedTestArchive {
    private CompressedTestArchive() {
    }

    static Path create(Path archive, Path sourceRoot, Entry... entries) throws IOException {
        Files.createDirectories(sourceRoot);
        List<String> members = new ArrayList<>();
        for (Entry entry : entries) {
            Path source = sourceRoot.resolve(entry.archivePath).normalize();
            Files.createDirectories(source.getParent());
            Files.write(source, entry.contents);
            members.add(entry.archivePath);
        }

        List<String> command = new ArrayList<>();
        command.add(testExecutable());
        command.add("a");
        command.add("-t7z");
        command.add("-mx=1");
        command.add("-bd");
        command.add("-y");
        command.add(archive.toAbsolutePath().toString());
        command.add("--");
        command.addAll(members);

        Process process = new ProcessBuilder(command).directory(sourceRoot.toFile())
            .redirectErrorStream(true).start();
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        try (InputStream input = process.getInputStream()) {
            byte[] buffer = new byte[1024];
            int read;
            while ((read = input.read(buffer)) != -1) {
                if (output.size() + read > 64 * 1024) {
                    process.destroyForcibly();
                    throw new IOException("7z fixture tool output exceeded its limit");
                }
                output.write(buffer, 0, read);
            }
        }
        try {
            if (!process.waitFor(20, TimeUnit.SECONDS)) {
                process.destroyForcibly();
                throw new IOException("7z fixture creation timed out");
            }
        } catch (InterruptedException e) {
            process.destroyForcibly();
            Thread.currentThread().interrupt();
            throw new IOException("7z fixture creation was interrupted", e);
        }
        if (process.exitValue() != 0) {
            throw new IOException("7z fixture creation failed: " + output.toString("UTF-8"));
        }
        for (Entry entry : entries) {
            Path source = sourceRoot.resolve(entry.archivePath).normalize();
            Files.deleteIfExists(source);
        }
        return archive;
    }

    private static String testExecutable() throws IOException {
        String configured = System.getProperty("rusefi.test.7z");
        if (configured != null && !configured.isEmpty()) {
            return configured;
        }
        if (System.getProperty("os.name", "").toLowerCase(java.util.Locale.ROOT).contains("windows")) {
            throw new IOException("Set -Drusefi.test.7z to the bundled 7za.exe for Windows fixture generation");
        }
        return CompressedIniArchive.resolveExecutable();
    }

    static final class Entry {
        final String archivePath;
        final byte[] contents;

        Entry(String archivePath, byte[] contents) {
            this.archivePath = archivePath;
            this.contents = contents;
        }
    }
}
