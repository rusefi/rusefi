package com.rusefi.maintenance;

import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.IniField;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.panama.PanamaHelper;

import java.util.Optional;
import java.util.concurrent.atomic.AtomicInteger;

public class OutputChannelsHelper {
    private static final String MCUSERIAL = PanamaHelper.MCUSERIAL;

    public static Optional<Integer> readMcuSerial(
        final PortResult ecuPort,
        final UpdateOperationCallbacks callbacks,
        ConnectivityContext connectivityContext) {
        return BinaryProtocolExecutor.executeWithSuspendedPortScanner(
            ecuPort.port,
            callbacks,
            (binaryProtocol) -> readMcuSerial(binaryProtocol, callbacks),
            Optional.empty(), connectivityContext,
            "readMcuSerial");
    }

    private static Optional<Integer> readMcuSerial(
        final BinaryProtocol binaryProtocol,
        final UpdateOperationCallbacks callbacks
    ) {
        callbacks.logLine("Reading `" + MCUSERIAL + "` output channel...");
        IniField mcuSerialField;
        try {
            mcuSerialField = PanamaHelper.getIniField(binaryProtocol);
        } catch (IniMemberNotFound e) {
            callbacks.logLine("Please update firmware to use this feature...");
            return Optional.empty();
        }
        final AtomicInteger mcuSerial = new AtomicInteger();
        SensorCentral.getInstance().addListener(() -> {
            mcuSerial.set(PanamaHelper.getMcuSerial(mcuSerialField));
        });
        if (binaryProtocol.requestOutputChannels()) {
            callbacks.logLine(String.format(PanamaHelper.MCUSERIAL + " is %d", mcuSerial.get()));
            return Optional.of(mcuSerial.get());
        } else {
            callbacks.logLine("Failed to request output channels...");
            return Optional.empty();
        }
    }

}
