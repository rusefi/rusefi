package com.rusefi.core.io;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.InvalidPathException;

import static com.devexperts.logging.Logging.getLogging;

public class BundleUtil {
    private static final Logging log = getLogging(BundleUtil.class);

    private static final char BUNDLE_TOKEN_SEPARATOR = '.';
    private static final String SNAPSHOT = "snapshot";

    /**
     * @return null in case of error
     */
    @Nullable
    public static String readBundleFullName() {
        try {
            Path path = Paths.get("").toAbsolutePath();
            String fullName = path.getParent().getFileName().toString();
            if (fullName.length() < 3)
                return null; // just paranoia check
            return fullName;
        } catch (InvalidPathException e) {
            log.error("Error reading bundle name", e);
            return null;
        }
    }

    @NotNull
    public static String readBundleFullNameNotNull() {
        String bundle = readBundleFullName();
        bundle = bundle == null ? "unknown bundle" : bundle;
        return bundle;
    }

    public static String getBundleTarget() {
        return getBundleTarget(readBundleFullName());
    }

    public static BundleInfo parse(String bundleFullName) {
        String[] bundleFullNameSplit = bundleFullName.split("\\" + BundleUtil.BUNDLE_TOKEN_SEPARATOR);
        if (bundleFullNameSplit.length != 3)
            throw new IllegalStateException("Unexpected parent folder name/bundleFullName [" + bundleFullName + "] exactly two dots expected");
        String branchName = bundleFullNameSplit[1];
        String target = bundleFullNameSplit[2];
        return new BundleInfo(branchName, target);
    }

    public static String getBundleTarget(String s) {
        return parse(s).getTarget();
    }

    public static class BundleInfo {
        private final String branchName;
        private final String target;

        public BundleInfo(String branchName, String target) {
            this.branchName = branchName;
            this.target = target;
        }

        public String getBranchName() {
            return branchName;
        }

        public boolean isMaster() {
            return SNAPSHOT.equals(branchName);
        }

        public String getTarget() {
            return target;
        }

        @Override
        public String toString() {
            return "BundleInfo{" +
                "branchName='" + branchName + '\'' +
                ", target='" + target + '\'' +
                '}';
        }
    }
}
