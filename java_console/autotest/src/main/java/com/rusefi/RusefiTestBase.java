package com.rusefi;

import com.opensr5.ini.IniFileReader;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.functional_tests.EcuTestHelper;
import com.rusefi.waves.EngineChart;
import org.junit.*;

import java.io.FileNotFoundException;

public class RusefiTestBase {
    protected EcuTestHelper ecu;

    protected boolean needsHardwareTriggerInput() {
        // Most tests do not, but some may need it
        return false;
    }

    @Before
    public void startUp() {
        BinaryProtocol.iniFileProvider = signature -> {
            try {
                return IniFileReader.readIniFile(LocalIniFileProvider.INI_FILE_FOR_SIMULATOR_ROOT_PATH);
            } catch (FileNotFoundException e) {
                throw new RuntimeException(e);
            }
        };
        try {
            ecu = EcuTestHelper.createInstance(needsHardwareTriggerInput());
        } catch (Throwable e) {
            throw new IllegalStateException("During start-up", e);
        }
    }

    @After
    public void checkStackUsage() {
        if (ecu != null)
            ecu.sendCommand("threadsinfo");
    }

    protected EngineChart nextChart() {
        return TestingUtils.nextChart(ecu.commandQueue);
    }
}
