package com.rusefi.veautotune.bt

import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import java.io.EOFException

/**
 * Standard Serial Port Profile UUID, used with
 * [BluetoothDevice.createRfcommSocketToServiceRecord] to open an RFCOMM
 * channel to a classic-Bluetooth SPP device (e.g. a rusEFI BT module).
 * This is the well-known SPP UUID, not something specific to rusEFI --
 * see Android's BluetoothDevice.createRfcommSocketToServiceRecord docs
 * and the Bluetooth SIG's Serial Port Profile spec.
 */
object RusefiSpp {
    val UUID: java.util.UUID = java.util.UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
}

/**
 * [ByteTransport] backed by a real, already-connected [BluetoothSocket].
 *
 * This is a dumb byte pipe: it knows nothing about the rusEFI wire
 * protocol (framing, CRC, command bytes). All protocol semantics live in
 * [com.rusefi.veautotune.protocol] and [RusefiBtConnection]; this class
 * only moves bytes in and out of the socket's streams, which keeps the
 * Global Constraint intact (no code path here can construct or send any
 * ECU command).
 *
 * Needs a real `BluetoothDevice`/Android runtime, so it is exercised
 * manually on-device per Task 6, not in a JVM unit test.
 */
class SocketTransport(private val socket: BluetoothSocket) : ByteTransport {

    override fun write(bytes: ByteArray) {
        socket.outputStream.write(bytes)
        socket.outputStream.flush()
    }

    override fun readFully(n: Int): ByteArray {
        val buffer = ByteArray(n)
        var offset = 0
        while (offset < n) {
            val remaining = n - offset
            val read = socket.inputStream.read(buffer, offset, remaining)
            if (read == -1) {
                throw EOFException(
                    "Bluetooth input stream closed after $offset of $n requested bytes"
                )
            }
            offset += read
        }
        return buffer
    }
}
