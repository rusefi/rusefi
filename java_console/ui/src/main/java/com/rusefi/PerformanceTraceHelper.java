package com.rusefi;

import com.devexperts.logging.FileLogger;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.commands.PTraceHelper;
import com.rusefi.tracing.Entry;
import com.rusefi.tracing.JsonOutput;
import com.rusefi.ui.RpmModel;

import javax.swing.*;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

import static com.rusefi.tools.ConsoleTools.startAndConnect;

public class PerformanceTraceHelper {
    public static void grabPerformanceTrace(JComponent parent, BinaryProtocol bp) {
        if (bp == null) {
            String msg = "Failed to locate serial ports";
            JOptionPane.showMessageDialog(parent, msg, msg, JOptionPane.ERROR_MESSAGE);
            return;
        }

        try {
            List<Entry> data = PTraceHelper.requestWaitAndGetPTrace(bp);
            if (data.isEmpty()) {
                String msg = "Empty PERF_TRACE response";
                JOptionPane.showMessageDialog(parent, msg, msg, JOptionPane.ERROR_MESSAGE);
                return;
            }
            MessagesCentral.getInstance().postMessage(PerformanceTraceHelper.class, "Got " + data.size() + " PTrace entries");
            int rpm = RpmModel.getInstance().getValue();
            String fileName = FileLogger.getDate() + "_rpm_" + rpm + "_rusEFI_trace" + ".json";

            File outputFile = new File(fileName);
            JsonOutput.writeToStream(data, new FileOutputStream(outputFile));
            MessagesCentral.getInstance().postMessage(PerformanceTraceHelper.class, "Saved to " + outputFile.getAbsolutePath());
            MessagesCentral.getInstance().postMessage(PerformanceTraceHelper.class, "See https://wiki.rusefi.com/Developer-Performance-Tracing");
        } catch (IOException e) {
            throw new IllegalStateException(e);
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
