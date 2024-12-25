package com.rusefi.nucleo;

import com.devexperts.logging.Logging;
import com.rusefi.RusefiTestBase;
import com.rusefi.f4discovery.DiscoveryPwmHardwareTest;
import com.rusefi.common.VssHardwareTestLogic;
import org.junit.Test;

import static com.devexperts.logging.Logging.getLogging;

public class NucleoVssHardwareTest extends RusefiTestBase {
    private static final Logging log = getLogging(DiscoveryPwmHardwareTest.class);

    @Override
    protected boolean needsHardwareTriggerInput() {
        // This test uses hardware trigger input!
        return true;
    }

    @Test
    public void testIdlePin() {
        VssHardwareTestLogic.runIdleVssTest(ecu, "PD2", "PA6");
    }
}
