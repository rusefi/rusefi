package com.rusefi.binaryprotocol;

import com.devexperts.logging.FileLogger;
import com.rusefi.Timeouts;
import com.rusefi.composite.CompositeEvent;
import com.rusefi.composite.CompositeParser;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import com.rusefi.stream.LogicdataStreamFile;
import com.rusefi.stream.StreamFile;
import com.rusefi.stream.TSHighSpeedLog;
import com.rusefi.stream.VcdStreamFile;
import org.jetbrains.annotations.NotNull;

import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class BinaryProtocolLogger {
    private static final int HIGH_RPM_DELAY = Integer.getInteger("high_speed_logger_time", 10);
    public static final int COMPOSITE_OFF_RPM = Integer.getInteger("high_speed_logger_rpm", 700);

    /**
     * Composite logging turns off after 10 seconds of RPM above 300
     */
    public boolean needCompositeLogger;
    private boolean isCompositeLoggerEnabled;
    private long lastLowRpmTime = System.currentTimeMillis();

    private final List<StreamFile> compositeLogs = new CopyOnWriteArrayList();

    private final SensorCentral.SensorListener rpmListener;
    private final Thread hook = new Thread(() -> closeComposites(), "BinaryProtocol::hook");

    public BinaryProtocolLogger(LinkManager linkManager) {
        rpmListener = currentRpm -> {
            /**
             * we only request and log composite logger at relatively low RPM
             */
            if (currentRpm <= COMPOSITE_OFF_RPM) {
                needCompositeLogger = linkManager.getCompositeLogicEnabled();
                lastLowRpmTime = System.currentTimeMillis();
            } else if (System.currentTimeMillis() - lastLowRpmTime > HIGH_RPM_DELAY * Timeouts.SECOND) {
                needCompositeLogger = false;
            }
        };

        // fragile or just scary: here we install a JVM level callback for gradual file footer
        Runtime.getRuntime().addShutdownHook(hook);
        needCompositeLogger = linkManager.getCompositeLogicEnabled();
    }

    private void createCompositesIfNeeded() {
        if (!compositeLogs.isEmpty())
            return;
        compositeLogs.addAll(Arrays.asList(
                new VcdStreamFile(getFileName("rusEFI_trigger_log_", ".vcd")),
                new LogicdataStreamFile(getFileName("rusEFI_trigger_log_", ".logicdata")),
                new TSHighSpeedLog(getFileName("rusEFI_trigger_log_"))
        ));
    }

    @NotNull
    public static String getFileName(String prefix) {
        return getFileName(prefix, ".csv");
    }

    @NotNull
    public static String getFileName(String prefix, String fileType) {
        return FileLogger.DIR + prefix + FileLogger.getDate() + fileType;
    }

    public void compositeLogic(BinaryProtocol binaryProtocol) {
        if (needCompositeLogger) {
            getComposite(binaryProtocol);
        } else if (isCompositeLoggerEnabled) {
            binaryProtocol.executeCommand(Integration.TS_SET_LOGGER_SWITCH, new byte[]{Integration.TS_COMPOSITE_DISABLE}, "disable composite");
            isCompositeLoggerEnabled = false;
            closeComposites();
        }
    }

    private void closeComposites() {
        for (StreamFile composite : compositeLogs) {
            composite.close();
        }
        compositeLogs.clear();
    }

    public void getComposite(BinaryProtocol binaryProtocol) {
        if (binaryProtocol.isClosed())
            return;

        // get command would enable composite logging in controller but we need to turn it off from our end
        // todo: actually if console gets disconnected composite logging might end up enabled in controller?
        isCompositeLoggerEnabled = true;

        byte[] response = binaryProtocol.executeCommand(Integration.TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY, "composite log");
        if (checkResponseCode(response)) {
            List<CompositeEvent> events = CompositeParser.parse(response);
            createCompositesIfNeeded();
            for (StreamFile composite : compositeLogs)
                composite.append(events);
        }
    }

    public void start() {
        SensorCentral.getInstance().addListener(Sensor.RPMValue, rpmListener);
    }

    public void close() {
        SensorCentral.getInstance().removeListener(Sensor.RPMValue, rpmListener);
        closeComposites();
        Runtime.getRuntime().removeShutdownHook(hook);
    }
}
