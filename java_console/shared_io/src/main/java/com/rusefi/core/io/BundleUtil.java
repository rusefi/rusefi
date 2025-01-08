package com.rusefi.core.io;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import static com.devexperts.logging.Logging.getLogging;

public class BundleUtil {
    private static final Logging log = getLogging(BundleUtil.class);

    private static final String SNAPSHOT = "snapshot";
    private static final String BRANCH_REF_FILE = "release.txt";

    /**
     * @return null in case of error
     */
    public static List<@NotNull String> readBundleFullName() {
        File f = new File(BRANCH_REF_FILE);
        if (!f.exists()) {
            log.error(BRANCH_REF_FILE + " not found");
            return null;
        }
        try {
            return Files.readAllLines(f.toPath());
        } catch (InvalidPathException | IOException e) {
            log.error("Error reading bundle name", e);
            return null;
        }
    }

    @NotNull
    public static BundleInfo readBundleFullNameNotNull() {
        List<@NotNull String> info = readBundleFullName();
        if (info == null)
            return BundleInfo.UNKNOWN;
        return parse(info);
    }

    public static String getBundleTarget() {
        return readBundleFullNameNotNull().getTarget();
    }

    public static BundleInfo parse(List<@NotNull String> info) {
        Map<String, String> keyValues = new TreeMap(String.CASE_INSENSITIVE_ORDER);
        for (String line : info) {
            String pair[] = line.split("=", 2);
            keyValues.put(pair[0], pair[1]);
        }
        String target = keyValues.get("target");
        String branchName = keyValues.get("release");
        return new BundleInfo(branchName, target);
    }

    public static class BundleInfo {
        static final BundleInfo UNKNOWN = new BundleInfo("unknown", "unknown");

        private final String branchName;
        private final String target;

        public BundleInfo(String branchName, String target) {
            this.branchName = branchName;
            this.target = target;
        }

        public static boolean isUndefined(BundleInfo bundleInfo) {
            return bundleInfo == UNKNOWN;
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

        public String getUiLabel() {
            return target + "." + branchName;
        }
    }
}
