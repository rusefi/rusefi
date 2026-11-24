package com.rusefi.maintenance;

import com.rusefi.PortResult;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class FirmwareOperationsOpenBltHandoffTest {
    @Test
    void scannerCollectsEveryNewOpenBltPortBeforeDecidingWhetherHandoffIsAmbiguous() {
        List<PortResult> candidates = FirmwareOperations.detectNewOpenBltPorts(
            Arrays.asList("COM7", "COM8"), Collections.emptyList(),
            port -> new OpenbltInfo(true, "rusefi.test-board", "1.0", Collections.emptyList()));

        assertEquals(Arrays.asList(
            new PortResult("COM7", com.rusefi.SerialPortType.OpenBlt),
            new PortResult("COM8", com.rusefi.SerialPortType.OpenBlt)), candidates);
    }

    @Test
    void noIniHandoffRequiresSamePortAndMatchingBootloaderBoardIdentity() {
        String signature = "rusEFI master.2026.08.30.test-board.1234567890";
        PortResult matching = new PortResult("COM7", com.rusefi.SerialPortType.OpenBlt, null,
            new OpenbltInfo(true, "rusefi.test-board"));
        PortResult otherBoard = new PortResult("COM7", com.rusefi.SerialPortType.OpenBlt, null,
            new OpenbltInfo(true, "rusefi.other-board"));
        PortResult noIdentity = new PortResult("COM7", com.rusefi.SerialPortType.OpenBlt, null,
            new OpenbltInfo(true, null));
        ConnectedEcuTarget jobTargetB = ConnectedEcuTarget.forManualRecovery("test-board");
        ConnectedEcuTarget unrelatedSharedTargetA = ConnectedEcuTarget.forManualRecovery("other-board");

        assertTrue(FirmwareOperations.isVerifiedNoIniHandoff("COM7", matching, signature, jobTargetB));
        assertFalse(FirmwareOperations.isVerifiedNoIniHandoff("COM7", matching, signature, unrelatedSharedTargetA),
            "the global board-A target must not validate a board-B no-INI handoff");
        assertFalse(FirmwareOperations.isVerifiedNoIniHandoff("COM7", otherBoard, signature, jobTargetB));
        assertFalse(FirmwareOperations.isVerifiedNoIniHandoff("COM7", noIdentity, signature, jobTargetB));
        assertFalse(FirmwareOperations.isVerifiedNoIniHandoff("COM8", matching, signature, jobTargetB),
            "a renamed port has no per-device identity, so continuity cannot be established");
    }
}
