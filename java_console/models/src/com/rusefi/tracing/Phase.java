package com.rusefi.tracing;

public enum Phase {
    // Begin
    B,
    // End
    E,
    i,
    ;

    public static Phase decode(byte phase) {
        switch (phase) {
            case 0:
                return B;
            case 1:
                return E;
            case 2:
            case 3:
                return i;
            default:
                throw new IllegalStateException("Unexpected " + phase);
        }
    }
}
