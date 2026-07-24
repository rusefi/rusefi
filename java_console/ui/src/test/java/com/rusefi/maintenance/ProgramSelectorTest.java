package com.rusefi.maintenance;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.List;

import static com.rusefi.maintenance.ProgramSelector.mainButtonModeFor;
import static com.rusefi.maintenance.ProgramSelector.resolveFlashPort;
import static com.rusefi.maintenance.UpdateMode.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Unit tests for the Update-Firmware main-button decision logic that maps a detected/selected port and
 * the live-connection state to an {@link UpdateMode}. Covers the DFU regression where a board sitting in
 * the STM32 built-in bootloader was mislabeled as a (dead) OpenBLT action. [tag:better_ux_for_flashing]
 */
public class ProgramSelectorTest {
    private static final List<PortResult> NO_PORTS = Collections.emptyList();

    private static PortResult port(SerialPortType type) {
        return new PortResult("COM_TEST", type);
    }

    // ---- mainButtonModeFor ----

    @Test
    public void dfuDeviceMapsToManualDfu() {
        // A connected DFU device must flash via DFU_MANUAL, not be mislabeled as an OpenBLT action.
        assertEquals(DFU_MANUAL, mainButtonModeFor(port(SerialPortType.Dfu), false));
        assertEquals(DFU_MANUAL, mainButtonModeFor(port(SerialPortType.Dfu), true));
    }

    @Test
    public void openBltBoardMapsToManualOpenBlt() {
        assertEquals(OPENBLT_MANUAL, mainButtonModeFor(port(SerialPortType.OpenBlt), false));
        assertEquals(OPENBLT_MANUAL, mainButtonModeFor(port(SerialPortType.OpenBlt), true));
    }

    @Test
    public void liveEcuMapsToAutoOpenBlt() {
        assertEquals(OPENBLT_AUTO, mainButtonModeFor(port(SerialPortType.Ecu), true));
    }

    @Test
    public void offlineFallsBackToManualOpenBlt() {
        assertEquals(OPENBLT_MANUAL, mainButtonModeFor(port(SerialPortType.Ecu), false));
        assertEquals(OPENBLT_MANUAL, mainButtonModeFor(null, false));
    }

    // ---- resolveFlashPort ----

    @Test
    public void selectedDfuPortIsUsedDirectly() {
        PortResult dfu = port(SerialPortType.Dfu);
        assertSame(dfu, resolveFlashPort(dfu, false, NO_PORTS, NO_PORTS));
        assertSame(dfu, resolveFlashPort(dfu, true, NO_PORTS, NO_PORTS));
    }

    @Test
    public void selectedOpenBltPortIsUsedDirectly() {
        PortResult blt = port(SerialPortType.OpenBlt);
        assertSame(blt, resolveFlashPort(blt, false, NO_PORTS, NO_PORTS));
    }

    @Test
    public void offlineWithNullSelectionPrefersDetectedDfuOverOpenBlt() {
        // Right after launch the combo selection may be null/stale: a detected DFU device must win so the
        // button becomes "Manual DFU Update" rather than a dead OpenBLT action.
        PortResult dfu = port(SerialPortType.Dfu);
        PortResult blt = port(SerialPortType.OpenBlt);
        assertSame(dfu, resolveFlashPort(null, false, Collections.singletonList(dfu), Collections.singletonList(blt)));
    }

    @Test
    public void offlineWithNullSelectionFallsBackToDetectedOpenBlt() {
        PortResult blt = port(SerialPortType.OpenBlt);
        assertSame(blt, resolveFlashPort(null, false, NO_PORTS, Collections.singletonList(blt)));
    }

    @Test
    public void liveConnectionKeepsSelectedNonBootloaderPort() {
        PortResult ecu = port(SerialPortType.Ecu);
        PortResult dfu = port(SerialPortType.Dfu);
        // With a live ECU connection we do not override with detected bootloader ports.
        assertSame(ecu, resolveFlashPort(ecu, true, Collections.singletonList(dfu), NO_PORTS));
    }

    @Test
    public void nothingDetectedResolvesToNull() {
        assertNull(resolveFlashPort(null, false, NO_PORTS, NO_PORTS));
    }

    // ---- combined: resolved port feeds the button mode ----

    @Test
    public void offlineDfuHotPlugResultsInManualDfuMode() {
        PortResult dfu = port(SerialPortType.Dfu);
        PortResult resolved = resolveFlashPort(null, false, Collections.singletonList(dfu), NO_PORTS);
        assertEquals(DFU_MANUAL, mainButtonModeFor(resolved, false));
    }

    @Test
    public void staleEcuSelectionIsOverriddenByRenumberedBootloaderPort() {
        // Interrupted flash / OpenBLT reboot renumbering: the combo still shows the old ECU port while
        // the board now sits in the bootloader under a new port name — resolve to the freshly detected
        // bootloader, not the stale selection, so recovery flashes the right port.
        PortResult staleEcu = new PortResult("COM_OLD", SerialPortType.Ecu);
        PortResult renumberedBlt = new PortResult("COM_NEW", SerialPortType.OpenBlt);
        PortResult resolved = resolveFlashPort(staleEcu, false, NO_PORTS, Collections.singletonList(renumberedBlt));
        assertSame(renumberedBlt, resolved);
        assertEquals(OPENBLT_MANUAL, mainButtonModeFor(resolved, false));
    }

    @Test
    public void staleEcuSelectionWithBothBootloadersDetectedPrefersDfu() {
        // Corrupted-board recovery: DFU is the deeper recovery path, it must win over OpenBLT when a
        // stale (dead) ECU selection is being overridden.
        PortResult staleEcu = new PortResult("COM_OLD", SerialPortType.Ecu);
        PortResult dfu = port(SerialPortType.Dfu);
        PortResult blt = port(SerialPortType.OpenBlt);
        PortResult resolved = resolveFlashPort(staleEcu, false,
            Collections.singletonList(dfu), Collections.singletonList(blt));
        assertSame(dfu, resolved);
        assertEquals(DFU_MANUAL, mainButtonModeFor(resolved, false));
    }

    @Test
    public void bootloaderSelectionWinsEvenWithLiveConnection() {
        // A second board sitting in a bootloader while another ECU is live-connected: the explicit
        // bootloader selection must be flashed manually, not rerouted through the live AUTO path.
        PortResult blt = port(SerialPortType.OpenBlt);
        PortResult resolved = resolveFlashPort(blt, true, NO_PORTS, NO_PORTS);
        assertSame(blt, resolved);
        assertEquals(OPENBLT_MANUAL, mainButtonModeFor(resolved, true));
    }

    @Test
    public void firmwareControlsStayVisibleWhileJobRunsWithoutDetectedHardware() {
        assertTrue(ProgramSelector.shouldShowFirmwareControls(true, true, false));
        assertTrue(ProgramSelector.shouldShowFirmwareControls(true, false, true));
    }
}
