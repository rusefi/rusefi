package com.rusefi.sensor_logs;

import com.rusefi.FileLog;
import com.rusefi.NamedThreadFactory;
import com.rusefi.Timeouts;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.stream.Collectors;

public class BinarySensorLogRestarter implements SensorLog {
    private final static Executor UPLOAD_EXECUTOR = Executors.newSingleThreadExecutor(new NamedThreadFactory("BinarySensorLogRestarter"));

    private BinarySensorLog logger;

    private long seenRunning;

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
            Collection<Sensor> sensorsForLogging = filterOutSensorsWithoutType(SensorLogger.SENSORS);

            logger = new BinarySensorLog<>(sensor -> SensorCentral.getInstance().getValue(sensor), sensorsForLogging);
        }
        logger.writeSensorLogLine();
    }

    private static Collection<Sensor> filterOutSensorsWithoutType(Sensor[] sensors) {
        return Arrays.stream(sensors).filter(sensor -> sensor.getType() != null).collect(Collectors.toCollection(ArrayList::new));
    }

    @Override
    public synchronized void close() {
        if (logger != null) {
            logger.close();
        }
        logger = null;
        seenRunning = 0;
    }
}
