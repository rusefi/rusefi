package com.rusefi.io.can;

import org.junit.jupiter.api.Test;

public class PCanHelperTest {
    /**
     * The port scanner calls this on every scan cycle on macOS, where the JNI bridge may be absent
     * (no MacCAN) or present; on Linux CI the PCANBasic class cannot link at all. In every case the
     * answer must be a plain boolean, never a LinkageError escaping into the scanner thread.
     */
    @Test
    public void nativeApiProbeNeverThrows() {
        boolean first = PCanHelper.isNativeApiAvailable();
        // stable across repeated calls: a failed class initialization must not turn into a different error later
        boolean second = PCanHelper.isNativeApiAvailable();
        org.junit.jupiter.api.Assertions.assertEquals(first, second);
    }
}
