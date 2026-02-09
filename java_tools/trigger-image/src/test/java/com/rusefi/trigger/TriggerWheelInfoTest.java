package com.rusefi.trigger;

import org.junit.jupiter.api.Test;
import org.yaml.snakeyaml.Yaml;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

public class TriggerWheelInfoTest {
    @Test
    public void testReadTriggerWheelInfo() throws IOException {
        String data = "- name: TEST_TRIGGER\n" +
            "  humanName: Test Trigger\n" +
            "  operationMode: FOUR_STROKE_CAM_SENSOR\n" +
            "  isSecondWheelCam: true\n" +
            "  syncEdge: RiseOnly\n" +
            "  useOnlyPrimaryForSync: true\n" +
            "  syncGaps:\n" +
            "  - from: 1.5\n" +
            "    to: 2.5\n" +
            "  teeth:\n" +
            "  - channel: 0\n" +
            "    angle: 0\n" +
            "  - channel: 1\n" +
            "    angle: 52.76\n";

        Yaml yaml = new Yaml();

        Object obj = yaml.load(data);

        if (!(obj instanceof List)) {
                throw new IllegalStateException("Expected a list of triggers");
        }

        @SuppressWarnings("unchecked")
        List<Map<String, Object>> triggerList = (List<Map<String, Object>>) obj;

        TriggerWheelInfo info = TriggerWheelInfo.readTriggerWheelInfo(1, triggerList.get(0));

        assertNotNull(info);
        assertEquals(1, info.getId());
        assertEquals("TEST_TRIGGER", info.getTriggerName());
        assertTrue(info.isKnownOperationMode());
        assertFalse(info.isCrankBased());
        assertTrue(info.isHasSecondChannel());
        assertEquals("RiseOnly", info.getSyncEdge());

        List<TriggerSignal> signals = info.getSignals();
        assertEquals(4, signals.size());
        assertEquals(0.0, signals.get(0).getAngle());
        assertEquals(307.24, signals.get(1).getAngle());

        assertNotNull(info.getGaps());
        assertEquals(1.5, info.getGaps().gapFrom[0]);
        assertEquals(2.5, info.getGaps().gapTo[0]);
    }
}
