package com.rusefi.io.commands;

import org.junit.jupiter.api.Test;

import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class HelloCommandTest {
    @Test
    public void stripsProtocolStringTerminator() {
        String signature = "rusEFI fix/9978.2026.08.03.uaefi.1242381171";
        byte[] text = signature.getBytes(StandardCharsets.US_ASCII);
        byte[] response = new byte[text.length + 2];
        System.arraycopy(text, 0, response, 1, text.length);

        assertEquals(signature, HelloCommand.decodeStringResponse(response));
    }
}
