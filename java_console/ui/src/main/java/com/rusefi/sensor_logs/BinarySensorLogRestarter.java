package com.rusefi.sensor_logs;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.NamedThreadFactory;
import com.rusefi.Timeouts;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.maintenance.VersionChecker;
import com.rusefi.tools.online.Online;
import com.rusefi.tools.online.UploadResult;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.UIContext;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

public class BinarySensorLogRestarter implements SensorLog {
    private static final Logging log = getLogging(BinarySensorLogRestarter.class);
    private final static Executor UPLOAD_EXECUTOR = Executors.newSingleThreadExecutor(new NamedThreadFactory("BinarySensorLogRestarter"));
    private final UIContext uiContext;

    private BinarySensorLog logger;

    private long seenRunning;

    public BinarySensorLogRestarter(UIContext uiContext) {
        this.uiContext = uiContext;
    }

    @Override
    public double getSecondsSinceFileStart() {
        throw new UnsupportedOperationException();
    }

    @Override
    public synchronized void writeSensorLogLine() {
        double rpm = SensorCentral.getInstance().getValue(Sensor.RPMValue);
        if (rpm > 200) {
            seenRunning = System.currentTimeMillis();
        }
        if (rpm == 0 && seenRunning > 0 && (System.currentTimeMillis() - seenRunning) > 5 * Timeouts.SECOND) {
            // restart logging 5 seconds after last positive RPM
            close();
        }

        if (logger == null) {
            byte[] response = SensorCentral.getInstance().getResponse();
            logger = new BinarySensorLog<>(sensor -> {
                if (sensor instanceof Sensor) {
                    return SensorCentral.getInstance().getValue((Sensor) sensor);
                } else if (sensor instanceof CustomBinaryLogEntry && response != null) {
                    return ((CustomBinaryLogEntry) sensor).getValue(response);
                }
                return 0.0;
            }, SensorLogger.getSensors(uiContext));
        }
        logger.writeSensorLogLine();
    }

    @Override
    public synchronized void close() {
        if (logger != null) {
            logger.close();
            String fileName = logger.getFileName();
            scheduleUpload(fileName);
        }
        logger = null;
        seenRunning = 0;
    }

    private void scheduleUpload(String fileName) {
        log.error("Will upload " + fileName);
        UPLOAD_EXECUTOR.execute(new Runnable() {
            @Override
            public void run() {
                UploadResult result = Online.upload(new File(fileName), AuthTokenPanel.getAuthToken());
                System.out.println(result.toString());
            }
        });
    }
}
