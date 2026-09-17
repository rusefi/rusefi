package com.rusefi.ui;

import com.rusefi.io.can.slcan.SlcanClient;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

class SlcanTraceTest {
    @Test
    void exportPreservesBusExtendedIdsAndRemoteFrames() {
        assertEquals("(1.234000) can0 123#AABB",
                SlcanTab.formatCandump(1234, SlcanClient.Frame.parse("t1232AABB", true)));
        assertEquals("(1.234000) can1 00000123#AA",
                SlcanTab.formatCandump(1234, SlcanClient.Frame.parse("&T000001231AA")));
        assertEquals("(1.234000) can2 123#R8",
                SlcanTab.formatCandump(1234, SlcanClient.Frame.parse("$r1238")));
        assertEquals("(1.234000) canUnknown 123#",
                SlcanTab.formatCandump(1234, SlcanClient.Frame.parse("t1230")));
    }
}
