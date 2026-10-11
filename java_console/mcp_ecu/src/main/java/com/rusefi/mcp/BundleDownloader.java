package com.rusefi.mcp;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleInfoStrategy;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.net.PropertiesHolder;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.attribute.FileTime;
import java.util.Comparator;
import java.util.Enumeration;
import java.util.List;
import java.util.Locale;
import java.util.UUID;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/** Downloads autoupdate archives into a new directory without replacing the running installation. */
final class BundleDownloader {
    private static final Logging log = Logging.getLogging(BundleDownloader.class);
    private final String baseUrl;
    private final String whiteLabel;

    BundleDownloader() {
        this(PropertiesHolder.getBaseUrl(), ConnectionAndMeta.getWhiteLabel(ConnectionAndMeta.getProperties()));
    }

    BundleDownloader(String baseUrl, String whiteLabel) {
        this.baseUrl = baseUrl.replaceAll("/+$", "");
        this.whiteLabel = whiteLabel;
    }

    @SuppressWarnings("unchecked")
    JSONObject download(JSONObject arguments) throws IOException {
        String board = optionalString(arguments, "board", null);
        String branch = optionalString(arguments, "branch", BundleInfo.DEVELOPMENT);
        String destination = optionalString(arguments, "destination", null);
        Object universalArgument = arguments.getOrDefault("universal", Boolean.FALSE);
        if (!(universalArgument instanceof Boolean)) {
            throw new IllegalArgumentException("universal must be a boolean");
        }
        boolean universal = (Boolean) universalArgument;
        if (board == null && !universal) {
            throw new IllegalArgumentException("Specify board or set universal to true");
        }
        if (board != null && (!board.matches("[A-Za-z0-9][A-Za-z0-9_-]*") || board.equalsIgnoreCase("universal"))) {
            throw new IllegalArgumentException("board must be a board target; use universal: true for the universal updater");
        }
        if (!branch.matches("[A-Za-z0-9][A-Za-z0-9_.-]*") || branch.contains("..")) {
            throw new IllegalArgumentException("branch must be development, master, or a release name such as lts-26");
        }
        if (branch.equals("master")) {
            branch = BundleInfo.DEVELOPMENT;
        }
        // createDirectory deliberately refuses an existing destination, including an existing installation.
        Path directory = destination == null ? Files.createTempDirectory("rusefi_bundle_")
                : Files.createDirectory(Paths.get(destination).toAbsolutePath().normalize());
        try {
            Path bundle = Files.createDirectory(directory.resolve("bundle"));
            Path archivesDirectory = Files.createDirectory(directory.resolve("archives"));
            JSONArray archives = new JSONArray();
            String target = universal ? "universal" : board;
            Path archive = fetch(target, branch, archivesDirectory, archives);
            extract(archive, bundle, false);
            if (universal && board != null) {
                // Keep the universal console/release identity; add only the selected board's S-records.
                Path firmwareArchive = fetch(board, branch, archivesDirectory, archives);
                extract(firmwareArchive, bundle, true);
            }
            List<Path> srecs;
            try (Stream<Path> files = Files.walk(bundle)) {
                srecs = files.filter(Files::isRegularFile).filter(BundleDownloader::isSrec)
                        .sorted().collect(Collectors.toList());
            }
            if (board != null && srecs.isEmpty()) {
                throw new IOException("Bundle for " + board + " contains no SREC firmware");
            }
            JSONArray srecPaths = new JSONArray();
            for (Path srec : srecs) {
                srecPaths.add(EcuFirmwareUpdater.validateFirmwarePath(srec.toString()).toString());
            }
            JSONObject result = new JSONObject();
            result.put("success", true);
            result.put("board", board);
            result.put("universal", universal);
            result.put("branch", branch);
            result.put("directory", directory.toString());
            result.put("bundleDirectory", bundle.toString());
            result.put("archives", archives);
            result.put("srecPaths", srecPaths);
            result.put("firmwareIncluded", !srecPaths.isEmpty());
            if (srecPaths.size() == 1) {
                result.put("firmwarePath", srecPaths.get(0));
            }
            return result;
        } catch (IOException | RuntimeException e) {
            try {
                deleteDirectory(directory);
            } catch (IOException cleanupFailure) {
                e.addSuppressed(cleanupFailure);
            }
            throw e;
        }
    }

    private static String optionalString(JSONObject arguments, String key, String defaultValue) {
        if (!arguments.containsKey(key)) {
            return defaultValue;
        }
        Object value = arguments.get(key);
        if (!(value instanceof String) || ((String) value).trim().isEmpty()) {
            throw new IllegalArgumentException(key + " must be a non-empty string");
        }
        return (String) value;
    }

    @SuppressWarnings("unchecked")
    private Path fetch(String target, String branch, Path directory, JSONArray archives) throws IOException {
        String url = BundleInfoStrategy.getDownloadUrl(new BundleInfo(branch, null, target), baseUrl,
                BundleInfo::getBranchName);
        String name = whiteLabel + "_bundle_" + target + "_autoupdate.zip";
        Path archive = directory.resolve(name);
        try {
            downloadFile(url + name, archive);
        } catch (FileNotFoundException publicMissing) {
            if (target.equals("universal")) {
                throw publicMissing;
            }
            name = whiteLabel + "_bundle_" + target + "_obfuscated_public_autoupdate.zip";
            archive = directory.resolve(name);
            try {
                downloadFile(url + name, archive);
            } catch (IOException e) {
                e.addSuppressed(publicMissing);
                throw e;
            }
        }
        JSONObject info = new JSONObject();
        info.put("target", target);
        info.put("url", url + name);
        info.put("path", archive.toString());
        info.put("fileSize", Files.size(archive));
        archives.add(info);
        return archive;
    }

    private static void downloadFile(String url, Path destination) throws IOException {
        log.info("download_bundle: " + url);
        // Same server routing and request headers as the autoupdater, but always fetch a fresh copy.
        // Its cache helper prints to stdout, which is reserved for MCP JSON-RPC.
        HttpURLConnection connection = (HttpURLConnection) new URL(url + "?u=" + UUID.randomUUID()).openConnection();
        connection.setConnectTimeout(30_000);
        connection.setReadTimeout(60_000);
        connection.setUseCaches(false);
        connection.setRequestProperty("User-Agent", "RE-Internal-Sync");
        connection.setRequestProperty("Cache-Control", "no-cache, no-store, must-revalidate, max-age=0");
        connection.setRequestProperty("Pragma", "no-cache");
        try {
            int status = connection.getResponseCode();
            if (status == HttpURLConnection.HTTP_NOT_FOUND) {
                throw new FileNotFoundException("Bundle not found: " + url);
            }
            if (status != HttpURLConnection.HTTP_OK) {
                throw new IOException("Bundle download returned HTTP " + status + ": " + url);
            }
            long copied;
            try (InputStream input = connection.getInputStream()) {
                copied = Files.copy(input, destination);
            }
            long expected = connection.getContentLengthLong();
            if (expected >= 0 && copied != expected) {
                throw new IOException("Incomplete bundle download: expected " + expected + " bytes, received " + copied);
            }
            if (connection.getLastModified() > 0) {
                Files.setLastModifiedTime(destination, FileTime.fromMillis(connection.getLastModified()));
            }
        } finally {
            connection.disconnect();
        }
    }

    private static boolean isSrec(Path path) {
        return path.getFileName().toString().toLowerCase(Locale.ROOT).endsWith(".srec");
    }

    private static void extract(Path archive, Path directory, boolean firmwareOnly) throws IOException {
        int files = 0;
        try (ZipFile zip = new ZipFile(archive.toFile())) {
            Enumeration<? extends ZipEntry> entries = zip.entries();
            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                String name = entry.getName().replace('\\', '/');
                Path output = directory.resolve(name).normalize();
                if (!output.startsWith(directory) || name.contains(":")) {
                    throw new IOException("ZIP entry is outside the bundle directory: " + name);
                }
                if (entry.isDirectory()) {
                    if (!firmwareOnly) {
                        Files.createDirectories(output);
                    }
                    continue;
                }
                if (firmwareOnly && !isSrec(output)) {
                    continue;
                }
                Files.createDirectories(output.getParent());
                try (InputStream input = zip.getInputStream(entry)) {
                    Files.copy(input, output);
                }
                files++;
            }
        }
        if (files == 0) {
            throw new IOException("Bundle contains no " + (firmwareOnly ? "SREC firmware" : "files") + ": " + archive);
        }
    }

    private static void deleteDirectory(Path directory) throws IOException {
        List<Path> paths;
        try (Stream<Path> files = Files.walk(directory)) {
            paths = files.sorted(Comparator.reverseOrder()).collect(Collectors.toList());
        }
        for (Path path : paths) {
            Files.delete(path);
        }
    }
}
