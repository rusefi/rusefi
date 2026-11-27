package com.rusefi.veautotune.logger

import com.rusefi.veautotune.protocol.Channels
import java.io.StringWriter
import org.junit.Assert.assertEquals
import org.junit.Test

class CsvLoggerTest {

    @Test
    fun `writeHeader then appendRow produces expected csv lines`() {
        val sw = StringWriter()
        val logger = CsvLogger(sw)
        logger.writeHeader()
        logger.appendRow(0.5, Channels(rpm = 1500.0, map = 45.0, tps = 10.0, clt = 80.0, afr = 13.2))
        assertEquals("Time,RPM,MAP,TPS,CLT,Air/Fuel Ratio\n0.5,1500.0,45.0,10.0,80.0,13.2\n", sw.toString())
    }
}
