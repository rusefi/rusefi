package com.rusefi.ldmp;

import com.rusefi.config.generated.TsOutputs;
import com.rusefi.enums.live_data_e;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class StateDictionaryTest {

    private static final int OUTPUTS_SIZE = 612;

    @Test
    public void testStateDictionaryIsComplete() {
        // we assert that this does not fail
        StateDictionary.INSTANCE.getFields(live_data_e.LDS_ac_control);
    }

    @Test
    public void testStructSize() {
        // todo: do we have this struct size anywhere?
        assertEquals(OUTPUTS_SIZE, StateDictionary.getSize(TsOutputs.VALUES));
    }

    @Test
    public void testOffset() {
        assertEquals(OUTPUTS_SIZE, StateDictionary.INSTANCE.getOffset(live_data_e.LDS_knock_controller));
    }
}
