package com.rusefi.veautotune.bt

import com.rusefi.veautotune.protocol.Channels
import com.rusefi.veautotune.protocol.buildOutputChannelRequest
import com.rusefi.veautotune.protocol.decodeChannels
import com.rusefi.veautotune.protocol.parseResponse

/** Number of output-channel bytes requested, per the SDD plan's Global Constraints. */
private const val OUTPUT_CHANNELS_OFFSET = 0
private const val OUTPUT_CHANNELS_COUNT = 214

/**
 * Thin seam over the raw byte stream so this class is testable without a
 * real `BluetoothSocket`. Task 5 wires a real `BluetoothSocket`'s
 * input/output streams behind this interface; [readFully] there must loop
 * on `InputStream.read` until [n] bytes are collected or the stream closes.
 */
interface ByteTransport {
    fun write(bytes: ByteArray)
    fun readFully(n: Int): ByteArray
}

/**
 * Reads the ECU's output channels over [transport].
 *
 * This is the only thing this class does: it sends exactly one
 * output-channel read request (the request built by
 * [buildOutputChannelRequest], the only command this app is allowed to
 * build) and decodes exactly one response. No other command can be
 * constructed or sent through this class.
 */
class RusefiBtConnection(private val transport: ByteTransport) {

    fun readChannels(): Channels {
        transport.write(buildOutputChannelRequest(OUTPUT_CHANNELS_OFFSET, OUTPUT_CHANNELS_COUNT))

        val lengthPrefix = transport.readFully(2)
        val innerLength = ((lengthPrefix[0].toInt() and 0xFF) shl 8) or (lengthPrefix[1].toInt() and 0xFF)
        val inner = transport.readFully(innerLength)
        val crc = transport.readFully(4)

        val frame = ByteArray(lengthPrefix.size + inner.size + crc.size)
        System.arraycopy(lengthPrefix, 0, frame, 0, lengthPrefix.size)
        System.arraycopy(inner, 0, frame, lengthPrefix.size, inner.size)
        System.arraycopy(crc, 0, frame, lengthPrefix.size + inner.size, crc.size)

        val payload = parseResponse(frame)
        return decodeChannels(payload)
    }
}
