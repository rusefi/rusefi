package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * [tag:java_source_encoding] Java sources are UTF-8 and the build must compile them as such
 * (options.encoding in the root build.gradle). With the platform default, Windows builds turned
 * the ellipsis in "Loading console..." and "Scanning for ECU..." into three garbled characters.
 * This file deliberately keeps a literal non-ASCII character to catch that.
 */
class SourceEncodingTest {
    @Test
    void nonAsciiLiteralsCompileAsUtf8() {
        String literal = "…";
        assertEquals(1, literal.length(), "One character, not its UTF-8 bytes read as windows-1252");
        assertEquals(0x2026, literal.charAt(0));
    }
}
