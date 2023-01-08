package com.rusefi.core;

public class RusEfiSignature {
    private final String branch;
    private final String year;
    private final String month;
    private final String day;
    private final String bundle;
    private final String hash;

    public RusEfiSignature(String branch, String year, String month, String day, String bundle, String hash) {
        this.branch = branch;
        this.year = year;
        this.month = month;
        this.day = day;
        this.bundle = bundle;
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

    public String getBundle() {
        return bundle;
    }

    public String getHash() {
        return hash;
    }
}
