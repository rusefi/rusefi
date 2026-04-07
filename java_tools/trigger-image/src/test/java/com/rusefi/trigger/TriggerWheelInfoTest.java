package com.rusefi.trigger;

import org.junit.jupiter.api.Test;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class TriggerWheelInfoTest {
    @Test
    public void testReadTriggerWheelInfo() throws IOException {
        String data = "TRIGGERTYPE 1 2 TEST_TRIGGER 0.00\n" +
                "knownOperationMode=true\n" +
                "crankBased=false\n" +
                "hasSecondChannel=true\n" +
                "isSecondWheelCam=true\n" +
                "cycleDuration=720\n" +
                "gapsCount=1\n" +
                "syncEdge=RiseOnly\n" +
                "gapFrom.0=1.5\n" +
                "gapTo.0=2.5\n" +
                "# end of meta section\n" +
                "event 0 0 1 0.00 3.00\n" +
                "event 1 1 1 52.76 nan\n";

        BufferedReader reader = new BufferedReader(new StringReader(data));
        String firstLine = reader.readLine();
        TriggerWheelInfo info = TriggerWheelInfo.readTriggerWheelInfo(firstLine, reader);

        assertNotNull(info);
        assertEquals(1, info.getId());
        assertEquals("TEST_TRIGGER", info.getTriggerName());
        assertTrue(info.isKnownOperationMode());
        assertFalse(info.isCrankBased());
        assertTrue(info.isHasSecondChannel());
        assertEquals("RiseOnly", info.getSyncEdge());

        List<TriggerSignal> signals = info.getSignals();
        assertEquals(2, signals.size());
        assertEquals(0.0, signals.get(0).getAngle());
        assertEquals(52.76, signals.get(1).getAngle());

        assertNotNull(info.getGaps());
        assertEquals(1.5, info.getGaps().gapFrom[0]);
        assertEquals(2.5, info.getGaps().gapTo[0]);
    }

    @Test
    public void testReadTriggerWheelInfoWithComments() throws IOException {
        String data = "TRIGGERTYPE 2 1 COMMENT_TEST 10.0\n" +
                "crankBased=true\n" +
                "# some comment\n" +
                "event 0 0 1 0.00 1.00\n";

        BufferedReader reader = new BufferedReader(new StringReader(data));
        String firstLine = reader.readLine();
        TriggerWheelInfo info = TriggerWheelInfo.readTriggerWheelInfo(firstLine, reader);

        assertNotNull(info);
        assertEquals(2, info.getId());
        assertTrue(info.isCrankBased());
        assertEquals(1, info.getSignals().size());
    }
}
