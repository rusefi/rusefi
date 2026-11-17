package com.rusefi.io.can;

import org.junit.jupiter.api.Test;

public class PCanHelperTest {
    @Test
    public void windows500kTimingAllowsTwoQuantaOfResynchronization() {
        // [tag:pcan_500k_sjw] Hardware reproduction: one-Tq SJW repeats
        // 23 83 83 83 83 83 83 83 during a GPEC2A configuration read.
        int btr = PCanHelper.get500kBaudrate(true).getValue();
        int prescaler = ((btr >> 8) & 0x3f) + 1;
        int segment1 = (btr & 0x0f) + 1;
        int segment2 = ((btr >> 4) & 0x07) + 1;
        org.junit.jupiter.api.Assertions.assertEquals(500_000,
            8_000_000 / (prescaler * (1 + segment1 + segment2)));
        org.junit.jupiter.api.Assertions.assertEquals(0.875,
            (1.0 + segment1) / (1 + segment1 + segment2));
        org.junit.jupiter.api.Assertions.assertEquals(2, ((btr >> 14) & 0x03) + 1);
        org.junit.jupiter.api.Assertions.assertSame(
            peak.can.basic.TPCANBaudrate.PCAN_BAUD_500K, PCanHelper.get500kBaudrate(false));
    }

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
