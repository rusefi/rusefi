package com.rusefi.veautotune.bt

import com.rusefi.veautotune.protocol.BadResponse
import com.rusefi.veautotune.protocol.crc32
import org.junit.Assert.assertEquals
import org.junit.Assert.assertThrows
import org.junit.Test

private const val CHANNEL_BUFFER_SIZE = 214

/**
 * Test-only helper that wraps arbitrary inner bytes in the outer envelope
 * format: [2 bytes BE length of inner][inner bytes][4 bytes BE CRC32 of inner].
 * Mirrors Task 3's FrameTest helper -- duplicated here (rather than shared)
 * since it is test fixture code, not production code.
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
private fun validOutputChannelResponseFrame(payload: ByteArray): ByteArray {
    val inner = ByteArray(1 + payload.size)
    inner[0] = 0x00
    System.arraycopy(payload, 0, inner, 1, payload.size)
    return wrapInOuterEnvelope(inner)
}

class FakeTransport(private val response: ByteArray) : ByteTransport {
    val written = mutableListOf<ByteArray>()
    private var pos = 0
    override fun write(bytes: ByteArray) {
        written.add(bytes)
    }
    override fun readFully(n: Int): ByteArray {
        val chunk = response.copyOfRange(pos, pos + n)
        pos += n
        return chunk
    }
}

class RusefiBtConnectionTest {

    @Test
    fun `readChannels sends one request and decodes the response`() {
        val payload = ByteArray(CHANNEL_BUFFER_SIZE)
        // RPM = 1500 at offset 4 (U16 LE, scale 1.0)
        payload[4] = 0xDC.toByte(); payload[5] = 0x05.toByte() // 1500 = 0x05DC
        val validResponseFrame = validOutputChannelResponseFrame(payload)

        val transport = FakeTransport(validResponseFrame)
        val channels = RusefiBtConnection(transport).readChannels()

        assertEquals(1500.0, channels.rpm, 0.001)
        assertEquals(1, transport.written.size)
    }

    @Test
    fun `readChannels throws BadResponse on a bogus length prefix instead of hanging`() {
        // A corrupted length prefix declaring 10 bytes instead of the
        // expected 215 (1 status + 214 channel bytes). The bytes after the
        // length prefix don't matter -- readChannels must reject the
        // length before ever calling readFully(innerLength) with it, so a
        // short/garbage tail here must not make it hang or decode garbage.
        val bogusLengthPrefix = byteArrayOf(0x00, 0x0A)
        val transport = FakeTransport(bogusLengthPrefix + ByteArray(10))

        assertThrows(BadResponse::class.java) {
            RusefiBtConnection(transport).readChannels()
        }
    }
}
