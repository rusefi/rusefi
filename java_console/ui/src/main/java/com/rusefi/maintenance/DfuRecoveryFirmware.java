package com.rusefi.maintenance;

import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleInfoStrategy;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.io.UpdateOperationCallbacks;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Locale;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/** Prepares one public full firmware image for a manual DFU recovery. */
public final class DfuRecoveryFirmware {
    private static final String DFU_ERROR = "No DFU image is available for this board. Use OpenBLT recovery if this ECU requires OpenBLT.";

    private DfuRecoveryFirmware() {
    }

    /**
     * Downloads and extracts exactly one full DFU image for {@code target}.
     * The returned image is deliberately not staged in the console's shared firmware location.
     */
    public static String prepare(String target, UpdateOperationCallbacks callbacks) throws IOException {
        validateTarget(target);

        BundleInfo local = BundleUtil.readBundleFullNameNotNull();
        String branch = BundleInfo.isUndefined(local) ? "master" : local.getBranchName();
        BundleInfo wanted = new BundleInfo(branch, null, target);
        String baseUrl = BundleInfoStrategy.getDownloadUrl(wanted, PropertiesHolder.getBaseUrl(), BundleInfo::getBranchName);
        String zip = Autoupdate.downloadZipForTarget(wanted, baseUrl,
            callbacks::updateProgress, callbacks::logLine);
        if (zip == null) {
            throw noDfuImage("The public firmware archive could not be downloaded");
        }

        return extractDfuImage(target, Path.of(zip), Path.of(zip).toAbsolutePath().getParent());
    }

    /** Visible for offline fixture tests; always extracts into a newly-created directory. */
    static String extractDfuImage(String target, Path zip, Path extractionParent) throws IOException {
        validateTarget(target);
        if (isObfuscated(zip.getFileName().toString())) {
            throw noDfuImage("The selected archive is obfuscated");
        }

        List<ZipEntry> candidates = new ArrayList<>();
        try (ZipFile archive = new ZipFile(zip.toFile())) {
            Enumeration<? extends ZipEntry> entries = archive.entries();
            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                validateBinEntry(entry, target);
                if (isDfuCandidate(entry, target)) {
                    candidates.add(entry);
                }
            }
            if (candidates.size() != 1) {
                throw noDfuImage(candidates.isEmpty() ? "The archive contains no matching full .bin" : "The archive contains multiple full .bin images");
            }

            ZipEntry image = candidates.get(0);
            if (image.getSize() == 0) {
                throw noDfuImage("The full .bin is empty");
            }

            Path destinationDirectory = Files.createTempDirectory(extractionParent, "dfu-" + target + "-");
            Path destination = destinationDirectory.resolve(image.getName());
            try (InputStream input = archive.getInputStream(image)) {
                Files.copy(input, destination, StandardCopyOption.REPLACE_EXISTING);
            }
            if (Files.size(destination) == 0) {
                Files.deleteIfExists(destination);
                throw noDfuImage("The extracted full .bin is empty");
            }
            return destination.toAbsolutePath().toString();
        }
    }

    private static boolean isDfuCandidate(ZipEntry entry, String target) {
        if (entry.isDirectory() || !isRootFile(entry.getName())) {
            return false;
        }
        String name = entry.getName().toLowerCase(Locale.ROOT);
        if (isObfuscated(name) || !name.endsWith(".bin")) {
            return false;
        }
        if (name.equals("rusefi.bin")) {
            return true;
        }
        if (!name.startsWith("rusefi_")) {
            return false;
        }
        return hasTarget(name, target);
    }

    private static void validateBinEntry(ZipEntry entry, String target) throws IOException {
        if (entry.isDirectory() || !isRootFile(entry.getName())) {
            return;
        }
        String name = entry.getName().toLowerCase(Locale.ROOT);
        if (!name.endsWith(".bin") || !name.startsWith("rusefi")) {
            return;
        }
        if (isObfuscated(name)) {
            throw noDfuImage("The archive contains an obfuscated .bin");
        }
        if (name.startsWith("rusefi_") && !hasTarget(name, target)) {
            throw noDfuImage("The archive full .bin is for a different board target");
        }
    }

    private static boolean hasTarget(String name, String target) {
        String expectedTarget = target.toLowerCase(Locale.ROOT);
        return name.equals("rusefi_" + expectedTarget + ".bin")
            || expectedTarget.equals(FindFileHelper.extractTargetFromFirmwareName(name));
    }

    private static boolean isRootFile(String name) {
        return !name.contains("/") && !name.contains("\\");
    }

    private static boolean isObfuscated(String name) {
        return name.toLowerCase(Locale.ROOT).contains("obfuscated");
    }

    private static void validateTarget(String target) throws IOException {
        if (target == null || !target.matches("[A-Za-z0-9][A-Za-z0-9_-]*")
            || ManualDfuRecovery.isUniversalBundle(target)) {
            throw noDfuImage("Invalid board target");
        }
    }

    private static IOException noDfuImage(String detail) {
        return new IOException(DFU_ERROR + " " + detail + ".");
    }
}
