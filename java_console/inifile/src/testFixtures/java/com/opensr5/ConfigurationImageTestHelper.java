package com.opensr5;

import java.nio.ByteOrder;

/**
 * [tag:image_byte_order] Production code must state the byte order of every image, taken from
 * the controller's ini. Tests of little-endian (rusEFI) content use this shortcut instead.
 */
public class ConfigurationImageTestHelper {
    private ConfigurationImageTestHelper() {
    }

    /** @return a zero-filled image */
    public static ConfigurationImage newLittleEndian(int size) {
        return new ConfigurationImage(size, ByteOrder.LITTLE_ENDIAN);
    }

    public static ConfigurationImage newLittleEndian(byte[] content) {
        return new ConfigurationImage(content, ByteOrder.LITTLE_ENDIAN);
    }
}
