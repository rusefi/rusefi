package com.rusefi.core;

public class RusEfiSignature {
    private final String branch;
    private final String year;
    private final String month;
    private final String day;
    private final String bundleTarget;
    private final String hash;

    public RusEfiSignature(
        final String branch,
        final String year,
        final String month,
        final String day,
        final String bundleTarget,
        final String hash
    ) {
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
}
