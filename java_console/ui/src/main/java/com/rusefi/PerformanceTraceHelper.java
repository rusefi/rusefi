package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.tracing.Entry;
import com.rusefi.tracing.JsonOutput;
import com.rusefi.ui.RpmModel;

import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;
import static com.rusefi.tools.ConsoleTools.startAndConnect;

public class PerformanceTraceHelper {
    public static void grabPerformanceTrace(BinaryProtocol bp) {
        bp.executeCommand(new byte[]{Fields.TS_PERF_TRACE_BEGIN}, "begin trace");

        try {
            Thread.sleep(500);

            byte[] packet = bp.executeCommand(new byte[]{Fields.TS_PERF_TRACE_GET_BUFFER}, "get trace", true);
            if (!checkResponseCode(packet, (byte) Fields.TS_RESPONSE_OK) || ((packet.length - 1) % 8) != 0)
                throw new IllegalStateException("Unexpected packet");

            List<Entry> data = Entry.parseBuffer(packet);

            int rpm = RpmModel.getInstance().getValue();
            String fileName = Logger.getDate() + "_rpm_" + rpm + "_rusEFI_trace" + ".json";


            JsonOutput.writeToStream(data, new FileOutputStream(fileName));
        } catch (IOException | InterruptedException e1) {
            throw new IllegalStateException(e1);
        }
    }

    public static void getPerformanceTune() {
        startAndConnect(linkManager -> {
            BinaryProtocol binaryProtocol = linkManager.getConnector().getBinaryProtocol();
            grabPerformanceTrace(binaryProtocol);
            System.exit(0);
            return null;
        });
    }
}
