package com.rusefi;

public class RusEfiSignature {
    private final String year;
    private final String month;
    private final String day;
    private final String bundle;
    private final String hash;

    public RusEfiSignature(String year, String month, String day, String bundle, String hash) {

        this.year = year;
        this.month = month;
        this.day = day;
        this.bundle = bundle;
        this.hash = hash;
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
