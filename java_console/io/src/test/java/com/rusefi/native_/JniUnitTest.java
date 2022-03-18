package com.rusefi.native_;

import com.rusefi.core.Sensor;
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

        byte[] outputs = engineLogic.getOutputs();

        assertEquals(TS_FILE_VERSION, (int) getValue(outputs, Sensor.FIRMWARE_VERSION));

        assertEquals(14.0, getValue(outputs, Sensor.TARGET_AFR));
//        assertEquals(1, getValue(outputs, Sensor.veValue));
    }

    private double getValue(byte[] outputs, Sensor sensor) {
        ByteBuffer bb = littleEndianWrap(outputs, sensor.getOffset(), 4);
        return sensor.getValueForChannel(bb) * sensor.getScale();
    }
}
