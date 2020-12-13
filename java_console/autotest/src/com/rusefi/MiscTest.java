package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Ignore;
import org.junit.Test;

public class MiscTest {
    @Test
    public void burn() {
        EcuTestHelper ecu = EcuTestHelper.createInstance();

        BinaryProtocol bp = ecu.getLinkManager().getCurrentStreamState();
        // let's make sure 'burn' command works since sometimes it does not
        bp.burn();
    }
}
