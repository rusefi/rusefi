package com.rusefi.core;

import com.devexperts.logging.Logging;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.nio.file.AtomicMoveNotSupportedException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.Locale;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.FileUtil.RUSEFI_SETTINGS_FOLDER;

public class SignatureHelper {
    private static final Logging log = getLogging(SignatureHelper.class);
    private final static String LOCAL_INI_CACHE_FOLDER = RUSEFI_SETTINGS_FOLDER + "ini_database";
    private static final int HTTP_TIMEOUT_MILLIS = 10_000;
    private static final int MAX_INI_ENTRY_BYTES = 4 * 1024 * 1024;

    // todo: find a way to reference Fields.PROTOCOL_SIGNATURE_PREFIX
    private static final String PREFIX = "rusEFI ";
    private static final char SLASH = '/';
    /**
     * java -Dextra_local_ini_file_name=path_to_local_generated.ini ...
     */
    private static final String EXTRA_INI_SOURCE = System.getProperty("extra_local_ini_file_name");

    public static Pair<String, String> getUrl(String signature) {
        RusEfiSignature s = parse(signature);
        if (s == null)
            return null;

        String fileName = s.getHash() + ".ini";
        return new Pair<>("https://rusefi.com/online/ini/rusefi/" + s.getBranch() + SLASH + s.getYear() + SLASH +
                s.getMonth() + SLASH +
                s.getDay() + SLASH +
                s.getBundleTarget() + SLASH +
                fileName, fileName);
    }

    /**
     * Checks the local cache before optionally contacting the remote INI archive.
     */
    public static String downloadIfNotAvailable(Pair<String, String> p, boolean allowDownload) {
        return downloadIfNotAvailable(p, allowDownload, LOCAL_INI_CACHE_FOLDER);
    }

    static String downloadIfNotAvailable(Pair<String, String> p, boolean allowDownload, String cacheFolder) {
        return downloadIfNotAvailable(p, allowDownload, cacheFolder, null);
    }

    public static String downloadMatchingIniIfNotAvailable(
            Pair<String, String> location, boolean allowDownload, String expectedSignature) {
        return downloadIfNotAvailable(location, allowDownload, LOCAL_INI_CACHE_FOLDER, expectedSignature);
    }

    static String downloadMatchingIniIfNotAvailableForTests(
            Pair<String, String> location, boolean allowDownload, String cacheFolder, String expectedSignature) {
        return downloadIfNotAvailable(location, allowDownload, cacheFolder, expectedSignature);
    }

    private static String downloadIfNotAvailable(Pair<String, String> p, boolean allowDownload,
                                                 String cacheFolder, String expectedSignature) {
        if (p == null) {
            return null;
        }
        new File(cacheFolder).mkdirs();
        String localIniFile = cacheFolder + File.separator + p.second;
        File file = new File(localIniFile);
        if (file.exists() && file.length() > 10000) {
            if (expectedSignature == null || hasSignature(file, expectedSignature)) {
                log.info("Found cached at " + cacheFolder);
                return localIniFile;
            }
            log.info("Ignoring cached INI with a different signature at " + localIniFile);
        }
        if (EXTRA_INI_SOURCE != null) {
            if (expectedSignature == null
                    || hasSignature(new File(EXTRA_INI_SOURCE), expectedSignature)) {
                return EXTRA_INI_SOURCE;
            }
        }
        if (!allowDownload) {
            return null;
        }
        log.info(".ini not found in " + cacheFolder + "(" + localIniFile + "), trying to download " + p.first);

        // Validate in a temporary file before replacing a mismatched cache candidate.
        File tempFile = new File(localIniFile + ".tmp");
        HttpURLConnection httpURLConnection = null;
        try {
            httpURLConnection = (HttpURLConnection) new URL(p.first).openConnection();
            httpURLConnection.setConnectTimeout(HTTP_TIMEOUT_MILLIS);
            httpURLConnection.setReadTimeout(HTTP_TIMEOUT_MILLIS);
            int statusCode = httpURLConnection.getResponseCode();
            if (statusCode >= 300) {
                log.info("Unexpected code " + statusCode);
                tempFile.delete();
                return null;
            }
            try (BufferedInputStream in = new BufferedInputStream(httpURLConnection.getInputStream());
                 FileOutputStream fileOutputStream = new FileOutputStream(tempFile)) {
                byte[] dataBuffer = new byte[32 * 1024];
                int bytesRead;
                while ((bytesRead = in.read(dataBuffer, 0, dataBuffer.length)) != -1) {
                    fileOutputStream.write(dataBuffer, 0, bytesRead);
                }
            }
            if (expectedSignature != null && !hasSignature(tempFile, expectedSignature)) {
                tempFile.delete();
                return null;
            }
            try {
                Files.move(tempFile.toPath(), file.toPath(),
                    StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.ATOMIC_MOVE);
            } catch (AtomicMoveNotSupportedException e) {
                Files.move(tempFile.toPath(), file.toPath(), StandardCopyOption.REPLACE_EXISTING);
            }
            return localIniFile;
        } catch (IOException e) {
            System.err.println(e.getMessage());
            tempFile.delete();
            return null;
        } finally {
            if (httpURLConnection != null) {
                httpURLConnection.disconnect();
            }
        }
    }

    private static boolean hasSignature(File file, String expectedSignature) {
        try {
            return expectedSignature.equals(readIniSignature(file));
        } catch (IOException e) {
            return false;
        }
    }

    /**
     * Imports a matching user-selected INI or 7z archive into the local signature-keyed cache.
     * Archive paths are never extracted; the cache is replaced only after exact validation.
     * @return cached file path, or null if the signature cannot be parsed
     */
    public static String importIntoCache(String signature, File source) throws IOException {
        return importIntoCache(signature, source, LOCAL_INI_CACHE_FOLDER);
    }

    /**
     * Imports a matching INI or 7z archive into an explicitly selected cache directory.
     */
    public static String importIntoCache(String signature, File source, String cacheFolder) throws IOException {
        Pair<String, String> location = getUrl(signature);
        if (location == null) {
            return null;
        }
        byte[] ini = readMatchingIni(signature, source);
        checkInterrupted();
        return publishIni(signature, location, ini, cacheFolder);
    }

    public static String importIntoCache(String signature, byte[] source) throws IOException {
        return importIntoCache(signature, source, LOCAL_INI_CACHE_FOLDER);
    }

    /** Imports a validated, already-read INI snapshot into an explicit cache directory. */
    public static String importIntoCache(String signature, byte[] source, String cacheFolder) throws IOException {
        Pair<String, String> location = getUrl(signature);
        if (location == null) {
            return null;
        }
        if (source.length > MAX_INI_ENTRY_BYTES) {
            throw new IOException("INI exceeds the " + MAX_INI_ENTRY_BYTES + " byte limit");
        }
        byte[] snapshot = java.util.Arrays.copyOf(source, source.length);
        if (isOpaqueBinary(snapshot)) {
            throw new IOException("Expected plain INI contents, received binary data");
        }
        if (!signature.equals(readIniSignature(snapshot))) {
            throw new IOException("INI signature does not match " + signature);
        }
        checkInterrupted();
        return publishIni(signature, location, snapshot, cacheFolder);
    }

    static String importIntoCacheForTests(String signature, File source, String cacheFolder,
                                          Runnable afterSnapshotRead) throws IOException {
        Pair<String, String> location = getUrl(signature);
        if (location == null) {
            return null;
        }
        byte[] ini = readMatchingIni(signature, source);
        checkInterrupted();
        afterSnapshotRead.run();
        checkInterrupted();
        return publishIni(signature, location, ini, cacheFolder);
    }

    private static String publishIni(String signature, Pair<String, String> location, byte[] contents,
                                     String cacheFolder) throws IOException {
        Path cache = new File(cacheFolder).toPath();
        Files.createDirectories(cache);
        Path target = cache.resolve(location.second);
        Path temporary = Files.createTempFile(cache, location.second, ".tmp");
        try {
            Files.write(temporary, contents);
            checkInterrupted();
            try {
                Files.move(temporary, target, StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.ATOMIC_MOVE);
            } catch (AtomicMoveNotSupportedException e) {
                Files.move(temporary, target, StandardCopyOption.REPLACE_EXISTING);
            }
        } finally {
            Files.deleteIfExists(temporary);
        }
        String cachedIniFile = target.toString();
        log.info("Imported matching INI for " + signature + " into cache as " + cachedIniFile);
        return cachedIniFile;
    }

    /**
     * Reads a bounded INI snapshot and requires the exact signature. 7z entry paths are never
     * extracted or resolved against the filesystem.
     */
    public static byte[] readMatchingIni(String signature, File source) throws IOException {
        if (getUrl(signature) == null) {
            throw new IOException("Unparseable firmware signature: " + signature);
        }
        byte[] contents;
        String name = source.getName().toLowerCase(Locale.ROOT);
        if (name.endsWith(".7z")) {
            contents = CompressedIniArchive.readMatchingIni(signature, source);
        } else if (name.endsWith(".ini")) {
            contents = readPlainIni(source);
        } else {
            throw new IOException("Expected a plain .ini or .7z archive");
        }
        if (!signature.equals(readIniSignature(contents))) {
            throw new IOException("INI signature does not match " + signature);
        }
        return contents;
    }

    public static String readIniSignature(File source) throws IOException {
        return readIniSignature(readPlainIni(source));
    }

    private static byte[] readBounded(InputStream input, int maximumBytes) throws IOException {
        ByteArrayOutputStream output = new ByteArrayOutputStream(Math.min(maximumBytes, 32 * 1024));
        byte[] buffer = new byte[8192];
        int total = 0;
        while (true) {
            checkInterrupted();
            int read = input.read(buffer);
            if (read == -1) {
                break;
            }
            total += read;
            if (total > maximumBytes) {
                throw new IOException("INI contents exceed the " + maximumBytes + " byte limit");
            }
            output.write(buffer, 0, read);
        }
        return output.toByteArray();
    }

    /**
     * Extracts the first exact, case-insensitive {@code signature} key, matching RawIniFile's key
     * token semantics without depending on the INI model module.
     */
    public static String readIniSignature(byte[] contents) {
        if (isOpaqueBinary(contents)) {
            return null;
        }
        String text = new String(contents, StandardCharsets.UTF_8);
        try (BufferedReader reader = new BufferedReader(new StringReader(text))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] tokens = IniTokenUtil.splitTokens(line);
                if (tokens.length > 1 && "signature".equalsIgnoreCase(tokens[0])) {
                    return tokens[1];
                }
            }
        } catch (IllegalArgumentException malformedCandidate) {
            return null;
        } catch (IOException impossible) {
            throw new IllegalStateException(impossible);
        }
        return null;
    }

    private static byte[] readBoundedFile(File source, int maximumBytes) throws IOException {
        if (source.length() > maximumBytes) {
            throw new IOException("INI exceeds the " + maximumBytes + " byte limit");
        }
        try (InputStream input = new BufferedInputStream(new FileInputStream(source))) {
            return readBounded(input, maximumBytes);
        }
    }

    private static byte[] readPlainIni(File source) throws IOException {
        byte[] contents = readBoundedFile(source, MAX_INI_ENTRY_BYTES);
        if (isOpaqueBinary(contents)) {
            throw new IOException("Expected plain INI contents, received binary data");
        }
        return contents;
    }

    private static boolean isOpaqueBinary(byte[] contents) {
        if (contents.length >= 6
                && contents[0] == 0x37 && contents[1] == 0x7a && (contents[2] & 0xff) == 0xbc
                && (contents[3] & 0xff) == 0xaf && contents[4] == 0x27 && contents[5] == 0x1c) {
            return true;
        }
        for (byte value : contents) {
            int unsigned = value & 0xff;
            if (unsigned < 0x09 || unsigned > 0x0d && unsigned < 0x20) {
                return true;
            }
        }
        return false;
    }

    private static void checkInterrupted() throws InterruptedIOException {
        if (Thread.currentThread().isInterrupted()) {
            throw new InterruptedIOException("INI lookup was cancelled");
        }
    }

    public static RusEfiSignature parseSrec(String srecName) {
        if (srecName == null) {
            return null;
        }
        if (srecName.endsWith(".srec")) {
            srecName = srecName.substring(0, srecName.length() - 5);
        }
        if (srecName.startsWith("rusefi-") && srecName.endsWith("_obfuscated")) {
            // firmware/bundle.mk uses a dash prefix and a suffix for obfuscated update images.
            srecName = "rusefi_" + srecName.substring("rusefi-".length(), srecName.length() - "_obfuscated".length());
        }
        if (srecName.endsWith("_update")) {
            srecName = srecName.substring(0, srecName.length() - 7);
        }
        String[] parts = srecName.split("_");
        // format: rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32
        // or: rusefi_development_2026-04-27_3659024206688255410edc1e751b6736281e0efd
        if (parts.length < 4) {
            return null;
        }
        String branch = parts[1];
        String date = parts[2];
        String[] dateParts = date.split("-");
        if (dateParts.length != 3) {
            return null;
        }
        String year = dateParts[0];
        String month = dateParts[1];
        String day = dateParts[2];

        if (parts.length >= 6) {
            // New format
            // format: rusefi_branch_date_target_number_hash
            // example: rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32
            // parts[0] = rusefi
            // parts[1] = development
            // parts[2] = 2026-05-09
            // parts[parts.length - 1] = hash
            // parts[parts.length - 2] = number
            // target is everything between parts[2] and parts[parts.length - 2]
            StringBuilder targetBuilder = new StringBuilder();
            for (int i = 3; i < parts.length - 2; i++) {
                if (targetBuilder.length() > 0) {
                    targetBuilder.append("_");
                }
                targetBuilder.append(parts[i]);
            }
            String bundleTarget = targetBuilder.toString();
            String hash = parts[parts.length - 2];
            return new RusEfiSignature(branch, year, month, day, bundleTarget, hash, false);
        } else if (parts.length == 4) {
            // Legacy format
            // Legacy format only has the git hash, it does not have the numeric hash
            return new RusEfiSignature(branch, year, month, day, "all", null, true);
        }

        return null;
    }

    public static RusEfiSignature parse(final String signature) {
        if (signature == null || !signature.startsWith(PREFIX))
            return null;
        final String signatureWithoutPrefix = signature.substring(PREFIX.length()).trim();
        final String[] elements = signatureWithoutPrefix.split("\\.");
        if (elements.length != 6)
            return null;

        String branch = elements[0];
        String year = elements[1];
        String month = elements[2];
        String day = elements[3];
        String bundleTarget = elements[4];
        String hash = elements[5];

        return new RusEfiSignature(branch, year, month, day, bundleTarget, hash, false);
    }
}
