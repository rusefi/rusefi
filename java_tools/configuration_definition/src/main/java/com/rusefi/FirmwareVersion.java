package com.rusefi;

import org.jetbrains.annotations.NotNull;

public class FirmwareVersion {
    private final long crc32;

    public FirmwareVersion(long crc32) {

        this.crc32 = crc32;
    }

    @NotNull
    public String encode() {
        return Long.toString(crc32);
    }
}
