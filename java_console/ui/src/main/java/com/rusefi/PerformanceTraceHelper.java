package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.MessagesCentral;
import com.rusefi.tracing.Entry;
import com.rusefi.tracing.JsonOutput;
import com.rusefi.ui.RpmModel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;
import static com.rusefi.tools.ConsoleTools.startAndConnect;

public class PerformanceTraceHelper {
    public static void grabPerformanceTrace(JComponent parent, BinaryProtocol bp) {
        if (bp == null) {
            String msg = "Failed to locate serial ports";
            JOptionPane.showMessageDialog(parent, msg, msg, JOptionPane.ERROR_MESSAGE);
            return;
        }

        try {
            List<Entry> data = requestWaitAndGetPTrace(bp);
            if (data.isEmpty()) {
                String msg = "Empty PERF_TRACE response";
                JOptionPane.showMessageDialog(parent, msg, msg, JOptionPane.ERROR_MESSAGE);
                return;
            }
            MessagesCentral.getInstance().postMessage(PerformanceTraceHelper.class, "Got " + data.size() + " PTrace entries");
            int rpm = RpmModel.getInstance().getValue();
            String fileName = FileLog.getDate() + "_rpm_" + rpm + "_rusEFI_trace" + ".json";

            JsonOutput.writeToStream(data, new FileOutputStream(fileName));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    @NotNull
    private static List<Entry> requestWaitAndGetPTrace(BinaryProtocol bp) {
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

    public static void getPerformanceTune() {
        startAndConnect(linkManager -> {
            BinaryProtocol binaryProtocol = linkManager.getConnector().getBinaryProtocol();
            grabPerformanceTrace(null, binaryProtocol);
            System.exit(0);
            return null;
        });
    }
}
