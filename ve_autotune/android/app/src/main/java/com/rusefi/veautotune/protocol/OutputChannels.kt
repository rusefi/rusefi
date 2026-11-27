package com.rusefi.veautotune.protocol

/**
 * Decoded subset of the ECU's output channels response, as used by the
 * VE autotune recorder. Field offsets/scales come from the plan's Global
 * Constraints (byte offsets into the 214-byte output channels payload,
 * with the leading 0x00 status byte already stripped by the caller).
 */
data class Channels(
    val rpm: Double,
    val map: Double,
    val tps: Double,
    val clt: Double,
    val afr: Double
)

private fun readU16(payload: ByteArray, offset: Int): Int =
    (payload[offset].toInt() and 0xFF) or ((payload[offset + 1].toInt() and 0xFF) shl 8)

private fun readS16(payload: ByteArray, offset: Int): Int =
    readU16(payload, offset).toShort().toInt()

/**
 * Decodes [payload] (the 214-byte output channels buffer, status byte
 * already stripped by the caller) into [Channels].
 *
 * Offsets/scales (Global Constraints), copied verbatim from the .ini
 * source lines cited below, not re-derived:
 *  - rpm: U16 @ 4,   scale 1.0
 *  - map: U16 @ 30,  scale 0.0333333333333333
 *  - tps: S16 @ 22,  scale 0.01
 *  - clt: S16 @ 14,  scale 0.01
 *  - afr: U16 @ 210, scale 0.001 (already the ECU's computed AFR)
 */
fun decodeChannels(payload: ByteArray): Channels {
    // Guard against an undersized payload before reading any field -- the
    // highest byte offset used below is 210 (AFR's U16 read needs bytes
    // 210-211), so anything shorter than 212 bytes would otherwise throw
    // an uncaught IndexOutOfBoundsException instead of a handleable
    // BadResponse. With the caller (RusefiBtConnection) now pre-validating
    // the wire length to exactly 215 bytes (1 status + 214 channel bytes),
    // this should be unreachable via that path -- this is defense-in-depth
    // for decodeChannels being a public function other callers could use
    // directly.
    if (payload.size < 212) {
        throw BadResponse("Channel payload too short: ${payload.size} bytes")
    }
    // RPMValue: U16 offset 4, scale 1.0 (425490096.ini:3336 "RPMValue = scalar, U16, 4, \"RPM\", 1, 0")
    val rpm = readU16(payload, 4) * 1.0
    // MAPValue: U16 offset 30, scale 0.03333333333333333 (425490096.ini:3349 "MAPValue = scalar, U16, 30, \"kPa\", 0.03333333333333333, 0")
    val map = readU16(payload, 30) * 0.0333333333333333
    // TPSValue: S16 offset 22, scale 0.01 (425490096.ini:3345 "TPSValue = scalar, S16, 22, \"%\", 0.01, 0")
    val tps = readS16(payload, 22) * 0.01
    // coolant: S16 offset 14, scale 0.01 (425490096.ini:3341 "coolant = scalar, S16, 14, \"deg C\", 0.01, 0")
    val clt = readS16(payload, 14) * 0.01
    // AFRValue: U16 offset 210, scale 0.001 (425490096.ini:3425 "AFRValue = scalar, U16, 210, \"AFR\", 0.001, 0")
    val afr = readU16(payload, 210) * 0.001
    return Channels(rpm = rpm, map = map, tps = tps, clt = clt, afr = afr)
}
