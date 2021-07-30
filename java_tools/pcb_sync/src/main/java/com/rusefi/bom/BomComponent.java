package com.rusefi.bom;

/**
 * (c) Andrey Belomutskiy
 * 2/2/14
 */
public class BomComponent {
    private final String reference;

    public BomComponent(String reference) {
        this.reference = reference;
    }

    public String getReference() {
        return reference;
    }

    @Override
    public String toString() {
        return reference;
    }
}
