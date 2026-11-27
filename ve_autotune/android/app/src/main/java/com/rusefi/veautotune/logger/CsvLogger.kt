package com.rusefi.veautotune.logger

import com.rusefi.veautotune.protocol.Channels
import java.io.Writer

/**
 * Writes the recorded channel stream to CSV, one row per sample.
 *
 * Flushes after every write (header and each row) so that if the app
 * crashes or the Bluetooth link drops mid-session, whatever rows were
 * already captured are safely on disk rather than sitting in a buffer.
 */
class CsvLogger(private val out: Writer) {

    fun writeHeader() {
        out.write("t,rpm,map,tps,clt,afr\n")
        out.flush()
    }

    fun appendRow(tSeconds: Double, c: Channels) {
        out.write("$tSeconds,${c.rpm},${c.map},${c.tps},${c.clt},${c.afr}\n")
        out.flush()
    }
}
