package com.rusefi.native_;

import org.junit.Ignore;
import org.junit.Test;

import static junit.framework.Assert.assertTrue;

public class JniUnitTest {
    @Test
    @Ignore
    public void run() {
        JniSandbox.loadLibrary();

        EngineLogic engineLogic = new EngineLogic();
        String version = engineLogic.getVersion();
        assertTrue("Got " + version, version.contains("Hello"));
    }
}
