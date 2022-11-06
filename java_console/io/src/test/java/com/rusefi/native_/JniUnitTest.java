package com.rusefi.native_;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.enums.SensorType;
import org.junit.Test;

import java.nio.ByteBuffer;

import static com.rusefi.config.generated.Fields.TS_FILE_VERSION;
import static com.rusefi.config.generated.Fields.engine_type_e_MRE_MIATA_NB2_MAP;
import static com.rusefi.core.FileUtil.littleEndianWrap;
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

        double veValue = getValue(engineLogic.getOutputs(), Sensor.veValue);
        assertTrue("veValue", veValue > 40 && veValue < 90);

        assertEquals(18.11, getValue(engineLogic.getOutputs(), Sensor.runningFuel));

        engineLogic.setSensor(SensorType.Rpm.name(), 4000);
        engineLogic.invokePeriodicCallback();
        assertEquals(4000.0, getValue(engineLogic.getOutputs(), Sensor.RPMValue));

        assertEquals(18.11, getValue(engineLogic.getOutputs(), Sensor.runningFuel));

        engineLogic.setEngineType(engine_type_e_MRE_MIATA_NB2_MAP);
        assertEquals(2.45, getField(engineLogic, Fields.GEARRATIO1));
    }

    private double getField(EngineLogic engineLogic, Field gearratio1) {
        byte[] configuration = engineLogic.getConfiguration();
        assertNotNull("configuration", configuration);
        return gearratio1.getValue(new ConfigurationImage(configuration), gearratio1.getScale());
    }

    private double getValue(byte[] outputs, Sensor sensor) {
        ByteBuffer bb = littleEndianWrap(outputs, sensor.getOffset(), 4);
        return sensor.getValueForChannel(bb) * sensor.getScale();
    }
}
