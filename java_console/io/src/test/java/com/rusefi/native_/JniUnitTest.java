package com.rusefi.native_;

import com.rusefi.core.Sensor;
import org.junit.Test;

import java.nio.ByteBuffer;

import static com.rusefi.config.generated.Fields.TS_FILE_VERSION;
import static com.rusefi.shared.FileUtil.littleEndianWrap;
import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertTrue;

public class JniUnitTest {
    @Test
    public void run() {
        JniSandbox.loadLibrary();

        EngineLogic engineLogic = new EngineLogic();
        String version = engineLogic.getVersion();
        assertTrue("Got " + version, version.contains("Hello"));

        engineLogic.invokePeriodicCallback();

        byte[] outputs = engineLogic.getOutputs();

        Sensor sensor = Sensor.FIRMWARE_VERSION;
        ByteBuffer bb = littleEndianWrap(outputs, sensor.getOffset(), 4);
        double value = sensor.getValueForChannel(bb);
        assertEquals(TS_FILE_VERSION, (int)value);
    }
}
