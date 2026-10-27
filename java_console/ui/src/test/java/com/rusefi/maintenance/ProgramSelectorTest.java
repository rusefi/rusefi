package com.rusefi.maintenance;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static com.rusefi.maintenance.ProgramSelector.mainButtonModeFor;
import static com.rusefi.maintenance.ProgramSelector.resolveNormalFlashPort;
import static com.rusefi.maintenance.UpdateMode.*;
import static org.junit.jupiter.api.Assertions.*;

/** Normal update selection is capability-aware; DFU recovery is deliberately not a primary-button mode. [tag:better_ux_for_flashing] */
public class ProgramSelectorTest {
    private static PortResult port(String name, SerialPortType type) {
        return new PortResult(name, type);
    }

    @Test
    public void normalModesRequireAUsableLiveEcuButDetectedOpenBltCanUpdateDirectly() {
        PortResult ecu = port("COM_ECU", SerialPortType.Ecu);
        PortResult ecuWithBlt = port("COM_BLT_ECU", SerialPortType.EcuWithOpenblt);
        PortResult blt = port("COM_BOOT", SerialPortType.OpenBlt);
        PortResult dfu = port("DFU", SerialPortType.Dfu);

        assertEquals(DFU_AUTO, mainButtonModeFor(ecu, true, true));
        assertNull(mainButtonModeFor(ecu, true, false),
            "an ECU without OpenBLT must not fall back to it when DFU is unavailable");
        assertEquals(OPENBLT_AUTO, mainButtonModeFor(ecuWithBlt, true, true));
        assertNull(mainButtonModeFor(ecu, false, true));
        assertNull(mainButtonModeFor(ecuWithBlt, false, true));
        assertEquals(OPENBLT_MANUAL, mainButtonModeFor(blt, false, true));
        assertEquals(OPENBLT_MANUAL, mainButtonModeFor(blt, true, false));
        assertNull(mainButtonModeFor(dfu, true, true));
        assertNull(mainButtonModeFor(dfu, false, true));
    }

    @Test
    public void normalTargetRequiresCurrentIdentityAndMatchingLivePort() {
        PortResult ecu = port("COM_ECU", SerialPortType.Ecu);
        PortResult blt = port("COM_BOOT", SerialPortType.OpenBlt);
        PortResult dfu = port("DFU", SerialPortType.Dfu);
        List<PortResult> detected = Arrays.asList(ecu, blt, dfu);

        assertSame(ecu, resolveNormalFlashPort(ecu, true, "COM_ECU", detected, false));
        assertNull(resolveNormalFlashPort(ecu, true, "COM_OTHER", detected, false),
            "a live connection to another port must not authorize this selection");
        assertNull(resolveNormalFlashPort(ecu, false, null, detected, false),
            "an ECU port without a live LinkManager is not a manual programming target");
        assertNull(resolveNormalFlashPort(port("STALE", SerialPortType.Ecu), false, null, detected, false));
        assertNull(resolveNormalFlashPort(null, false, null, detected, false),
            "do not choose the first discovered bootloader implicitly");
        assertSame(blt, resolveNormalFlashPort(blt, false, null, detected, false));
        assertNull(resolveNormalFlashPort(dfu, true, "DFU", detected, false));
    }

    @Test
    public void automaticCanRequiresOptInAndMatchingLiveTransport() {
        PortResult socketCan = port(LinkManager.SOCKET_CAN, SerialPortType.Ecu);
        PortResult pcan = port(LinkManager.PCAN, SerialPortType.Ecu);
        PortResult slcan = port(LinkManager.SLCAN_PREFIX + "COM_SLCAN", SerialPortType.Ecu);
        List<PortResult> detected = Arrays.asList(socketCan, pcan, slcan);

        assertNull(resolveNormalFlashPort(socketCan, true, LinkManager.SOCKET_CAN, detected, false));
        assertNull(resolveNormalFlashPort(socketCan, false, LinkManager.SOCKET_CAN, detected, true));
        assertSame(socketCan, resolveNormalFlashPort(socketCan, true, LinkManager.SOCKET_CAN, detected, true));
        assertNull(resolveNormalFlashPort(socketCan, true, LinkManager.PCAN, detected, true));
        assertNull(resolveNormalFlashPort(pcan, true, LinkManager.SOCKET_CAN, detected, true));
        assertSame(pcan, resolveNormalFlashPort(pcan, true, LinkManager.PCAN, detected, true));
        assertNull(resolveNormalFlashPort(slcan, true, LinkManager.SLCAN_PREFIX + "COM_SLCAN", detected, true));
        assertFalse(ProgramSelector.hasRealSerialPort(detected));
        assertTrue(ProgramSelector.hasFirmwareTarget(false, socketCan));
        assertEquals(OPENBLT_AUTO, mainButtonModeFor(socketCan, true, false));
    }

    @Test
    public void dfuRecoveryUsesItsOwnBundleAndPlatformGateAndNeverEnablesNormalAction() {
        PortResult dfu = port("DFU", SerialPortType.Dfu);
        assertTrue(ProgramSelector.canUseManualDfu(true, true, "universal", "uaefi"),
            "the universal-bundle recovery exception remains supported");
        assertFalse(ProgramSelector.canUseManualDfu(false, false, "universal", "uaefi"));
        assertTrue(ProgramSelector.canUseManualDfu(true, false, "uaefi", "uaefi"));
        assertFalse(ProgramSelector.canUseManualDfu(true, true, "uaefi", "uaefi"));
        assertNull(mainButtonModeFor(dfu, true, true));
        assertFalse(ProgramSelector.shouldEnableMainButton(false, false, null));
        assertTrue(ProgramSelector.shouldEnableMainButton(true, false, OPENBLT_MANUAL));
        assertFalse(ProgramSelector.shouldEnableMainButton(true, true, OPENBLT_MANUAL));
    }

    @Test
    public void firmwareControlsStayVisibleWhileJobRunsWithoutDetectedHardware() {
        assertTrue(ProgramSelector.shouldShowFirmwareControls(true, true, false));
        assertTrue(ProgramSelector.shouldShowFirmwareControls(true, false, true));
        assertFalse(ProgramSelector.shouldShowFirmwareControls(true, false, false));
    }
}
