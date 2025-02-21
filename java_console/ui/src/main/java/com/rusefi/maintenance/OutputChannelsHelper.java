package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.IniField;
import com.rusefi.SerialPortScanner;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.panama.PanamaHelper;

import java.util.Optional;
import java.util.concurrent.atomic.AtomicInteger;

import static com.devexperts.logging.Logging.getLogging;

public class OutputChannelsHelper {
    private static final Logging log = getLogging(OutputChannelsHelper.class);

    public static Optional<Integer> readMcuSerial(
        final SerialPortScanner.PortResult ecuPort,
        final UpdateOperationCallbacks callbacks
    ) {
        return BinaryProtocolExecutor.executeWithSuspendedPortScanner(
            ecuPort.port,
            callbacks,
            (binaryProtocol) -> {
                try {
                    return readMcuSerial(binaryProtocol, callbacks);
                } catch (final Exception e) {
                    log.error("Reading `MCUSERIAL` output channel failed:", e);
                    callbacks.logLine("Reading `MCUSERIAL` output channel failed");
                    return Optional.empty();
                }
            },
            Optional.empty(),
            false
        );
    }

    private static Optional<Integer> readMcuSerial(
        final BinaryProtocol binaryProtocol,
        final UpdateOperationCallbacks callbacks
    ) {
        callbacks.logLine("Reading `MCUSERIAL` output channel...");
        Optional<Integer> result = Optional.empty();
        final IniField mcuSerialField = PanamaHelper.getIniField(binaryProtocol);
        if (mcuSerialField != null) {
            final AtomicInteger mcuSerial = new AtomicInteger();
            SensorCentral.getInstance().addListener(() -> {
                mcuSerial.set(PanamaHelper.getMcuSerial(mcuSerialField));
            });
            if (binaryProtocol.requestOutputChannels()) {
                callbacks.logLine(String.format("`MCUSERIAL` is %d", mcuSerial.get()));
                result = Optional.of(mcuSerial.get());
            } else {
                callbacks.logLine("Failed to request output channels...");
            }
        } else {
            callbacks.logLine("Please update firmware to use this feature...");
        }
        return result;
    }

}
