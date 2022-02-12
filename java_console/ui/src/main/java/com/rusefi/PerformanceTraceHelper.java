package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.tracing.Entry;
import com.rusefi.tracing.JsonOutput;
import com.rusefi.ui.RpmModel;

import javax.swing.*;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;
import static com.rusefi.tools.ConsoleTools.startAndConnect;

public class PerformanceTraceHelper {
    public static void grabPerformanceTrace(JComponent parent, BinaryProtocol bp) {
        if (bp == null) {
            JOptionPane.showMessageDialog(parent, "Failed to locate serial ports");
            return;
        }
        bp.executeCommand(Fields.TS_PERF_TRACE_BEGIN, "begin trace");

        try {
            Thread.sleep(500);

            byte[] packet = bp.executeCommand(Fields.TS_PERF_TRACE_GET_BUFFER, "get trace");
            if (!checkResponseCode(packet, (byte) Fields.TS_RESPONSE_OK) || ((packet.length - 1) % 8) != 0)
                throw new IllegalStateException("Unexpected packet, length=" + (packet != null ? 0 : packet.length));

            List<Entry> data = Entry.parseBuffer(packet);

            int rpm = RpmModel.getInstance().getValue();
            String fileName = FileLog.getDate() + "_rpm_" + rpm + "_rusEFI_trace" + ".json";


            JsonOutput.writeToStream(data, new FileOutputStream(fileName));
        } catch (IOException | InterruptedException e1) {
            throw new IllegalStateException(e1);
        }
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
