package com.rusefi.core.io;

import java.util.Objects;

public class BundleInfo {
    public static final String DEVELOPMENT = "development";
    public static final String UNIVERSAL = "universal";
    static final BundleInfo UNKNOWN = new BundleInfo("unknown", null, "unknown");

    private final String branchName;
    private final String nextBranchName;
    private final String target;

    public BundleInfo(String branchName, String nextBranchName, String target) {
        this.branchName = Objects.requireNonNull(branchName, "branchName");
        this.nextBranchName = nextBranchName;
        this.target = Objects.requireNonNull(target, "target");
    }

    public static boolean isUndefined(BundleInfo bundleInfo) {
        return bundleInfo == UNKNOWN;
    }

    public String getBranchName() {
        return branchName;
    }

    public String getNextBranchName() {
        return nextBranchName;
    }

    public boolean isMaster() {
        return DEVELOPMENT.equals(branchName);
    }

    /** True when running as a universal bundle — skips per-board mismatch checks. */
    public boolean isUniversal() {
        return UNIVERSAL.equals(target);
    }

    public String getTarget() {
        return target;
    }

    @Override
    public String toString() {
        String nextBranchString = nextBranchName != null && !nextBranchName.isEmpty() ? "nextBranchName='" + nextBranchName + '\'' : "";
        return "BundleInfo{" +
            "branchName='" + branchName + '\'' +
            nextBranchString +
            ", target='" + target + '\'' +
            '}';
    }

    public String getUiLabel() {
        return target + "." + branchName;
    }
}
