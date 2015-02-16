package com.rusefi.core.test;

import com.rusefi.core.ResponseBuffer;
import org.junit.Test;

import java.util.concurrent.atomic.AtomicReference;

import static org.junit.Assert.*;

/**
 * @author Andrey Belomutskiy
 *         12/26/12
 * @see ResponseBuffer
 */
public class ResponseBufferTest {
    @Test
    public void testSingleLine() {
        final AtomicReference<String> currentReference = new AtomicReference<>();

        ResponseBuffer rb = new ResponseBuffer(new ResponseBuffer.ResponseListener() {
            public void onResponse(String response) {
                currentReference.set(response);
            }
        });
        rb.append("\r");
        assertNull(currentReference.get());
        rb.append("\n");
        assertNull(currentReference.get());

        rb.append("hi\n");
        assertEquals("hi", currentReference.get());

        rb.append("\r\n\r\n\r\nhi2\n\n\n");
        assertEquals("hi2", currentReference.get());
    }
}
