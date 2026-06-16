package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;

import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.*;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Downloads board bundle ZIPs in the background after ECU connect.
 * The SREC firmware update file is extracted from the ZIP on demand when the
 * user clicks "Update Firmware".
 *
 * Cache layout:
 *   ~/.rusEFI/bundle_cache/{boardId}/{hash}.zip
 *   ~/.rusEFI/bundle_cache/{boardId}/{hash}_update.srec  (extracted)
 */
public class BackgroundBundleDownloader {
    private static final Logging log = getLogging(BackgroundBundleDownloader.class);

    private static final String BUNDLE_CACHE_DIR =
        FileUtil.RUSEFI_SETTINGS_FOLDER + "bundle_cache";
    private static final int BUFFER_SIZE = 64 * 1024;

    public static final BackgroundBundleDownloader INSTANCE = new BackgroundBundleDownloader();

    // ponytail: simple map, no eviction — cache is on disk anyway
    private final ConcurrentHashMap<String, Future<String>> downloads = new ConcurrentHashMap<>();

    // Board + hash of the most recently connected ECU — set by ensureBundle, used by tryExtractCurrentSrec
    private volatile String currentBoardId;
    private volatile String currentHash;
    private final ExecutorService executor = Executors.newCachedThreadPool(r -> {
        Thread t = new Thread(r, "bundle-downloader");
        t.setDaemon(true);
        return t;
    });

    private static String key(String boardId, String hash) {
        return boardId + "/" + hash;
    }

    private File bundleFile(String boardId, String hash) {
        return new File(BUNDLE_CACHE_DIR + File.separator + boardId + File.separator + hash + ".zip");
    }

    /**
     * Start downloading the bundle ZIP in the background if not already cached.
     * Safe to call multiple times — idempotent.
     */
    public void ensureBundle(String boardId, String hash, String bundleUrl) {
        currentBoardId = boardId;
        currentHash = hash;
        File dest = bundleFile(boardId, hash);
        if (dest.exists() && dest.length() > 0) {
            return;
        }
        downloads.computeIfAbsent(key(boardId, hash), ignored -> {
            log.info("Queuing bundle download: " + boardId + "/" + hash);
            return executor.submit(() -> downloadBundle(boardId, hash, bundleUrl, dest));
        });
    }

    /**
     * Extract the *_update.srec from the cached bundle ZIP.
     * Blocks until the bundle is downloaded (up to {@code timeout}).
     *
     * @return local path to extracted SREC file, or null
     */
    @Nullable
    public String extractSrec(String boardId, String hash, long timeout, TimeUnit unit)
            throws InterruptedException {
        File dest = bundleFile(boardId, hash);
        if (!dest.exists()) {
            Future<String> f = downloads.get(key(boardId, hash));
            if (f == null) {
                return null;
            }
            try {
                f.get(timeout, unit);
            } catch (TimeoutException e) {
                log.info("Bundle download timed out: " + boardId + "/" + hash);
                return null;
            } catch (ExecutionException e) {
                log.info("Bundle download failed: " + e.getMessage());
                return null;
            }
        }
        return extractSrecFromZip(dest.getAbsolutePath(), boardId, hash);
    }

    /**
     * Extract the SREC for the most recently connected ECU.
     * Used by the universal firmware update path — no board/hash needed at the call site.
     *
     * @return local SREC path, or null if no ECU has connected or bundle not ready
     */
    @Nullable
    public String tryExtractCurrentSrec() throws InterruptedException {
        String boardId = currentBoardId;
        String hash = currentHash;
        if (boardId == null || hash == null) {
            return null;
        }
        return extractSrec(boardId, hash, 5, TimeUnit.MINUTES);
    }

    private String downloadBundle(String boardId, String hash, String bundleUrl, File dest) {
        log.info("Downloading bundle " + boardId + "/" + hash + " from " + bundleUrl);
        dest.getParentFile().mkdirs();
        File tmp = new File(dest.getAbsolutePath() + ".tmp");
        try {
            HttpURLConnection conn = (HttpURLConnection) new URL(bundleUrl).openConnection();
            conn.setConnectTimeout(30_000);
            conn.setReadTimeout(120_000);
            try (InputStream in = new BufferedInputStream(conn.getInputStream(), BUFFER_SIZE);
                 FileOutputStream out = new FileOutputStream(tmp)) {
                byte[] buf = new byte[BUFFER_SIZE];
                int n;
                while ((n = in.read(buf)) >= 0) {
                    out.write(buf, 0, n);
                }
            }
            if (!tmp.renameTo(dest)) {
                // rename may fail on Windows across drives
                FileUtil.copyFile(tmp.getAbsolutePath(), dest.getAbsolutePath());
                tmp.delete();
            }
            log.info("Bundle cached: " + dest);
            return dest.getAbsolutePath();
        } catch (IOException e) {
            log.info("Bundle download failed: " + e.getMessage());
            tmp.delete();
            return null;
        }
    }

    @Nullable
    private String extractSrecFromZip(String zipPath, String boardId, String hash) {
        File srecDest = new File(
            BUNDLE_CACHE_DIR + File.separator + boardId + File.separator + hash + "_update.srec");
        if (srecDest.exists() && srecDest.length() > 0) {
            return srecDest.getAbsolutePath();
        }
        try (ZipInputStream zis = new ZipInputStream(new FileInputStream(zipPath))) {
            ZipEntry entry;
            while ((entry = zis.getNextEntry()) != null) {
                // SREC is at any depth inside the ZIP — match by filename suffix
                String name = new File(entry.getName()).getName();
                if (name.endsWith("_update.srec")) {
                    srecDest.getParentFile().mkdirs();
                    try (FileOutputStream fos = new FileOutputStream(srecDest)) {
                        byte[] buf = new byte[BUFFER_SIZE];
                        int n;
                        while ((n = zis.read(buf)) >= 0) {
                            fos.write(buf, 0, n);
                        }
                    }
                    log.info("Extracted SREC: " + srecDest);
                    return srecDest.getAbsolutePath();
                }
            }
        } catch (IOException e) {
            log.info("SREC extraction failed: " + e.getMessage());
        }
        log.info("No *_update.srec found in " + zipPath);
        return null;
    }
}
