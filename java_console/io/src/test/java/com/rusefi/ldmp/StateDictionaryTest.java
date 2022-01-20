package com.rusefi.ldmp;

import com.rusefi.enums.live_data_e;
import org.junit.Test;

public class StateDictionaryTest {
    @Test
    public void testStateDictionaryIsComplete() {
        // we assert that this does not fail
        StateDictionary.INSTANCE.getFields(live_data_e.LDS_ac_control);
    }
}
