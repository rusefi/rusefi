package com.rusefi.core;

import com.rusefi.core.io.BundleUtil;

public class RusEfiSignature {
    private final String branch;
    private final String year;
    private final String month;
    private final String day;
    private final String bundleTarget;
    private final String hash;

    public RusEfiSignature(String branch, String year, String month, String day, String bundleTarget, String hash) {
        this.branch = branch;
        this.year = year;
        this.month = month;
        this.day = day;
        this.bundleTarget = bundleTarget;
        this.hash = hash;
    }

    public String getBranch() {
        return branch;
    }

    public String getYear() {
        return year;
    }

    public String getMonth() {
        return month;
    }

    public String getDay() {
        return day;
    }

    public String getBundleTarget() {
        return bundleTarget;
    }

    public String getHash() {
        return hash;
    }

    public boolean matchesBundle() {
        String bundleSignature = BundleUtil.getBundleTarget();

        if (bundleSignature == null || this.bundleTarget == null) {
            // If the bundle signature isn't valid, return that it matches (renamed folder?)
            return true;
        }

        return bundleSignature.equalsIgnoreCase(this.bundleTarget);
    }
}
