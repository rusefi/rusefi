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
 * Offsets/scales (Global Constraints):
 *  - rpm: U16 @ 4,   scale 1.0
 *  - map: U16 @ 30,  scale 0.0333333333333333
 *  - tps: S16 @ 22,  scale 0.01
 *  - clt: S16 @ 14,  scale 0.01
 *  - afr: U16 @ 210, scale 0.001 (already the ECU's computed AFR)
 */
fun decodeChannels(payload: ByteArray): Channels {
    val rpm = readU16(payload, 4) * 1.0
    val map = readU16(payload, 30) * 0.0333333333333333
    val tps = readS16(payload, 22) * 0.01
    val clt = readS16(payload, 14) * 0.01
    val afr = readU16(payload, 210) * 0.001
    return Channels(rpm = rpm, map = map, tps = tps, clt = clt, afr = afr)
}
