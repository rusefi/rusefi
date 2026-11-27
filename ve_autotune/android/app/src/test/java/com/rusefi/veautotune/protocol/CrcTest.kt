package com.rusefi.veautotune.protocol

import org.junit.Assert.assertEquals
import org.junit.Test

// Same vectors as java_console's own CRCTest.java
// (java_console/io/src/test/java/com/rusefi/binaryprotocol/test/CRCTest.java),
// ported verbatim so correctness is checked against an existing trusted
// vector, not a fresh guess.
class CrcTest {
    @Test
    fun `crc32 matches known vector for single byte A`() {
        val a = byteArrayOf('A'.code.toByte())
        assertEquals(0xD3D99E8BL, crc32(a))
    }

    @Test
    fun `crc32 matches known vector for single byte S`() {
        val a = byteArrayOf('S'.code.toByte())
        assertEquals(0x2060EFC3L, crc32(a))
    }
}
