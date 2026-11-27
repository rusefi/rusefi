package com.rusefi.veautotune.protocol

import org.junit.Assert.assertEquals
import org.junit.Assert.assertThrows
import org.junit.Test

class OutputChannelsTest {
    @Test
    fun `decodeChannels reads little-endian fields at the documented offsets`() {
        val buf = ByteArray(214)
        // RPM = 1500 at offset 4 (U16 LE, scale 1.0)
        buf[4] = 0xDC.toByte(); buf[5] = 0x05.toByte() // 1500 = 0x05DC
        // MAP = 45.0 kPa at offset 30 (U16 LE, raw = round(45.0 / 0.0333333333333333) = 1350)
        buf[30] = 0x46.toByte(); buf[31] = 0x05.toByte() // 1350 = 0x0546
        // coolant = -5.00 C at offset 14 (S16 LE, raw = -500)
        val cltRaw = (-500).toShort()
        buf[14] = cltRaw.toByte(); buf[15] = (cltRaw.toInt() shr 8).toByte()
        // TPS = 12.34 % at offset 22 (S16 LE, raw = 1234)
        val tpsRaw = (1234).toShort()
        buf[22] = tpsRaw.toByte(); buf[23] = (tpsRaw.toInt() shr 8).toByte()
        // AFR = 14.700 at offset 210 (U16 LE, scale 0.001, raw = 14700 = 0x396C)
        buf[210] = 0x6C.toByte(); buf[211] = 0x39.toByte()

        val c = decodeChannels(buf)

        assertEquals(1500.0, c.rpm, 0.001)
        assertEquals(45.0, c.map, 0.01)
        assertEquals(-5.00, c.clt, 0.01)
        assertEquals(12.34, c.tps, 0.01)
        assertEquals(14.700, c.afr, 0.001)
    }

    @Test
    fun `decodeChannels decodes a negative tps without treating it as unsigned`() {
        val buf = ByteArray(214)
        // TPS = -12.34 % at offset 22 (S16 LE, raw = -1234)
        val tpsRaw = (-1234).toShort()
        buf[22] = tpsRaw.toByte(); buf[23] = (tpsRaw.toInt() shr 8).toByte()

        val c = decodeChannels(buf)

        // If offset 22 were decoded as unsigned, -1234 (raw 0xFB2E = 64302)
        // would wrongly read back as 643.02 instead of -12.34.
        assertEquals(-12.34, c.tps, 0.01)
    }

    @Test
    fun `decodeChannels throws BadResponse on a too-short payload instead of IndexOutOfBoundsException`() {
        // AFR needs bytes 210-211, so a 50-byte payload is far too short.
        // Without the guard this would throw an uncaught
        // IndexOutOfBoundsException instead of a handleable BadResponse.
        val tooShort = ByteArray(50)

        assertThrows(BadResponse::class.java) {
            decodeChannels(tooShort)
        }
    }
}
