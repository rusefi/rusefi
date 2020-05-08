package com.rusefi.test;

import com.rusefi.FiringOrderTSLogic;
import org.junit.Test;

public class FiringOrderTSLogicTest {
    @Test
    public void parseFiringOrderLine() {
        FiringOrderTSLogic.parseLine("FO_1_3_4_2 = 1, // typical inline 4", new FiringOrderTSLogic.State());
    }
}
