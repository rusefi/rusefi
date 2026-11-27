package com.rusefi.veautotune.protocol

/**
 * Request/response framing for the rusEFI binary protocol's output-channel
 * read command ('O'), per the SDD plan's Global Constraints wire format:
 *
 * Outer envelope (java_console/io/src/main/java/com/rusefi/binaryprotocol/
 * IoHelper.java#makeCrc32Packet):
 *   [2 bytes BIG-ENDIAN length of inner][inner bytes][4 bytes BIG-ENDIAN CRC32 of inner bytes]
 *
 * Inner request command (java_console/io/src/main/java/com/rusefi/io/commands/
 * ByteRange.java#packOffsetAndSize -- offset/count are packed as swap16(value)
 * through a big-endian putShort, which is equivalent to writing them
 * little-endian on the wire):
 *   ['O' (0x4F)] + offset as LITTLE-ENDIAN 16-bit + count as LITTLE-ENDIAN 16-bit
 * This is the ONE place the inner payload's endianness differs from the
 * outer envelope's -- do not "fix" it to match big-endian.
 *
 * Inner response (java_console/io/src/main/java/com/rusefi/io/tcp/
 * BinaryProtocolServer.java#getOutputCommandResponse):
 *   [1 byte: 0x00 = OK][count raw bytes]
 * A first byte other than 0x00 means the ECU rejected the request.
 *
 * This file must never grow a generic "build any command" helper -- the only
 * frame this app is allowed to build is the output-channel read request.
 */

private const val OUTPUT_COMMAND: Byte = 0x4F // 'O'
private const val STATUS_OK: Byte = 0x00

class BadResponse(message: String) : Exception(message)

/**
 * Builds the full outer-enveloped request for an output-channel read of
 * [count] bytes starting at [offset]. The only command this app ever sends.
 */
fun buildOutputChannelRequest(offset: Int, count: Int): ByteArray {
    val inner = ByteArray(5)
    inner[0] = OUTPUT_COMMAND
    inner[1] = (offset and 0xFF).toByte()
    inner[2] = ((offset shr 8) and 0xFF).toByte()
    inner[3] = (count and 0xFF).toByte()
    inner[4] = ((count shr 8) and 0xFF).toByte()
    return wrapOuterEnvelope(inner)
}

/** [2 bytes BE length of inner][inner][4 bytes BE CRC32 of inner]. */
private fun wrapOuterEnvelope(inner: ByteArray): ByteArray {
    val outer = ByteArray(2 + inner.size + 4)
    outer[0] = ((inner.size shr 8) and 0xFF).toByte()
    outer[1] = (inner.size and 0xFF).toByte()
    System.arraycopy(inner, 0, outer, 2, inner.size)

    val crc = crc32(inner)
    val crcOffset = outer.size - 4
    outer[crcOffset] = ((crc shr 24) and 0xFF).toByte()
    outer[crcOffset + 1] = ((crc shr 16) and 0xFF).toByte()
    outer[crcOffset + 2] = ((crc shr 8) and 0xFF).toByte()
    outer[crcOffset + 3] = (crc and 0xFF).toByte()
    return outer
}

/**
 * Parses a full outer envelope (as read off the socket per the outer BE16
 * length prefix -- see Task 4) into the decoded output-channel payload.
 *
 * Verifies the trailing CRC32 against the inner bytes and that the first
 * inner byte is the OK status; throws [BadResponse] otherwise.
 */
fun parseResponse(frame: ByteArray): ByteArray {
    val innerLength = ((frame[0].toInt() and 0xFF) shl 8) or (frame[1].toInt() and 0xFF)
    val inner = frame.copyOfRange(2, 2 + innerLength)
    val crcOffset = 2 + innerLength

    val expectedCrc = readBigEndianUInt32(frame, crcOffset)
    val actualCrc = crc32(inner)
    if (actualCrc != expectedCrc) {
        throw BadResponse(
            "CRC mismatch: expected 0x%08x, got 0x%08x".format(expectedCrc, actualCrc)
        )
    }

    val status = inner[0]
    if (status != STATUS_OK) {
        throw BadResponse("ECU rejected request: 0x%02x".format(status))
    }

    return inner.copyOfRange(1, inner.size)
}

private fun readBigEndianUInt32(bytes: ByteArray, offset: Int): Long {
    return ((bytes[offset].toLong() and 0xFF) shl 24) or
        ((bytes[offset + 1].toLong() and 0xFF) shl 16) or
        ((bytes[offset + 2].toLong() and 0xFF) shl 8) or
        (bytes[offset + 3].toLong() and 0xFF)
}
