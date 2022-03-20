package com.rusefi.native_;

import com.rusefi.core.Sensor;
import com.rusefi.enums.SensorType;
import org.junit.Test;

import java.nio.ByteBuffer;

import static com.rusefi.config.generated.Fields.TS_FILE_VERSION;
import static com.rusefi.shared.FileUtil.littleEndianWrap;
import static junit.framework.Assert.*;

public class JniUnitTest {
    @Test
    public void run() {
        JniSandbox.loadLibrary();

        EngineLogic engineLogic = new EngineLogic();
        String version = engineLogic.getVersion();
        assertTrue("Got " + version, version.contains("Hello"));

        engineLogic.invokePeriodicCallback();

        assertEquals(TS_FILE_VERSION, (int) getValue(engineLogic.getOutputs(), Sensor.FIRMWARE_VERSION));

        assertEquals(14.0, getValue(engineLogic.getOutputs(), Sensor.TARGET_AFR));
        double veValue = getValue(engineLogic.getOutputs(), Sensor.veValue);
        assertTrue("veValue", veValue > 40 && veValue < 90);

        assertEquals(18.11, getValue(engineLogic.getOutputs(), Sensor.runningFuel));

        engineLogic.setSensor(SensorType.Rpm.name(), 4000);
        engineLogic.invokePeriodicCallback();
        assertEquals(4000.0, getValue(engineLogic.getOutputs(), Sensor.RPM));

        assertEquals(18.11, getValue(engineLogic.getOutputs(), Sensor.runningFuel));
    }

    private double getValue(byte[] outputs, Sensor sensor) {
        ByteBuffer bb = littleEndianWrap(outputs, sensor.getOffset(), 4);
        return sensor.getValueForChannel(bb) * sensor.getScale();
    }
}
