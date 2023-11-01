package com.rusefi;

import com.rusefi.enums.engine_type_e;
import com.rusefi.f4discovery.HighRevTest;
import org.junit.Test;

public class PassatHighRevTest extends RusefiTestBase {
    @Test
    public void runPassatTest() {
        HighRevTest.runHighRevTest(engine_type_e.MRE_VW_B6, ecu);
    }
}
