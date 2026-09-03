package com.rusefi.ui.livedocs;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

class LiveDocsRegistryTest {
    @Test
    void slicesOutputWithoutResponseCodeOrSourceMutation() {
        byte[] overallOutputs = {0, 10, 11, 12, 13, 14};
        byte[] original = overallOutputs.clone();

        assertArrayEquals(new byte[]{11, 12, 13},
            LiveDocsRegistry.sliceOutput(overallOutputs, 1, 3));
        assertArrayEquals(original, overallOutputs);
    }

    @Test
    void absentOrIncompleteOutputReturnsNoLiveData() {
        assertNull(LiveDocsRegistry.sliceOutput(null, 0, 1));
        assertNull(LiveDocsRegistry.sliceOutput(new byte[]{0, 10}, 1, 1));
    }
}
