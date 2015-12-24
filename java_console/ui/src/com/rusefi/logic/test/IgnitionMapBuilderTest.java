package com.rusefi.logic.test;

import com.rusefi.logic.IgnitionMapBuilder;
import org.junit.Test;

import static junit.framework.Assert.assertEquals;

/**
 * (c) Andrey Belomutskiy 2012-2015
 * 12/24/2015
 */
public class IgnitionMapBuilderTest {
    @Test
    public void testIgnitionMapBuilder() {
        assertEquals(33.0, IgnitionMapBuilder.getTopAdvanceForBore(IgnitionMapBuilder.ChamberStyle.OPEN_CHAMBER));

        assertEquals(22.0, IgnitionMapBuilder.getTopAdvanceForBore(IgnitionMapBuilder.ChamberStyle.SWITL_TUMBLE));
    }
}
