package com.rusefi.maintenance;

import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.FirmwareBranch;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

/** Validated firmware artifacts bound to the ECU identity and exact artifact bytes observed at selection. */
public final class FirmwareSelection {
    private final String sourceSignature;
    private final BundleInfo destination;
    private final @Nullable String srecPath;
    private final @Nullable String binPath;
    private final byte[] srecFingerprint;
    private final byte[] binFingerprint;
    private final boolean confirmedSwitch;

    /** Package-private local-file seam for deterministic fingerprint and transport tests. */
    FirmwareSelection(String sourceSignature, BundleInfo destination, @Nullable String srecPath,
                      @Nullable String binPath, boolean confirmedSwitch) throws IOException, NoSuchAlgorithmException {
        this.sourceSignature = sourceSignature;
        this.destination = destination;
        this.srecPath = srecPath;
        this.binPath = binPath;
        this.confirmedSwitch = confirmedSwitch;
        this.srecFingerprint = fingerprint(srecPath);
        this.binFingerprint = fingerprint(binPath);
    }

    public static @Nullable FirmwareSelection latest(String currentSignature, @Nullable String requestedBranch,
            boolean explicitSwitchConfirmed, UpdateOperationCallbacks callbacks) {
        RusEfiSignature source = SignatureHelper.parse(currentSignature);
        if (source == null) {
            callbacks.firmwareUpdateBlocked("Couldn't identify the ECU or firmware file. The update was stopped.");
            return null;
        }
        String branch = requestedBranch == null ? source.getBranch() : requestedBranch;
        BundleInfo destination = new BundleInfo(FirmwareBranch.normalize(branch), null, source.getBundleTarget());
        if (!FirmwareFlashEligibility.isAllowed(currentSignature, destination, explicitSwitchConfirmed, callbacks)) {
            return null;
        }
        Autoupdate.FirmwareArtifacts artifacts = Autoupdate.downloadFirmwareForBranch(destination,
            callbacks::updateProgress, callbacks::logLine);
        if (artifacts == null || !sameIdentity(destination, artifacts.getBundleInfo())) {
            callbacks.firmwareUpdateBlocked("Couldn't download firmware for " + destination.getTarget()
                + " (branch: " + destination.getBranchName() + "). Please try again.");
            return null;
        }
        return verifiedSelection(currentSignature, destination, artifacts.getSrecPath().orElse(null),
            artifacts.getBinPath().orElse(null), explicitSwitchConfirmed, callbacks);
    }

    /** Historical/recovery artifacts require validated release metadata; legacy SREC may use its parsed filename identity. */
    public static @Nullable FirmwareSelection fromArtifacts(String currentSignature, BundleInfo destination,
            Autoupdate.FirmwareArtifacts artifacts, boolean confirmedSwitch) {
        return fromArtifacts(currentSignature, destination, artifacts, confirmedSwitch, UpdateOperationCallbacks.DUMMY);
    }

    public static @Nullable FirmwareSelection fromArtifacts(String currentSignature, BundleInfo destination,
            Autoupdate.FirmwareArtifacts artifacts, boolean confirmedSwitch, UpdateOperationCallbacks callbacks) {
        if (artifacts == null || destination == null) {
            callbacks.firmwareUpdateBlocked("Couldn't verify the board and branch for this firmware. Please choose another version.");
            return null;
        }
        BundleInfo metadata = artifacts.getBundleInfo();
        if (metadata == null) {
            if (artifacts.getBinPath().isPresent()) {
                callbacks.firmwareUpdateBlocked("This firmware is missing the information needed to check its board and branch. Please choose another version.");
                return null;
            }
            String srec = artifacts.getSrecPath().orElse(null);
            RusEfiSignature inferred = srec == null ? null : SignatureHelper.parseSrec(Paths.get(srec).getFileName().toString());
            if (inferred == null || !sameIdentity(destination, inferred.asBundleInfo())) {
                callbacks.firmwareUpdateBlocked("Couldn't verify the board and branch for this firmware. Please choose another version.");
                return null;
            }
        } else if (!sameIdentity(destination, metadata)) {
            callbacks.firmwareUpdateBlocked("This firmware does not match the board or branch you selected. Please choose another version.");
            return null;
        }
        if (!FirmwareFlashEligibility.isAllowed(currentSignature, destination, confirmedSwitch, callbacks)) {
            return null;
        }
        return verifiedSelection(currentSignature, destination, artifacts.getSrecPath().orElse(null),
            artifacts.getBinPath().orElse(null), confirmedSwitch, callbacks);
    }

    /** Automatic explicit SREC paths are accepted only when their filename identity is known and eligible. */
    public static @Nullable FirmwareSelection fromSrec(String currentSignature, String srecPath,
            UpdateOperationCallbacks callbacks) {
        RusEfiSignature signature = SignatureHelper.parseSrec(Paths.get(srecPath).getFileName().toString());
        if (signature == null) {
            callbacks.firmwareUpdateBlocked("Couldn't identify the board and branch from the firmware filename. Please select a file with its original name.");
            return null;
        }
        BundleInfo destination = signature.asBundleInfo();
        if (!FirmwareFlashEligibility.isAllowed(currentSignature, destination, false, callbacks)) {
            return null;
        }
        return verifiedSelection(currentSignature, destination, srecPath, null, false, callbacks);
    }

    private static @Nullable FirmwareSelection verifiedSelection(String sourceSignature, BundleInfo destination,
            @Nullable String srecPath, @Nullable String binPath, boolean confirmedSwitch,
            UpdateOperationCallbacks callbacks) {
        try {
            FirmwareSelection selection = new FirmwareSelection(sourceSignature, destination, srecPath, binPath, confirmedSwitch);
            if (!selection.hasAnyArtifact()) {
                callbacks.firmwareUpdateBlocked("No usable firmware file was found. Please download the firmware again.");
                return null;
            }
            return selection;
        } catch (IOException | NoSuchAlgorithmException | RuntimeException e) {
            callbacks.firmwareUpdateBlocked("Couldn't check the firmware file: " + e.getMessage());
            return null;
        }
    }

    public boolean revalidate(String currentSignature, UpdateOperationCallbacks callbacks) {
        if (!sourceSignature.equals(currentSignature)) {
            callbacks.firmwareUpdateBlocked("The connected ECU or its firmware has changed. Please restart the update.");
            return false;
        }
        if (!FirmwareFlashEligibility.isAllowed(currentSignature, destination, confirmedSwitch, callbacks)) {
            return false;
        }
        if (!sameArtifact(srecPath, srecFingerprint) || !sameArtifact(binPath, binFingerprint)) {
            callbacks.firmwareUpdateBlocked("The firmware file has changed or cannot be read. Please download it again.");
            return false;
        }
        return true;
    }

    public boolean revalidateForSrec(String currentSignature, UpdateOperationCallbacks callbacks) {
        return revalidate(currentSignature, callbacks) && requireArtifact(srecPath, "SREC", callbacks);
    }

    public boolean revalidateForBin(String currentSignature, UpdateOperationCallbacks callbacks) {
        return revalidate(currentSignature, callbacks) && requireArtifact(binPath, "BIN", callbacks);
    }

    public String getSrecPath() { return srecPath; }
    public String getBinPath() { return binPath; }
    public BundleInfo getDestination() { return destination; }

    public boolean matchesSrecPath(@Nullable String otherPath) {
        return matchesPath(srecPath, otherPath);
    }

    public boolean matchesBinPath(@Nullable String otherPath) {
        return matchesPath(binPath, otherPath);
    }

    private static boolean matchesPath(@Nullable String selected, @Nullable String otherPath) {
        if (otherPath == null) {
            return true;
        }
        if (selected == null) {
            return false;
        }
        try {
            return Paths.get(selected).toAbsolutePath().normalize()
                .equals(Paths.get(otherPath).toAbsolutePath().normalize());
        } catch (RuntimeException e) {
            return false;
        }
    }

    private boolean hasAnyArtifact() {
        return srecFingerprint != null || binFingerprint != null;
    }

    private static boolean requireArtifact(@Nullable String path, String type, UpdateOperationCallbacks callbacks) {
        if (path != null) {
            return true;
        }
        callbacks.firmwareUpdateBlocked("This download is missing the " + type + " file needed for this update.");
        return false;
    }

    private static @Nullable byte[] fingerprint(@Nullable String path) throws IOException, NoSuchAlgorithmException {
        if (path == null) {
            return null;
        }
        Path file = Paths.get(path);
        if (!Files.isRegularFile(file) || !Files.isReadable(file)) {
            throw new IOException("Cannot read firmware file: " + path);
        }
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        try (InputStream input = Files.newInputStream(file)) {
            byte[] buffer = new byte[8192];
            int count;
            while ((count = input.read(buffer)) != -1) {
                digest.update(buffer, 0, count);
            }
        }
        return digest.digest();
    }

    private static boolean sameArtifact(@Nullable String path, @Nullable byte[] expected) {
        if (path == null || expected == null) {
            return path == null && expected == null;
        }
        try {
            return Arrays.equals(expected, fingerprint(path));
        } catch (IOException | NoSuchAlgorithmException e) {
            return false;
        }
    }

    private static boolean sameIdentity(@Nullable BundleInfo first, @Nullable BundleInfo second) {
        return first != null && second != null && first.getTarget() != null && second.getTarget() != null
            && first.getTarget().equals(second.getTarget())
            && FirmwareBranch.isSameBranch(first.getBranchName(), second.getBranchName());
    }
}
