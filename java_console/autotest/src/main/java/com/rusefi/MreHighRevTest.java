package com.rusefi;

import com.rusefi.enums.engine_type_e;
import com.rusefi.f4discovery.HighRevTest;
import org.junit.Test;

public class MreHighRevTest extends RusefiTestBase {
    @Test
    public void runMreTest() {
        HighRevTest.runHighRevTest(engine_type_e.MRE_SUBARU_EJ18, ecu);
    }
}
