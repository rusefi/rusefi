package com.rusefi.veautotune.protocol

import org.junit.Assert.assertArrayEquals
import org.junit.Assert.assertEquals
import org.junit.Assert.assertThrows
import org.junit.Test

private const val CHANNEL_BUFFER_SIZE = 214

/**
 * Test-only helper that wraps arbitrary inner bytes in the outer envelope
 * format: [2 bytes BE length of inner][inner bytes][4 bytes BE CRC32 of inner].
 * Mirrors IoHelper.makeCrc32Packet (java_console/io/src/main/java/com/rusefi/
 * binaryprotocol/IoHelper.java). buildOutputChannelRequest only ever builds
 * 'O' request frames, so response fixtures (status byte + data) need this
 * separate helper rather than reusing production code.
 */
private fun wrapInOuterEnvelope(inner: ByteArray): ByteArray {
    val outer = ByteArray(2 + inner.size + 4)
    outer[0] = ((inner.size shr 8) and 0xFF).toByte()
    outer[1] = (inner.size and 0xFF).toByte()
    System.arraycopy(inner, 0, outer, 2, inner.size)
    val crc = crc32(inner)
    outer[outer.size - 4] = ((crc shr 24) and 0xFF).toByte()
    outer[outer.size - 3] = ((crc shr 16) and 0xFF).toByte()
    outer[outer.size - 2] = ((crc shr 8) and 0xFF).toByte()
    outer[outer.size - 1] = (crc and 0xFF).toByte()
    return outer
}

/** Builds a valid output-channel response frame: 0x00 status + payload + correct CRC. */
private fun validOutputChannelResponseFrame(
    payload: ByteArray = ByteArray(CHANNEL_BUFFER_SIZE)
): ByteArray {
    val inner = ByteArray(1 + payload.size)
    inner[0] = 0x00
    System.arraycopy(payload, 0, inner, 1, payload.size)
    return wrapInOuterEnvelope(inner)
}

class FrameTest {

    @Test
    fun `buildOutputChannelRequest encodes command and little-endian offset-count`() {
        val req = buildOutputChannelRequest(0, 214)
        // inner = ['O', 0x00,0x00, 0xD6,0x00] = 5 bytes; outer length prefix = 5
        assertEquals(0, req[0].toInt()); assertEquals(5, req[1].toInt())
        assertEquals('O'.code.toByte(), req[2])
        assertEquals(0, req[3].toInt()); assertEquals(0, req[4].toInt())       // offset=0 LE
        assertEquals(0xD6.toByte(), req[5]); assertEquals(0x00.toByte(), req[6]) // count=214 LE
        assertEquals(11, req.size) // 2 (len) + 5 (inner) + 4 (crc)
    }

    @Test
    fun `buildOutputChannelRequest encodes a non-zero offset little-endian`() {
        val req = buildOutputChannelRequest(0x1234, 0x0001)
        assertEquals('O'.code.toByte(), req[2])
        assertEquals(0x34.toByte(), req[3]); assertEquals(0x12.toByte(), req[4]) // offset LE
        assertEquals(0x01.toByte(), req[5]); assertEquals(0x00.toByte(), req[6]) // count LE
    }

    @Test
    fun `parseResponse rejects bad crc`() {
        val good = validOutputChannelResponseFrame()
        val corrupted = good.copyOf().also { it[it.size - 1] = (it[it.size - 1] + 1).toByte() }
        assertThrows(BadResponse::class.java) { parseResponse(corrupted) }
    }

    @Test
    fun `parseResponse rejects non-OK status byte`() {
        val payload = ByteArray(CHANNEL_BUFFER_SIZE)
        val inner = ByteArray(1 + payload.size)
        inner[0] = 0x01 // not OK -- ECU rejected the request
        System.arraycopy(payload, 0, inner, 1, payload.size)
        val frame = wrapInOuterEnvelope(inner)

        assertThrows(BadResponse::class.java) { parseResponse(frame) }
    }

    @Test
    fun `parseResponse returns 214-byte payload on success`() {
        val frame = validOutputChannelResponseFrame()
        assertEquals(214, parseResponse(frame).size)
    }

    @Test
    fun `parseResponse returns the actual payload bytes untouched`() {
        val payload = ByteArray(CHANNEL_BUFFER_SIZE) { i -> i.toByte() }
        val frame = validOutputChannelResponseFrame(payload)

        assertArrayEquals(payload, parseResponse(frame))
    }
}
