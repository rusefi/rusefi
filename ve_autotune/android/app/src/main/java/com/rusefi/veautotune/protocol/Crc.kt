package com.rusefi.veautotune.protocol

import java.util.zip.CRC32

/**
 * Thin wrapper over [java.util.zip.CRC32] so callers don't need to
 * instantiate/reset it themselves. Matches the CRC32 algorithm used by
 * java_console's IoHelper.getCrc32 (same underlying implementation).
 */
fun crc32(bytes: ByteArray): Long {
    val crc = CRC32()
    crc.update(bytes)
    return crc.value
}
