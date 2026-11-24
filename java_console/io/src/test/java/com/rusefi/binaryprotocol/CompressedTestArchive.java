package com.rusefi.binaryprotocol;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.concurrent.TimeUnit;

final class CompressedTestArchive {
    private CompressedTestArchive() {
    }

    static Path create(Path archive, Path sourceRoot, String member, byte[] contents) throws IOException {
        Path source = sourceRoot.resolve(member).normalize();
        Files.createDirectories(source.getParent());
        Files.write(source, contents);
        Process process = new ProcessBuilder(testExecutable(), "a", "-t7z", "-mx=1", "-bd", "-y",
            archive.toAbsolutePath().toString(), "--", member)
            .directory(sourceRoot.toFile()).redirectErrorStream(true).start();
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
            throw new IOException("7z fixture creation failed: " + new String(output.toByteArray(), StandardCharsets.UTF_8));
        }
        Files.deleteIfExists(source);
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
        return "7z";
    }
}
