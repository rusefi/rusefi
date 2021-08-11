package com.rusefi.bom;

/**
 * (c) Andrey Belomutskiy
 * 2/2/14
 */
public class BomRecord {
    private final String mfgPart;
    private final String storePart;
    private final String customerRef;
    private final int padCount;
    private final String userComment;

    public BomRecord(String mfgPart, String storePart, String customerRef, int padCount, String userComment) {
        this.mfgPart = mfgPart;
        this.storePart = storePart;
        this.customerRef = customerRef;
        this.padCount = padCount;
        this.userComment = userComment;
    }

    public String getMfgPart() {
        return mfgPart;
    }

    public String getStorePart() {
        return storePart;
    }

    public String getCustomerRef() {
        return customerRef;
    }

    public int getPadCount() {
        return padCount;
    }

    public String getUserComment() {
        return userComment;
    }
}
