package com.rusefi.io.commands;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.tracing.Entry;
import org.jetbrains.annotations.NotNull;

import java.util.List;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class PTraceHelper {
    @NotNull
    public static List<Entry> requestWaitAndGetPTrace(BinaryProtocol bp) {
        bp.executeCommand(Fields.TS_PERF_TRACE_BEGIN, "begin trace");
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }

        return executeGetPTraceCommand(bp);
    }

    @NotNull
    private static List<Entry> executeGetPTraceCommand(BinaryProtocol bp) {
        byte[] packet = bp.executeCommand(Fields.TS_PERF_TRACE_GET_BUFFER, "get trace");
        if (!checkResponseCode(packet, (byte) Fields.TS_RESPONSE_OK) || ((packet.length - 1) % 8) != 0)
            throw new IllegalStateException("Unexpected packet, length=" + (packet != null ? 0 : packet.length));

        return Entry.parseBuffer(packet);
    }
}
